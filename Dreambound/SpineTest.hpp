#ifndef SPINETEST_HPP
#define SPINETEST_HPP

#include "Object.hpp"
#include <spine/spine-sfml.h>
#include <memory>

class SpineTest : public Object {
private:
	
	spAtlas* atlas;
	spSkeletonBounds* bounds;
	spSkeletonData* skeletonData;
	spAnimationStateData* stateData;
	std::unique_ptr<spine::SkeletonDrawable> drawable;
	spSkeleton* skeleton;

	int x;
	int y;
public:
	SpineTest();
	~SpineTest();
	virtual void step();
	virtual void render(sf::RenderWindow* canvas);
	virtual unsigned char getEvents();
};
#endif