package io.crowbar.instrumentation.runtime;

import static org.junit.Assert.*;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.events.AbstractEventListener;
import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.events.EventListenerMocks.EventListenerChecks;
import io.crowbar.instrumentation.events.EventListenerMocks.OracleListener;
import io.crowbar.instrumentation.events.EventListenerMocks.TestableRegisterNodeListener;
import io.crowbar.instrumentation.events.EventListenerMocks.TestableRegisterProbeListener;
import io.crowbar.instrumentation.events.EventListenerMocks.TestableTransactionEndListener;
import io.crowbar.instrumentation.events.EventListenerMocks.TestableTransactionStartListener;

import java.lang.reflect.Constructor;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class CollectorTest {
    private EventListenerChecks checks;

    public Collector newCollectorInstance (String name, EventListener listener) throws Exception {
        // We need to be able to create different instances of collector
        Class< ? >[] argTypes = {String.class, EventListener.class};
        Constructor<Collector> c = Collector.class.getDeclaredConstructor(argTypes);

        // this may not be possible in some JVMs, or in JVMs with less security permissions:
        c.setAccessible(true);

        Collector collector = c.newInstance(name, listener);
        return collector;
    }

    @Before
    public void setup () {
        checks = new EventListenerChecks();
    }

    @Test
    public void collectorConstructionTest () throws Exception {
        String collectorName = "Collector1";
        Collector collector = newCollectorInstance(collectorName, null);


        assertEquals(collectorName, collector.getRootNode().getName());

        collectorName = "Collector2";
        collector = newCollectorInstance(collectorName, null);
        assertEquals(collectorName, collector.getRootNode().getName());
    }

    @Test
    public void registerNodeEventTest () throws Exception {
        TestableRegisterNodeListener trnl = new TestableRegisterNodeListener(checks);
        Collector collector = newCollectorInstance("collector", trnl);


        assertEquals(collector.getRootNode().getTree().size(), 1);

        int id = collector.getRootNode().getId();
        String nodeName = "nodeName";
        checks.nodeName = nodeName;
        checks.parentNodeId = id;

        collector.registerNode(nodeName, id);
        assertTrue(trnl.wasTriggered());
        assertEquals(collector.getRootNode().getTree().size(), 2);
    }

    @Test
    public void registerProbeEventTest () throws Exception {
        TestableRegisterProbeListener trpl = new TestableRegisterProbeListener(checks);
        Collector collector = newCollectorInstance("collector", trpl);

        int id = collector.getRootNode().getId();
        ProbeType type = ProbeType.HIT_PROBE;


        checks.probeNodeId = id;
        checks.probeType = type;

        collector.registerProbe("groupName", id, type);
        assertTrue(trpl.wasTriggered());
    }

    @Test
    public void transactionStartEventTest () throws Exception {
        TestableTransactionStartListener ttsl = new TestableTransactionStartListener(checks);
        Collector collector = newCollectorInstance("collector", ttsl);

        int id = collector.getRootNode().getId();


        checks.startTransactionId = id;

        collector.transactionStart(id);
        assertTrue(ttsl.wasTriggered());
    }

    @Test
    public void transactionEndEventTest () throws Exception {
        TestableTransactionEndListener ttel = new TestableTransactionEndListener(checks);
        Collector collector = newCollectorInstance("collector", ttel);

        int id = collector.getRootNode().getId();


        checks.endTransactionId = id;
        checks.endTransactionExceptionClass = null;
        checks.endTransactionExceptionMessage = null;

        collector.transactionEnd(id);
        assertTrue(ttel.wasTriggered());
    }

    @Test
    public void transactionEndWithExceptionEventTest () throws Exception {
        TestableTransactionEndListener ttel = new TestableTransactionEndListener(checks);
        Collector collector = newCollectorInstance("collector", ttel);

        int id = collector.getRootNode().getId();
        String exceptionMessage = "exception message";


        checks.endTransactionId = id;
        checks.endTransactionExceptionClass = "java.lang.Exception";
        checks.endTransactionExceptionMessage = exceptionMessage;

        collector.transactionEnd(id, new Exception(exceptionMessage));
        assertTrue(ttel.wasTriggered());
    }

    @Test
    public void oracleEventTest () throws Exception {
        OracleListener ol = new OracleListener(checks);
        Collector collector = newCollectorInstance("collector", ol);

        int id = collector.getRootNode().getId();
        double error = 0.12345;
        double confidence = 0.98765;


        checks.oracleId = id;
        checks.oracleError = error;
        checks.oracleConfidence = confidence;

        collector.oracle(id, error, confidence);
        assertTrue(ol.wasTriggered());
    }

    private void fillWithHitProbes (Collector collector) {
        collector.registerProbe("group1", 1, ProbeType.HIT_PROBE);
        collector.registerProbe("group1", 1, ProbeType.HIT_PROBE);
        collector.registerProbe("group1", 1, ProbeType.HIT_PROBE);
        collector.registerProbe("group1", 1, ProbeType.HIT_PROBE);

        collector.registerProbe("group2", 1, ProbeType.HIT_PROBE);
        collector.registerProbe("group2", 1, ProbeType.HIT_PROBE);
        collector.registerProbe("group2", 1, ProbeType.HIT_PROBE);

        collector.registerProbe("group3", 1, ProbeType.HIT_PROBE);
    }

    @Test
    public void hitVectorSizeTest () throws Exception {
        Collector collector = newCollectorInstance("collector", new AbstractEventListener() {});


        fillWithHitProbes(collector);

        assertEquals(collector.getHitVector("group1").length, 4);
        assertEquals(collector.getHitVector("group2").length, 3);
        assertEquals(collector.getHitVector("group3").length, 1);
    }

    @Test
    public void testNotNullCollector () {
        Collector.start("", null);
        Assert.assertNotNull(Collector.instance());
    }

    @Test
    public void testOracle () {
        Collector.start("", null);
        Collector c = Collector.instance();
        c.oracle(0, 0.0, 0.0);
        // FIXME: catch NullPointerException
    }

    @Test(expected = NullPointerException.class)
    public void testGetHitVectorEmptyCollector () {
        Collector.start("", null);
        Collector c = Collector.instance();
        c.getHitVector("");
    }

    @Test(expected = IndexOutOfBoundsException.class)
    public void testHitEmptyCollector () {
        Collector.start("", null);
        Collector c = Collector.instance();
        c.hit(-1);
    }
}