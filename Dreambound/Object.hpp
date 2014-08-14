#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object {
public:
	Object();
	~Object();
	void beginStep();
	void step();
	void endStep();
	void render();
};

#endif