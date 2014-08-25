package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;

import org.junit.Test;

public class SpectrumBuilderTest {
    @Test
    public void testEmptySpectrum () {
        SpectrumBuilder sb = new SpectrumBuilder();


        Spectrum<Hit, TrM> spectrum = sb.getSpectrum();
        assertEquals(0, spectrum.getComponentCount());
        assertEquals(0, spectrum.getTransactionCount());
    }

    @Test
    public void testRegisterNode () {
        // TODO
        // fail("to implement");
    }

    //    @Test(expected = Exception.class)
    public void testRegisterProbe () throws Exception {
        //SpectrumBuilder sb = new SpectrumBuilder();
        //sb.registerProbe(0,0,null);
    }

    @Test
    public void testEndTransaction () {
        // TODO
        //fail("to implement");
    }

    @Test
    public void testOracle () {
        // TODO
        // create a SpectrumBuilder with one transaction and three components
        // create a probe
        // set the oracle
        // fail("to implement");
    }

    @Test
    public void testToString () {
        SpectrumBuilder sb = new SpectrumBuilder();


        Spectrum<Hit, TrM> spectrum = sb.getSpectrum();
        assertEquals("{class='Spectrum', components=[], transactions=[]}", spectrum.toString());
    }
}
