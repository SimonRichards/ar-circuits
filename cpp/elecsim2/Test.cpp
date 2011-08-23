#include "Logic.hpp"
using namespace std;

// Test simulation runs
int main (int argc, char** argv) {
        int passCount = 0, failCount = 0;
        Source a, b, c, d;

        a.on = true;
        b.on = true;

        And andGate;
        andGate.connect(a);
        andGate.connect(b);

        Sink output;
        output.connect(andGate);

        if (!output.determine()) {
            cerr << "And gate failed to read two high inputs" << endl;
            failCount++;
        }
        else { 
            passCount++;
        }

        a.on = false;

        if (output.determine()) {
            cerr << "And gate failed to read low input" << endl;
            failCount++;
        }
        else { 
            passCount++;
        }

        a.on = true;
        b.on = true;
        c.on = true;
        d.on = true;

        And and1;
        And and2;
        Or orGate;
        Xor xorGate;

        Sink out1;
        Sink out2;

        and1.connect(a);
        and1.connect(b);
        and1.connect(c);

        and2.connect(c);
        and2.connect(d);

        orGate.connect(and1);
        orGate.connect(and2);

        xorGate.connect(and1);
        xorGate.connect(and2);

        out1.connect(xorGate);
        out2.connect(orGate);

        if (out1.determine) {
            cerr << "Xor gate failed" << endl;
            failCount++;
        }
        else { 
            passCount++;
        }

        if (!out2.determine) {
            cerr << "Or gate failed" << endl;
            failCount++;
        }
        else { 
            passCount++;
        }

        cout << passCount << " tests passed, " << failCount << " tests failed" << endl;
        return 0;
}
