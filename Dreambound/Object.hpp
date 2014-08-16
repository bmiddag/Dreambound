#ifndef OBJECT_HPP
#define OBJECT_HPP

// Game object abstract class
class Object {
public:
	Object() {};
	virtual ~Object() {};
	virtual void beginStep() {};
	virtual void step() {};
	virtual void endStep() {};
	virtual void render() {};
};

#endif