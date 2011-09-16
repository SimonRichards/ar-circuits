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
					/*System.out.println(j + " " + res + " " +
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
						voltageSources[ji].setCurrent(/*ji,*/ res);
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
		//System.out.println((System.currentTimeMillis()-lastFrameTime)/(double) iter);
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
}

