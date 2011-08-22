package elecsim2;

public class Simulation {

    private Source[] sources;
    private Sink[] sinks;
    private And[] ands;
    private Or[] ors;
    private Xor[] xors;

    public Simulation(int numSources, int numSinks, int numOr, int numAnd, int numXor) {
        sources = new Source[numSources];
        sinks = new Sink[numSinks];
        ands = new And[numAnd];
        ors = new Or[numOr];
        xors = new Xor[numXor];
        initArray(sources, Source.class, numSources);
        initArray(sinks, Sink.class, numSinks);
        initArray(ands, And.class, numAnd); // Do this shit with a map <String, Collection>
        initArray(ors, Or.class, numOr);
        initArray(xors, Xor.class, numXor);
    }

    private <T> void initArray(T[] array, Class<T> type, int num) {
        for (int i = 0; i < num; i++) {
            T temp;
            try {
                temp = type.newInstance();
            } catch (ReflectiveOperationException e) {
                throw new RuntimeException("Components need no-arg constructors");
            }
            array[i] = temp;
        }
    }

    public Source getSource(int id) {
        return sources[id];
    }

    public Sink getSink(int id) {
        return sinks[id];
    }

    public Gate getGate(String type, int id) {
        switch(type) {
            case "and":
                return ands[id];
            case "or":
                return ors[id];
            case "xor":
                return xors[id];
            default:
                throw new RuntimeException("invalid type");
        }
    }
}
