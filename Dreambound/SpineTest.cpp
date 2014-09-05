#include "SpineTest.hpp"
#include <iostream>

SpineTest::SpineTest() {
	// Load atlas, skeleton, and animations.
	atlas = spAtlas_createFromFile("data/skeleton.atlas", 0);
	spSkeletonJson* json = spSkeletonJson_create(atlas);
	json->scale = 0.4f;
	skeletonData = spSkeletonJson_readSkeletonDataFile(json, "data/skeleton.json");
	if (!skeletonData) {
		printf("%s\n", json->error);
		exit(0);
	}
	spSkeletonJson_dispose(json);
	bounds = spSkeletonBounds_create();

	drawable = std::make_unique<spine::SkeletonDrawable>(skeletonData);
	skeleton = drawable->getSkeleton();

	//Configure animations
	AnimMap[Anim::Idle] = std::make_unique<spine::Animation>(spSkeletonData_findAnimation(skeletonData, "anm_idle"), true, 1.f, 0.f, 60.f);
	AnimMap[Anim::Run] = std::make_unique<spine::Animation>(spSkeletonData_findAnimation(skeletonData, "anm_run"), true, 7.f/4.f, 30.f, 130.f);

	x = 320;
	y = 460;
	skeleton->x = x;
	skeleton->y = y;
}

SpineTest::~SpineTest() {
	spSkeletonData_dispose(skeletonData);
	spSkeletonBounds_dispose(bounds);
	spAtlas_dispose(atlas);
}

void SpineTest::step() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		x -= 12;
		skeleton->flipX = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		x += 12;
		skeleton->flipX = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) y -= 8;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) y += 8;

	// Handle animations
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))) {
		if (drawable->getAnimation() != AnimMap[Anim::Run].get()) drawable->setAnimation(AnimMap[Anim::Run].get(), 10.f, spine::BlendType::Linear);
	} else {
		if (drawable->getAnimation() != AnimMap[Anim::Idle].get()) drawable->setAnimation(AnimMap[Anim::Idle].get(), 40.f, spine::BlendType::Linear);
	}

	skeleton->x = x;
	skeleton->y = y;
	drawable->update();
}

void SpineTest::render(sf::RenderWindow* canvas) {
	canvas->draw(*drawable);
}

unsigned char SpineTest::getEvents() {
	return GameEvent::Step | GameEvent::Render;
}