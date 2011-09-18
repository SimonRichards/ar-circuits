#include "stdafx.h"
#include "components.h"
#include "simulator.h"
#include "node.h"
#include <Windows.h>
#include <math.h>
#include <iostream>
#include <cmath>

using namespace std;

namespace circuit_sim {

	Simulator::Simulator() {
		lastTime = 0;
		secTime = 0;
		frames = 0;
		steps = 0;
		framerate = 0;
		steprate = 0;
	}

	void Simulator::analyseCircuit() {
		if (components.empty())
			return;

		int i, j;
		int vscount = 0;

		//TODO: delete all old nodes
		nodes.clear();
		bool gotGround = false;
		bool gotRail = false;
		Component *volt; //todo set this to main Voltage Source

		// if no ground, and no rails, then the voltage elm's first terminal
		// is ground
		Node cn; //TODO: all the x-y stuff won't work without a gui
		nodes.push_back(cn);

		// allocate nodes and voltage sources
		for each (Component* component in components) {
			int ivs = component->getVoltageSourceCount();
			/*
			if (1) {
			//create new node
			Node cn;
			NodeLink cnl;
			cnl.num = j;
			cnl.elm = component;
			cn.links.push_back(cnl);
			component->setNode(j, nodes.size());
			nodes.push_back(cn);
			} else {
			//link to existing node
			NodeLink cnl;
			cnl.num = j;
			cnl.elm = component;
			nodes[k].links.push_back(cnl);
			component->setNode(j, k);
			// if it's the ground node, make sure the node voltage is 0,
			// cause it may not get set later
			}
			if (k == 0)
				component->setNodeVoltage(j, 0);
			vscount += ivs;*/
		}
		voltageSources = new Component*[vscount];
		vscount = 0;
		circuitNonLinear = false;
		//cout <<"ac3");

		//// determine if circuit is nonlinear TODO reimplement if needed
		for each (Component* component in components) {
			//	if (component->nonLinear())
			//		circuitNonLinear = true;
			int ivs = component->getVoltageSourceCount();
			for (j = 0; j != ivs; j++) {
				voltageSources[vscount] = component;
				component->setVoltageSource(vscount++); //set voltage source id
			}
		}
		voltageSourceCount = vscount;

		int matrixSize = nodes.size()-1 + vscount;
		circuitMatrix = new double*[matrixSize]; 
		for (i = 0; i < matrixSize; i++) circuitMatrix[i] = new double[matrixSize];
		circuitRightSide = new double[matrixSize];
		origMatrix = new double*[matrixSize];
		for (i = 0; i < matrixSize; i++) origMatrix[i] = new double[matrixSize];
		origRightSide = new double[matrixSize];
		circuitMatrixSize = circuitMatrixFullSize = matrixSize;
		circuitRowInfo = new RowInfo[matrixSize];
		circuitPermute = new int[matrixSize];
		int vs = 0;
		circuitNeedsMap = false;

		// stamp linear circuit elements
		for each (Component* component in components) {
			component->stamp();
		}
		//cout <<"ac4");

		// determine nodes that are unconnected
		bool *closure = new bool[nodes.size()];
		bool *tempclosure = new bool[nodes.size()]; //TODO delete later
		bool changed = true;
		closure[0] = true;
		while (changed) {
			changed = false;
			for each (Component* component in components) {
				// loop through all ce's nodes to see if they are connected
				// to other nodes not in closure
				for (j = 0; j < component->getPostCount(); j++) {
					if (!closure[component->getNode(j)]) {
						/* TODO: reinstate if we need a ground element
						if (component->hasGroundConnection(j))
							closure[component->getNode(j)] = changed = true;
						*/
						continue;
					}
					int k;
					for (k = 0; k != component->getPostCount(); k++) {
						if (j == k)
							continue;
						int kn = component->getNode(k);
						if (component->getConnection(j, k) && !closure[kn]) {
							closure[kn] = true;
							changed = true;
						}
					}
				}
			}
			if (changed)
				continue;

			// connect unconnected nodes
			for (i = 0; i != nodes.size(); i++)
				if (!closure[i]) {
					cout << "node " << i << " unconnected" << endl;
					stampResistor(0, i, 1e8);
					closure[i] = true;
					changed = true;
					break;
				}
		}
		//cout <<"ac5");

		for each (Component* component in components) {
			// look for inductors with no current path
			if (component->getType() == INDUCTOR) {
				FindPathInfo fpi(FindPathInfo::INDUCT, component,
					component->getNode(1), nodes.size());
				// first try findPath with maximum depth of 5, to avoid slowdowns
				if (!fpi.findPath(component->getNode(0), 5, components) &&
					!fpi.findPath(component->getNode(0), components)) {
						cout << " no path" << endl;
						component->reset();
				}
			}
			// look for current sources with no current path
			if (component->getType() == CURRENT_SOURCE) {
				FindPathInfo fpi(FindPathInfo::INDUCT, component, component->getNode(1), nodes.size());
				if (!fpi.findPath(component->getNode(0), components)) {
					cerr << "No path for current source!" << endl;
					return;
				}
			}
			// look for voltage source loops
			if ((component->getType() == SOURCE)  /*&& component.getPostCount() == 2)*/ ||
				(component->getType() == WIRE))  {
					FindPathInfo fpi(FindPathInfo::VOLTAGE, component,
						component->getNode(1), nodes.size());
					if (fpi.findPath(component->getNode(0), components)) {
						cerr << "Voltage source/wire loop with no resistance!" << endl;
						return;
					}
			}
			// look for shorted caps, or caps w/ voltage but no R
			if (component->getType() == CAPACITOR) {
				FindPathInfo fpi(FindPathInfo::SHORT, component, component->getNode(1), nodes.size());
				if (fpi.findPath(component->getNode(0), components)) {
					cout << " shorted" << endl;
					component->reset();
				} else {
					FindPathInfo fpi2(FindPathInfo::CAP_V, component, component->getNode(1), nodes.size());
					if (fpi2.findPath(component->getNode(0), components)) {
						cerr << "Capacitor loop with no resistance!" << endl;
						return;
					}
				}
			}
		}
		//cout <<"ac6");

		// simplify the matrix; this speeds things up quite a bit
		for (i = 0; i != matrixSize; i++) {
			int qm = -1, qp = -1;
			double qv = 0;
			RowInfo re = circuitRowInfo[i];
			/*cout <<"row " + i + " " + re.lsChanges + " " + re.rsChanges + " " +
			re.dropRow);*/
			if (re.lsChanges || re.dropRow || re.rsChanges)
				continue;
			double rsadd = 0;

			// look for rows that can be removed
			for (j = 0; j != matrixSize; j++) {
				double q = circuitMatrix[i][j];
				if (circuitRowInfo[j].type == RowInfo::ROW_CONST) {
					// keep a running total of const values that have been
					// removed already
					rsadd -= circuitRowInfo[j].value*q;
					continue;
				}
				if (q == 0)
					continue;
				if (qp == -1) {
					qp = j;
					qv = q;
					continue;
				}
				if (qm == -1 && q == -qv) {
					qm = j;
					continue;
				}
				break;
			}
			//cout <<"line " + i + " " + qp + " " + qm + " " + j);
			/*if (qp != -1 && circuitRowInfo[qp].lsChanges) {
			cout <<"lschanges");
			continue;
			}
			if (qm != -1 && circuitRowInfo[qm].lsChanges) {
			cout <<"lschanges");
			continue;
			}*/
			if (j == matrixSize) {
				if (qp == -1) {
					cerr << "Matrix error" << endl;
					return;
				}
				RowInfo elt = circuitRowInfo[qp];
				if (qm == -1) {
					// we found a row with only one nonzero entry; that value
					// is a constant
					int k;
					for (k = 0; elt.type == RowInfo::ROW_EQUAL && k < 100; k++) {
						// follow the chain
						/*cout <<"following equal chain from " +
						i + " " + qp + " to " + elt.nodeEq);*/
						qp = elt.nodeEq;
						elt = circuitRowInfo[qp];
					}
					if (elt.type == RowInfo::ROW_EQUAL) {
						// break equal chains
						//cout <<"Break equal chain");
						elt.type = RowInfo::ROW_NORMAL;
						continue;
					}
					if (elt.type != RowInfo::ROW_NORMAL) {
						cout << "type already " << elt.type << " for " << qp << "!" << endl;
						continue;
					}
					elt.type = RowInfo::ROW_CONST;
					elt.value = (circuitRightSide[i]+rsadd)/qv;
					circuitRowInfo[i].dropRow = true;
					//cout <<qp + " * " + qv + " = const " + elt.value);
					i = -1; // start over from scratch
				} else if (circuitRightSide[i]+rsadd == 0) {
					// we found a row with only two nonzero entries, and one
					// is the negative of the other; the values are equal
					if (elt.type != RowInfo::ROW_NORMAL) {
						//cout <<"swapping");
						int qq = qm;
						qm = qp; qp = qq;
						elt = circuitRowInfo[qp];
						if (elt.type != RowInfo::ROW_NORMAL) {
							// we should follow the chain here, but this
							// hardly ever happens so it's not worth worrying
							// about
							cout << "swap failed" << endl;
							continue;
						}
					}
					elt.type = RowInfo::ROW_EQUAL;
					elt.nodeEq = qm;
					circuitRowInfo[i].dropRow = true;
					//cout <<qp + " = " + qm);
				}
			}
		}
		//cout <<"ac7");

		// find size of new matrix
		int nn = 0;
		for (i = 0; i != matrixSize; i++) {
			RowInfo elt = circuitRowInfo[i];
			if (elt.type == RowInfo::ROW_NORMAL) {
				elt.mapCol = nn++;
				//cout <<"col " + i + " maps to " + elt.mapCol);
				continue;
			}
			if (elt.type == RowInfo::ROW_EQUAL) {
				RowInfo* e2;
				// resolve chains of equality; 100 max steps to avoid loops
				for (j = 0; j != 100; j++) {
					e2 = &circuitRowInfo[elt.nodeEq]; //TODO may not work
					if (e2->type != RowInfo::ROW_EQUAL)
						break;
					if (i == e2->nodeEq)
						break;
					elt.nodeEq = e2->nodeEq;
				}
			}
			if (elt.type == RowInfo::ROW_CONST)
				elt.mapCol = -1;
		}
		for (i = 0; i != matrixSize; i++) {
			RowInfo elt = circuitRowInfo[i];
			if (elt.type == RowInfo::ROW_EQUAL) {
				RowInfo e2 = circuitRowInfo[elt.nodeEq];
				if (e2.type == RowInfo::ROW_CONST) {
					// if something is equal to a const, it's a const
					elt.type = e2.type;
					elt.value = e2.value;
					elt.mapCol = -1;
					//cout <<i + " = [late]const " + elt.value);
				} else {
					elt.mapCol = e2.mapCol;
					//cout <<i + " maps to: " + e2.mapCol);
				}
			}
		}
		//cout <<"ac8");

		// make the new, simplified matrix
		int newsize = nn;
		double **newmatx = new double*[newsize];
		for (i = 0; i < newsize; i++) newmatx[i] = new double[newsize];
		double *newrs    = new double[newsize];
		int ii = 0;
		for (i = 0; i != matrixSize; i++) {
			RowInfo rri = circuitRowInfo[i];
			if (rri.dropRow) {
				rri.mapRow = -1;
				continue;
			}
			newrs[ii] = circuitRightSide[i];
			rri.mapRow = ii;
			//cout <<"Row " + i + " maps to " + ii);
			for (j = 0; j != matrixSize; j++) {
				RowInfo ri = circuitRowInfo[j];
				if (ri.type == RowInfo::ROW_CONST)
					newrs[ii] -= ri.value*circuitMatrix[i][j];
				else
					newmatx[ii][ri.mapCol] += circuitMatrix[i][j];
			}
			ii++;
		}

		circuitMatrix = newmatx;
		circuitRightSide = newrs;
		matrixSize = circuitMatrixSize = newsize;
		for (i = 0; i != matrixSize; i++)
			origRightSide[i] = circuitRightSide[i];
		for (i = 0; i != matrixSize; i++)
			for (j = 0; j != matrixSize; j++)
				origMatrix[i][j] = circuitMatrix[i][j];
		circuitNeedsMap = true;

		/*
		cout <<"matrixSize = " + matrixSize + " " + circuitNonLinear);
		for (j = 0; j != circuitMatrixSize; j++) {
		for (i = 0; i != circuitMatrixSize; i++)
		System.out.print(circuitMatrix[j][i] + " ");
		System.out.print("  " + circuitRightSide[j] + "\n");
		}
		System.out.print("\n");*/

		// if a matrix is linear, we can do the lu_factor here instead of
		// needing to do it every frame
		if (!circuitNonLinear) {
			if (!lu_factor(circuitMatrix, circuitMatrixSize, circuitPermute)) {
				cerr << "Singular matrix!" << endl;
				return;
			}
		}
	}

	void Simulator::runCircuit() {
		int iter;
		bool debugprint = dumpMatrix;
		dumpMatrix = false;
		long steprate = (long) (160*getIterCount());
		long tm = GetTickCount();
		long lit = lastIterTime;
		if (1000 >= steprate*(tm-lastIterTime))
			return;
		for (iter = 1; ; iter++) {
			unsigned int i, j, k, subiter;
			for each (Component* component in components) {
				component->startIteration();
			}
			steps++;
			int subiterCount = 5000;
			for (subiter = 0; subiter != subiterCount; subiter++) {
				converged = true;
				subIterations = subiter;
				for (i = 0; i != circuitMatrixSize; i++)
					circuitRightSide[i] = origRightSide[i];
				if (circuitNonLinear) {
					for (i = 0; i != circuitMatrixSize; i++)
						for (j = 0; j != circuitMatrixSize; j++)
							circuitMatrix[i][j] = origMatrix[i][j];
				}
				for each (Component* component in components) {
					component->step();
				}

				bool printit = debugprint;
				debugprint = false;
				for (j = 0; j != circuitMatrixSize; j++) {
					for (i = 0; i != circuitMatrixSize; i++) {
						double x = circuitMatrix[i][j];
						if (x != x) {
							cerr << "nan/infinite matrix!" << endl;
							return;
						}
					}
				}
				if (printit) {
					for (j = 0; j != circuitMatrixSize; j++) {
						for (i = 0; i != circuitMatrixSize; i++)
							cout << circuitMatrix[j][i] << "," << endl;
						cout << "  " << circuitRightSide[j] << endl;
					}
					cout << "\n" << endl;
				}
				if (circuitNonLinear) {
					if (converged && subiter > 0)
						break;
					if (!lu_factor(circuitMatrix, circuitMatrixSize,
						circuitPermute)) {
							cerr << "Singular matrix!" << endl;
							return;
					}
				}
				lu_solve(circuitMatrix, circuitMatrixSize, circuitPermute,
					circuitRightSide);

				for (j = 0; j != circuitMatrixFullSize; j++) {
					RowInfo ri = circuitRowInfo[j];
					double res = 0;
					if (ri.type == RowInfo::ROW_CONST)
						res = ri.value;
					else
						res = circuitRightSide[ri.mapCol];
					/*cout <<j + " " + res + " " +
					ri.type + " " + ri.mapCol);*/
					if (res != res) {
						converged = false;
						//debugprint = true;
						break;
					}

					if (j < nodes.size()-1) {
						Node cn = nodes[j+1];
						for (k = 0; k != cn.links.size(); k++) {
							NodeLink cnl = cn.links[k];
							cnl.elm->setNodeVoltage(cnl.num, res);
						}
					} else {
						int ji = j-(nodes.size()-1);
						//cout << "setting vsrc " + ji + " to " + res);
						voltageSources[ji]->setCurrent(/*ji,*/ res);
					}
				}
				if (!circuitNonLinear)
					break;
			}
			if (subiter > 5)
				cout << "converged after " << subiter << " iterations\n" << endl;
			if (subiter == subiterCount) {
				cerr << "Convergence failed!" << endl;
				break;
			}
			t += timeStep;
			//for (i = 0; i != scopeCount; i++)
			//	scopes[i].timeStep();
			tm = GetTickCount();
			lit = tm;
			if (iter*1000 >= steprate*(tm-lastIterTime) ||
				(tm-lastFrameTime > 500))
				break;
		}
		lastIterTime = lit;
		//cout <<(System.currentTimeMillis()-lastFrameTime)/(double) iter);
	}


	// control voltage source vs with voltage from n1 to n2 (must
	// also call stampVoltageSource())
	void Simulator::stampVCVS(int n1, int n2, double coef, int vs) {
		int vn = nodes.size()+vs;
		stampMatrix(vn, n1, coef);
		stampMatrix(vn, n2, -coef);
	}

	// stamp independent voltage source #vs, from n1 to n2, amount v
	void Simulator::stampVoltageSource(int n1, int n2, int vs, double v) {
		int vn = nodes.size()+vs;
		stampMatrix(vn, n1, -1);
		stampMatrix(vn, n2, 1);
		stampRightSide(vn, v);
		stampMatrix(n1, vn, 1);
		stampMatrix(n2, vn, -1);
	}

	// use this if the amount of voltage is going to be updated in doStep()
	void Simulator::stampVoltageSource(int n1, int n2, int vs) {
		int vn = nodes.size()+vs;
		stampMatrix(vn, n1, -1);
		stampMatrix(vn, n2, 1);
		stampRightSide(vn);
		stampMatrix(n1, vn, 1);
		stampMatrix(n2, vn, -1);
	}

	void Simulator::updateVoltageSource(int n1, int n2, int vs, double v) {
		int vn = nodes.size()+vs;
		stampRightSide(vn, v);
	}

	void Simulator::stampResistor(int n1, int n2, double r) {
		double r0 = 1/r;
		if (r == 0) {
			cout << "bad resistance " << r << " " << r0 << "\n" << endl;
			int a = 0;
			a /= a;
		}
		stampMatrix(n1, n1, r0);
		stampMatrix(n2, n2, r0);
		stampMatrix(n1, n2, -r0);
		stampMatrix(n2, n1, -r0);
	}


	void Simulator::stampConductance(int n1, int n2, double r0) {
		stampMatrix(n1, n1, r0);
		stampMatrix(n2, n2, r0);
		stampMatrix(n1, n2, -r0);
		stampMatrix(n2, n1, -r0);
	}

	// current from cn1 to cn2 is equal to voltage from vn1 to 2, divided by g
	void Simulator::stampVCCurrentSource(int cn1, int cn2, int vn1, int vn2, double g) {
		stampMatrix(cn1, vn1, g);
		stampMatrix(cn2, vn2, g);
		stampMatrix(cn1, vn2, -g);
		stampMatrix(cn2, vn1, -g);
	}

	void Simulator::stampCurrentSource(int n1, int n2, double i) {
		stampRightSide(n1, -i);
		stampRightSide(n2, i);
	}

	// stamp a current source from n1 to n2 depending on current through vs
	void Simulator::stampCCCS(int n1, int n2, int vs, double gain) {
		int vn = nodes.size()+vs;
		stampMatrix(n1, vn, gain);
		stampMatrix(n2, vn, -gain);
	}

	// stamp value x in row i, column j, meaning that a voltage change
	// of dv in node j will increase the current into node i by x dv.
	// (Unless i or j is a voltage source node.)
	void Simulator::stampMatrix(int i, int j, double x) {
		if (i > 0 && j > 0) {
			if (circuitNeedsMap) {
				i = circuitRowInfo[i-1].mapRow;
				RowInfo ri = circuitRowInfo[j-1];
				if (ri.type == RowInfo::ROW_CONST) {
					//cout << "Stamping constant " + i + " " + j + " " + x);
					circuitRightSide[i] -= x*ri.value;
					return;
				}
				j = ri.mapCol;
				//cout << "stamping " + i + " " + j + " " + x);
			} else {
				i--;
				j--;
			}
			circuitMatrix[i][j] += x;
		}
	}

	// stamp value x on the right side of row i, representing an
	// independent current source flowing into node i
	void Simulator::stampRightSide(int i, double x) {
		if (i > 0) {
			if (circuitNeedsMap) {
				i = circuitRowInfo[i-1].mapRow;
				//cout << "stamping " + i + " " + x);
			} else
				i--;
			circuitRightSide[i] += x;
		}
	}

	// indicate that the value on the right side of row i changes in doStep()
	void Simulator::stampRightSide(int i) {
		//cout << "rschanges true " + (i-1));
		if (i > 0)
			circuitRowInfo[i-1].rsChanges = true;
	}

	// indicate that the values on the left side of row i change in doStep()
	void Simulator::stampNonLinear(int i) {
		if (i > 0)
			circuitRowInfo[i-1].lsChanges = true;
	}

	double Simulator::getIterCount() {
		//return (Math.exp((speedBar.getValue()-1)/24.) + .5);
		return .1*exp((172-61)/24.); //TODO?
	}


	// factors a matrix into upper and lower triangular matrices by
	// gaussian elimination.  On entry, a[0..n-1][0..n-1] is the
	// matrix to be factored.  ipvt[] returns an integer vector of pivot
	// indices, used in the lu_solve() routine.
	bool Simulator::lu_factor(double **a, int n, int *ipvt) {
		int i,j,k;

		double *scaleFactors = new double[n];

		// divide each row by its largest element, keeping track of the
		// scaling factors
		for (i = 0; i != n; i++) { 
			double largest = 0;
			for (j = 0; j != n; j++) {
				double x = fabs(a[i][j]);
				if (x > largest)
					largest = x;
			}
			// if all zeros, it's a singular matrix
			if (largest == 0)
				return false;
			scaleFactors[i] = 1.0/largest;
		}

		// use Crout's method; loop through the columns
		for (j = 0; j != n; j++) {

			// calculate upper triangular elements for this column
			for (i = 0; i != j; i++) {
				double q = a[i][j];
				for (k = 0; k != i; k++)
					q -= a[i][k]*a[k][j];
				a[i][j] = q;
			}

			// calculate lower triangular elements for this column
			double largest = 0;
			int largestRow = -1;
			for (i = j; i != n; i++) {
				double q = a[i][j];
				for (k = 0; k != j; k++)
					q -= a[i][k]*a[k][j];
				a[i][j] = q;
				double x = fabs(q);
				if (x >= largest) {
					largest = x;
					largestRow = i;
				}
			}

			// pivoting
			if (j != largestRow) {
				double x;
				for (k = 0; k != n; k++) {
					x = a[largestRow][k];
					a[largestRow][k] = a[j][k];
					a[j][k] = x;
				}
				scaleFactors[largestRow] = scaleFactors[j];
			}

			// keep track of row interchanges
			ipvt[j] = largestRow;

			// avoid zeros
			if (a[j][j] == 0.0) {
				cout << "avoided zero" << endl;
				a[j][j]=1e-18;
			}

			if (j != n-1) {
				double mult = 1.0/a[j][j];
				for (i = j+1; i != n; i++)
					a[i][j] *= mult;
			}
		}
		return true;
	}

	// Solves the set of n linear equations using a LU factorization
	// previously performed by lu_factor.  On input, b[0..n-1] is the right
	// hand side of the equations, and on output, contains the solution.
	void Simulator::lu_solve(double** a, int n, int* ipvt, double* b) {
		int i;

		// find first nonzero b element
		for (i = 0; i != n; i++) {
			int row = ipvt[i];

			double swap = b[row];
			b[row] = b[i];
			b[i] = swap;
			if (swap != 0)
				break;
		}

		int bi = i++;
		for (; i < n; i++) {
			int row = ipvt[i];
			int j;
			double tot = b[row];

			b[row] = b[i];
			// forward substitution using the lower triangular matrix
			for (j = bi; j < i; j++)
				tot -= a[i][j]*b[j];
			b[i] = tot;
		}
		for (i = n-1; i >= 0; i--) {
			double tot = b[i];

			// back-substitution using the upper triangular matrix
			int j;
			for (j = i+1; j != n; j++)
				tot -= a[i][j]*b[j];
			b[i] = tot/a[i][i];
		}
	}

	FindPathInfo::FindPathInfo(int t, Component* c, int d, int numNodes) {
		dest = d;
		type = t;
		firstElm = c;
		used = new bool[numNodes];
	}

	bool FindPathInfo::findPath(int n1, int depth, vector<Component*> components) {
		if (n1 == dest)
			return true;
		if (depth-- == 0)
			return false;
		if (used[n1]) {
			//cout <<"used " + n1);
			return false;
		}
		used[n1] = true;
		int i;
		for each(Component* component in components) {
			if (component == firstElm)
				continue;
			if (type == INDUCT) {
				if (component->getType() == CURRENT_SOURCE)
					continue;
			}
			if (type == VOLTAGE) {
				if (!(component->getType() == WIRE || component->getType() == SOURCE))
					continue;
			}
			if (type == SHORT && !(component->getType() == WIRE))
				continue;
			if (type == CAP_V) {
				if (!(component->getType() == WIRE || component->getType() == CAPACITOR ||
					component->getType() == SOURCE))
					continue;
			}
			if (n1 == 0) {
				// look for posts which have a ground connection;
				// our path can go through ground
				int j;
				for (j = 0; j != component->getPostCount(); j++)
					if (/*component->hasGroundConnection(j) &&*/
						findPath(component->getNode(j), depth, components)) {
							used[n1] = false;
							return true;
					}
			}
			int j;
			for (j = 0; j != component->getPostCount(); j++) {
				//cout <<ce + " " + component->getNode(j));
				if (component->getNode(j) == n1)
					break;
			}
			if (j == component->getPostCount())
				continue;
			if (/*component->hasGroundConnection(j) &&*/ findPath(0, depth, components)) {
				//cout <<ce + " has ground");
				used[n1] = false;
				return true;
			}
			if (type == INDUCT && component->getType() == INDUCTOR) {
				double c = component->getCurrent();
				if (j == 0)
					c = -c;
				//cout <<"matching " + c + " to " + firstElm.getCurrent());
				//cout <<component + " " + firstElm);
				if (abs(c-firstElm->getCurrent()) > 1e-10)
					continue;
			}
			int k;
			for (k = 0; k != component->getPostCount(); k++) {
				if (j == k)
					continue;
				//cout <<ce + " " + component->getNode(j) + "-" + component->getNode(k));
				if (/*component->getConnection(j, k) &&*/ findPath(component->getNode(k), depth, components)) {
					//cout <<"got findpath " + n1);
					used[n1] = false;
					return true;
				}
				//cout <<"back on findpath " + n1);
			}
		}
		used[n1] = false;
		//cout <<n1 + " failed");
		return false;
	}
}

