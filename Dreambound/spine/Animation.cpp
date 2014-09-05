#include <spine/spine-sfml.h>

namespace spine {
	Animation::Animation(spAnimation* animation, bool loop, float timeScale, float startFrame, float stopFrame) {
		this->animation = animation;
		this->loop = loop;
		this->timeScale = timeScale;
		this->startFrame = startFrame;
		this->stopFrame = stopFrame;
		
		nextAnimation = NULL;
		blendFactor = 1.f;
		blendFrames = 0.f;
		blendType = BlendType::Linear;
	}

	void Animation::setNextAnimation(Animation* nextAnimation, float blendFrames, BlendType blendType, float blendFactor) {
		this->nextAnimation = nextAnimation;
		this->blendFrames = blendFrames;
		this->blendType = blendType;
		this->blendFactor = blendFactor;
	}

	// Current animation getters
	spAnimation* Animation::getAnimation() {
		return animation;
	}

	bool Animation::isLoop() {
		return loop;
	}

	float Animation::getTimeScale() {
		return timeScale;
	}

	float Animation::getStartFrame() {
		return startFrame;
	}

	float Animation::getStopFrame() {
		return stopFrame;
	}

	// Next animation getters
	Animation* Animation::getNextAnimation() {
		return nextAnimation;
	}

	float Animation::getBlendFactor() {
		return blendFactor;
	}

	float Animation::getBlendFrames() {
		return blendFrames;
	}

	BlendType Animation::getBlendType() {
		return blendType;
	}
}