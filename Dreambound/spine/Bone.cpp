#include <spine/spine-sfml.h>
#include "../Config.hpp"

namespace spine {
	Bone::Bone(spBone* bone, spSkeleton* skeleton) {
		this->bone = bone;
		this->skeleton = skeleton;
	}

	float Bone::getRotation() {
		return bone->rotation;
	}

	spBone* Bone::getBone() {
		return bone;
	}

	void Bone::setRotation(float rotation) {
		bone->rotation = rotation;
		spBone_updateWorldTransform(bone, skeleton->flipX, skeleton->flipY);
	}

	WobblyBone::WobblyBone(spBone* bone, spSkeleton* skeleton, float stiffness, float stretchFactor) : Bone(bone, skeleton) {
		this->stiffness = stiffness;
		this->stretchFactor = stretchFactor;
		defaultRotation = bone->rotation;
		rotationSpeed = 0.f;
		previousParentRotation = bone->parent->rotation;
	}

	void WobblyBone::update(sf::Vector2f force) {
		float maxRotation = 90.f * (1.f - stiffness);
		float rotation = getRotation();
		float xForceFactor = sin((bone->worldRotation) * PI / 180.f) * (1.f - stiffness);
		float yForceFactor = cos((bone->worldRotation) * PI / 180.f) * (1.f - stiffness);
		//xForceFactor = ((xForceFactor > 0.f) ? 0.2f : -0.2f) + (0.8f * xForceFactor);
		//yForceFactor = ((yForceFactor > 0.f) ? 0.2f : -0.2f) + (0.8f * yForceFactor);

		rotationSpeed += (force.x * xForceFactor + force.y * yForceFactor);
		rotationSpeed -= (bone->parent->rotation - previousParentRotation) * 3;
		rotationSpeed *= (1.f - pow(stiffness, 2));
		rotationSpeed -= (stiffness*2) * ((rotation - defaultRotation) > 0.f ? 1.f : -1.f) * pow((rotation - defaultRotation) / maxRotation, 2);

		if (stretchFactor != 0.f) {
			// not working yet
			//bone->scaleX += stretchFactor * (force.x * xForceFactor + force.y * yForceFactor);
			//bone->scaleY += stretchFactor * force.y * yForceFactor;
		}

		setRotation(fmod(rotation + rotationSpeed,360.f));

		previousParentRotation = bone->parent->rotation;
	}
}