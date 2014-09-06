#ifndef SPINETEST_HPP
#define SPINETEST_HPP

#include "Object.hpp"
#include <spine/spine-sfml.h>
#include <memory>

class SpineTest : public Object {
private:
	enum Anim {
		Idle,
		Run
	};

	std::map<Anim, std::unique_ptr<spine::Animation>> animMap;

	enum Hair {
		Front1, Front2, Front3, Front4, Front5, Front6, Front7, Front8, Front9, Front10, Front11, Back1, Back2, Spike,
		PonytailTie, PonytailBase, Ponytail1, Ponytail2, Ponytail3, Ponytail4, Ponytail5, Ponytail6, Ponytail7,
		Ponytail8, Ponytail9, Ponytail10, Ponytail11, Ponytail12, Ponytail13, Ponytail14, Ponytail15
	};

	std::map<Hair, std::unique_ptr<spine::WobblyBone>> hairMap;
	std::unique_ptr<spine::Bone> headBone;

	sf::Vector2f wind;
	
	spAtlas* atlas;
	spSkeletonBounds* bounds;
	spSkeletonData* skeletonData;
	std::unique_ptr<spine::SkeletonDrawable> drawable;
	spSkeleton* skeleton;

	int x;
	int y;
public:
	SpineTest();
	~SpineTest();
	void initHair();
	void updateHair(sf::Vector2f force);
	virtual void step();
	virtual void render(sf::RenderWindow* canvas);
	virtual unsigned char getEvents();
};
#endif