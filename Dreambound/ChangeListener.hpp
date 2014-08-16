#ifndef CHANGELISTENER_HPP
#define CHANGELISTENER_HPP

// Change listener interface
class ChangeListener
{
public:
	virtual ~ChangeListener() {}
	virtual void stateChanged() = 0; // every child needs to override this
};

#endif