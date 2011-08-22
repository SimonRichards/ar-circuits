package elecsim1;

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
        sim = new Simulation(5, 5, 2);
    }

    @After
    public void tearDown() {
    }

    @Test
    public void testOneBulb() {
        sim.getBulb(0).setLeft(sim.source);
        sim.getBulb(0).setRight(sim.sink);
        sim.run();
        assertEquals(25, sim.getBulb(0).getPower(), 0.1);
    }

    @Test
    public void testScrub() {
        testOneBulb();
        sim.scrub();
        testOneBulb();
    }

    @Test
    public void testTwoBulbs() {
        sim.getBulb(0).setLeft(sim.source);
        Node mid = sim.newNode();
        sim.getBulb(0).setRight(mid);
        sim.getBulb(1).setLeft(mid);
        sim.getBulb(1).setRight(sim.sink);

        sim.run();

        assertEquals("Left bulb", 12.5, sim.getBulb(0).getPower(), 0.1);
        assertEquals("Right bulb", 12.5, sim.getBulb(1).getPower(), 0.1);
    }
}
