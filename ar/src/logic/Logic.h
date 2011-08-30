#pragma once
#include <vector>

class Gate {
    protected:
        std::vector<Gate*> _inputs;

    public:
        void scrub();

        void connect(Gate* gate);

        virtual bool determine() = 0;
};

class And : public Gate {

     virtual bool determine();
};

class Xor : public Gate {
   virtual bool determine();
};

class Or : public Gate {
public:
	virtual bool determine();
};

class Sink {
private:
	Gate* _input;

public:
	//Sink();
	virtual bool determine();
	void connect(Gate* input);
};

class Source : public Gate{
    private:

    public:
        bool on;

        void connect(Gate* gate);

        void scrub();

        virtual bool determine();
};
