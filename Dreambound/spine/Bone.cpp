#include <spine/spine-sfml.h>
#include "../Config.hpp"

namespace spine {
	Bone::Bone(spBone* bone, spSkeleton* skeleton) {
		this->bone = bone;
		this->skeleton = skeleton;
	}

	float Bone::getRotation() {
		return spine::getRotation(bone->rotation);
	}

	spBone* Bone::getBone() {
		return bone;
	}

	void Bone::setRotation(float rotation) {
		bone->rotation = spine::getRotation(rotation);
		spBone_updateWorldTransform(bone, skeleton->flipX, skeleton->flipY);
	}

	WobblyBone::WobblyBone(spBone* bone, spSkeleton* skeleton, float stiffness, float stretchFactor, float horAmplifier, float verAmplifier) : Bone(bone, skeleton) {
		this->stiffness = stiffness;
		defaultRotation = getRotation();
		rotationSpeed = 0.f;
		previousParentRotation = spine::getRotation(bone->parent->worldRotation);

		// Stretching
		this->stretchFactor = stretchFactor;

		this->horizontalAmplifier = horAmplifier;
		this->verticalAmplifier = verAmplifier;
	}

	void WobblyBone::update(sf::Vector2f force) {
		float maxRotation = 90.f * (1.f - stiffness);
		float rotation = getRotation();
		//float xForceFactor = (sin((bone->worldRotation) * PI / 180.f) > 0.f ? 1.f : -1.f) * (1.f - stiffness);
		//float yForceFactor = (cos((bone->worldRotation) * PI / 180.f) > 0.f ? 1.f : -1.f) * (1.f - stiffness);
		float xForceFactor = sin((bone->worldRotation) * PI / 180.f) * (1.f - stiffness) * horizontalAmplifier; // * 0.7f;
		float yForceFactor = cos((bone->worldRotation) * PI / 180.f) * (1.f - stiffness) * verticalAmplifier; // * 1.4f;

		//xForceFactor = ((xForceFactor > 0.f) ? 0.2f : -0.2f) + (0.8f * xForceFactor);
		//yForceFactor = ((yForceFactor > 0.f) ? 0.2f : -0.2f) + (0.8f * yForceFactor);

		rotationSpeed += (force.x * xForceFactor + force.y * yForceFactor);
		float parentRotationDiff = spine::getRotation(bone->parent->worldRotation - previousParentRotation);
		rotationSpeed -= parentRotationDiff * 3;
		rotationSpeed *= (1.f - pow(stiffness, 2));
		rotationSpeed -= (stiffness*2) * (spine::getRotation(rotation - defaultRotation) > 0.f ? 1.f : -1.f) * pow(spine::getRotation(rotation - defaultRotation) / maxRotation, 2);

		if (stretchFactor != 0.f) {
			// not working yet
			//bone->scaleX += stretchFactor * (force.x * xForceFactor + force.y * yForceFactor);
			//bone->scaleX -= pow(stretchFactor,2) * (rotationSpeed);
			//bone->scaleY += stretchFactor * force.y * yForceFactor;
		}

		setRotation(rotation + rotationSpeed);

		previousParentRotation = bone->parent->worldRotation;
	}
}