package io.crowbar.instrumentation.spectrum;

import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.serializers.HitSpectrumSerializer;

public final class SpectrumSerializer {
    public static String serialize (Spectrum< ? extends Activity, TrM, CmpM> spectrum) {
        int numComp = spectrum.getComponentCount();
        int numTran = spectrum.getTransactionCount();
        StringBuilder str = new StringBuilder();
        boolean first = true;


        str.append("[");

        for (int i = 0; i < numComp; i++) {
            CmpM m = spectrum.getComponent(i).getMetadata();

            if (!first)
                str.append(", ");

            str.append(i + ": (");
            str.append(m.getType().getName());
            str.append(", ");
            str.append(m.getNode().getFullName(":", 1));
            str.append(")");
            first = false;
        }

        str.append("]\n");


        str.append(numComp);
        str.append(" ");
        str.append(numTran);
        str.append("\n");

        for (Transaction< ? extends Activity, TrM> t : spectrum.byTransaction()) {
            str.append(HitSpectrumSerializer.serialize(t, numComp));
            str.append(" # exceptionClass: " + (t == null ? "null" : t.getMetadata().getExceptionClass()));
            str.append(", exceptionMessage: " + (t == null ? "null" : t.getMetadata().getExceptionMessage()));
            str.append("\n");
        }

        return str.toString();
    }
}