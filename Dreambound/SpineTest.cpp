#include "SpineTest.hpp"

SpineTest::SpineTest() {
	// Load atlas, skeleton, and animations.
	atlas = spAtlas_createFromFile("data/blue.atlas", 0);
	spSkeletonJson* json = spSkeletonJson_create(atlas);
	json->scale = 0.4f;
	skeletonData = spSkeletonJson_readSkeletonDataFile(json, "data/blue.json");
	if (!skeletonData) {
		printf("%s\n", json->error);
		exit(0);
	}
	spSkeletonJson_dispose(json);
	bounds = spSkeletonBounds_create();

	drawable = std::make_unique<spine::SkeletonDrawable>(skeletonData);
	skeleton = drawable->getSkeleton();

	// Configure animations
	animMap[Anim::Idle] = std::make_unique<spine::Animation>(spSkeletonData_findAnimation(skeletonData, "anm_idle"), true, 0.75f, 0.f, 60.f);
	animMap[Anim::Run] = std::make_unique<spine::Animation>(spSkeletonData_findAnimation(skeletonData, "anm_run"), true, 7.f/4.f, 30.f, 130.f);

	// Configure hair map
	wind = sf::Vector2f(0.f, 0.f);
	headBone = std::make_unique<spine::Bone>(spSkeleton_findBone(skeleton, "bone_head"), skeleton);
	initHair();

	x = 320;
	y = 460;
	skeleton->x = x;
	skeleton->y = y;

	moeitemeter = 0.f;
}

SpineTest::~SpineTest() {
	spSkeletonData_dispose(skeletonData);
	spSkeletonBounds_dispose(bounds);
	spAtlas_dispose(atlas);
}

void SpineTest::step() {
	// Get previous head X and Y to handle hair later
	float prevHeadX = headBone.get()->getBone()->worldX + x;
	float prevHeadY = headBone.get()->getBone()->worldY + y;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		x -= 12;
		skeleton->flipX = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		x += 12;
		skeleton->flipX = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) y -= 18;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) y += 18;

	// Wind test
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) wind.x -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) wind.x += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) wind.y -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) wind.y += 1;

	// Handle animations
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))) {
		if (drawable->getAnimation() != animMap[Anim::Run].get()) drawable->setAnimation(animMap[Anim::Run].get(), 10.f, spine::BlendType::Linear);
	} else {
		if (drawable->getAnimation() != animMap[Anim::Idle].get()) drawable->setAnimation(animMap[Anim::Idle].get(), 40.f, spine::BlendType::Linear);
	}

	skeleton->x = x;
	skeleton->y = y;

	drawable->update();

	// Get current head X and Y to handle hair movement
	float headX = headBone.get()->getBone()->worldX + x;
	float headY = headBone.get()->getBone()->worldY + y;

	// Head rotation test
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9)) {
		headBone.get()->setRotation(headBone.get()->getRotation() - moeitemeter);
		moeitemeter += 3.f;
	} else {
		moeitemeter = 0.f;
	}

	// Update hair
	updateHair(sf::Vector2f(skeleton->flipX ? -(headX - prevHeadX - wind.x) : (headX - prevHeadX - wind.x), headY - prevHeadY - wind.y));
}

void SpineTest::render(sf::RenderWindow* canvas) {
	canvas->draw(*drawable);
}

unsigned char SpineTest::getEvents() {
	return GameEvent::Step | GameEvent::Render;
}

// Initialize hair map
void SpineTest::initHair() {
	hairMap[Hair::Front1] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_1"), skeleton, 0.65f, 0.09f);
	hairMap[Hair::Front2] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_2"), skeleton, 0.55f, 0.05f);
	hairMap[Hair::Front3] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_3"), skeleton, 0.68f, 0.12f);
	hairMap[Hair::Front4] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_4"), skeleton, 0.70f, 0.2f);
	hairMap[Hair::Front5] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_5"), skeleton, 0.75f, 0.2f);
	hairMap[Hair::Front6] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_6"), skeleton, 0.7f, 0.1f);
	hairMap[Hair::Front7] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_7"), skeleton, 0.8f, 0.2f);
	hairMap[Hair::Front8] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_8"), skeleton, 0.75f, 0.15f);
	hairMap[Hair::Front9] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_9"), skeleton, 0.65f, 0.08f);
	hairMap[Hair::Front10] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_10"), skeleton, 0.75f, 0.1f);
	hairMap[Hair::Front11] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_11"), skeleton, 0.75f, 0.2f);

	hairMap[Hair::Back1] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_back_1"), skeleton, 0.8f, 0.05f);
	hairMap[Hair::Back2] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_back_2"), skeleton, 0.8f, 0.08f);
	hairMap[Hair::Spike] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_spike"), skeleton, 0.75f, 0.05f);

	hairMap[Hair::PonytailTie] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_tie"), skeleton, 0.85f, 0.0f);
	hairMap[Hair::PonytailBase] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_base"), skeleton, 0.8f, 0.0f);
	hairMap[Hair::Ponytail1] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_1"), skeleton, 0.78f, 0.2f);
	hairMap[Hair::Ponytail2] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_2"), skeleton, 0.70f, 0.15f);
	hairMap[Hair::Ponytail3] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_3"), skeleton, 0.70f, 0.22f);
	hairMap[Hair::Ponytail4] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_4"), skeleton, 0.60f, 0.17f);
	hairMap[Hair::Ponytail5] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_5"), skeleton, 0.50f, 0.15f);
	hairMap[Hair::Ponytail6] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_6"), skeleton, 0.35f, 0.12f);
	hairMap[Hair::Ponytail7] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_7"), skeleton, 0.55f, 0.17f);
	hairMap[Hair::Ponytail8] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_8"), skeleton, 0.80f, 0.19f);
	hairMap[Hair::Ponytail9] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_9"), skeleton, 0.77f, 0.16f);
	hairMap[Hair::Ponytail10] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_10"), skeleton, 0.60f, 0.17f);
	hairMap[Hair::Ponytail11] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_11"), skeleton, 0.82f, 0.12f);
	hairMap[Hair::Ponytail12] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_12"), skeleton, 0.60f, 0.15f);
	hairMap[Hair::Ponytail13] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_13"), skeleton, 0.70f, 0.13f);
	hairMap[Hair::Ponytail14] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_14"), skeleton, 0.50f, 0.19f);
	hairMap[Hair::Ponytail15] = std::make_unique<spine::WobblyBone>(spSkeleton_findBone(skeleton, "bone_hair_ponytail_15"), skeleton, 0.80f, 0.11f);
}

void SpineTest::updateHair(sf::Vector2f force) {
	for (auto it = hairMap.begin(); it != hairMap.end(); ++it) {
		it->second->update(force);
	}
}