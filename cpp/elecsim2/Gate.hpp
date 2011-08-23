virtual class Gate {
    protected:
        vector<Gate> inputs;

    public:
        void scrub();

        void connect(Gate gate);

        virtual boolean determine();
};
