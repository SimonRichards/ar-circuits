package elecsim2;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

public class SimulationTest {
    Simulation sim;

    @Before
    public void setUp() {
        sim = new Simulation(5, 5, 10, 10, 10);
    }

    @After
    public void tearDown() {
    }

    @Test
    public void testAndOn() {
        Source a = sim.getSource(0);
        Source b = sim.getSource(1);
        a.on = true;
        b.on = true;

        Gate and = sim.getGate("and", 0);
        and.connect(a);
        and.connect(b);

        Sink output = sim.getSink(0);
        output.connect(and);

        assertTrue("And gate", output.determine());
    }

    @Test
    public void testAndOff() {
        Source a = sim.getSource(0);
        Source b = sim.getSource(1);
        a.on = false;
        b.on = true;

        Gate and = sim.getGate("and", 0);
        and.connect(a);
        and.connect(b);

        Sink output = sim.getSink(0);
        output.connect(and);

        assertFalse("And gate", output.determine());
    }

    @Test
    public void testTwo() {
        testAndOn();
        testAndOff();
    }

    @Test
    public void testComplex() {
        Source a = sim.getSource(0);
        Source b = sim.getSource(1);
        Source c = sim.getSource(2);
        Source d = sim.getSource(3);
        a.on = true;
        b.on = true;
        c.on = true;
        d.on = true;

        Gate and1 = sim.getGate("and", 0);
        Gate and2 = sim.getGate("and", 1);
        Gate or = sim.getGate("or", 0);
        Gate xor = sim.getGate("xor", 0);

        Sink out1 = sim.getSink(0);
        Sink out2 = sim.getSink(1);

        and1.connect(a);
        and1.connect(b);
        and1.connect(c);

        and2.connect(c);
        and2.connect(d);

        or.connect(and1);
        or.connect(and2);

        xor.connect(and1);
        xor.connect(and2);

        out1.connect(xor);
        out2.connect(or);

        assertFalse("Xor gate", out1.determine());
        assertTrue("Or gate", out2.determine());
    }
}
