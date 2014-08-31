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

	// Configure mixing.
	stateData = spAnimationStateData_create(skeletonData);
	spAnimationStateData_setMixByName(stateData, "anm_idle", "anm_idle", 0.2f);
	spAnimationStateData_setMixByName(stateData, "anm_run", "anm_idle", 0.5f);

	std::unique_ptr<spine::SkeletonDrawable> skeletonDrawable(new spine::SkeletonDrawable(skeletonData, stateData));
	drawable = std::move(skeletonDrawable);
	drawable->timeScale = 2;

	skeleton = drawable->skeleton;
	skeleton->flipX = false;
	skeleton->flipY = false;
	spSkeleton_setToSetupPose(skeleton);

	spSkeleton_updateWorldTransform(skeleton);

	spAnimationState_setAnimationByName(drawable->state, 0, "anm_idle", true);

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
	TrackEntry* entry = spAnimationState_getCurrent(drawable->state, 0);
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))) {
		if (strcmp("anm_run", entry->animation->name) != 0) spAnimationState_setAnimationByName(drawable->state, 0, "anm_run", true);
		drawable->timeScale = 3.5;
	} else {
		if (strcmp("anm_idle", entry->animation->name) != 0) spAnimationState_setAnimationByName(drawable->state, 0, "anm_idle", true);
		drawable->timeScale = 2;
	}

	skeleton->x = x;
	skeleton->y = y;
	drawable->update(1.f / 60.f);
}

void SpineTest::render(sf::RenderWindow* canvas) {
	canvas->draw(*drawable);
}

unsigned char SpineTest::getEvents() {
	return GameEvent::Step | GameEvent::Render;
}