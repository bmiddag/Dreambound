#include "SpineTest.hpp"

SpineTest::SpineTest() {
	// Load atlas, skeleton, and animations.
	atlas = Atlas_createFromFile("data/skeleton.atlas", 0);
	SkeletonJson* json = SkeletonJson_create(atlas);
	json->scale = 0.6f;
	skeletonData = SkeletonJson_readSkeletonDataFile(json, "data/skeleton.json");
	if (!skeletonData) {
		printf("%s\n", json->error);
		exit(0);
	}
	SkeletonJson_dispose(json);
	bounds = SkeletonBounds_create();

	// Configure mixing.
	stateData = AnimationStateData_create(skeletonData);
	AnimationStateData_setMixByName(stateData, "anm_idle", "anm_idle", 0.2f);
	AnimationStateData_setMixByName(stateData, "anm_run", "anm_idle", 0.2f);

	std::unique_ptr<spine::SkeletonDrawable> skeletonDrawable(new spine::SkeletonDrawable(skeletonData, stateData));
	drawable = std::move(skeletonDrawable);
	drawable->timeScale = 2;

	skeleton = drawable->skeleton;
	skeleton->flipX = false;
	skeleton->flipY = false;
	Skeleton_setToSetupPose(skeleton);

	Skeleton_updateWorldTransform(skeleton);

	AnimationState_setAnimationByName(drawable->state, 0, "anm_idle", true);
	AnimationState_addAnimationByName(drawable->state, 0, "anm_run", true, 10);

	x = 320;
	y = 460;
	skeleton->x = x;
	skeleton->y = y;
}

SpineTest::~SpineTest() {
	SkeletonData_dispose(skeletonData);
	SkeletonBounds_dispose(bounds);
	Atlas_dispose(atlas);
}

void SpineTest::step() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x -= 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x += 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) y -= 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) y += 5;
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