/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.1
 * 
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to install, execute and perform the Spine Runtimes
 * Software (the "Software") solely for internal use. Without the written
 * permission of Esoteric Software (typically granted by licensing Spine), you
 * may not (a) modify, translate, adapt or otherwise create derivative works,
 * improvements of the Software or develop new applications using the Software
 * or (b) remove, delete, alter or obscure any trademarks or any copyright,
 * trademark, patent or other intellectual property or proprietary rights
 * notices on or in the Software, including any copy thereof. Redistributions
 * in binary or source form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef SPINE_SFML_H_
#define SPINE_SFML_H_

#include <memory>
#include <spine/spine.h>
#include <spine/extension.h>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace spine {

	typedef enum BlendType {
		Constant,
		Linear,
		SlowStart,
		SlowEnd,
		Smooth
	} BlendType;

	class Bone {
	protected:
		spBone* bone;
		spSkeleton* skeleton;

	public:
		Bone(spBone* bone, spSkeleton* skeleton);
		float getRotation();
		spBone* getBone();
		void setRotation(float rotation);
	};

	class WobblyBone: public Bone {
	private:
		float defaultRotation;
		float stiffness;
		float rotationSpeed;
		float stretchFactor;
		float previousParentRotation;

	public:
		WobblyBone(spBone* bone, spSkeleton* skeleton, float stiffness = 0.5f, float stretchFactor = 0.f);
		void update(sf::Vector2f force);
	};

	class Animation {
	private:
		spAnimation* animation;
		bool loop;
		float timeScale; // Time scale factor for this animation (Total FPS = skeleton FPS * anim time scale)
		float startFrame;
		float stopFrame;

		// Next animation fields
		Animation* nextAnimation;
		float blendFactor;
		float blendFrames;
		BlendType blendType;

	public:
		Animation(spAnimation* animation, bool loop, float timeScale, float startFrame, float stopFrame);
		void setNextAnimation(Animation* nextAnimation, float blendFrames = 0.f, BlendType blendType = BlendType::Linear, float blendFactor = 0.f);

		// Current animation getters
		spAnimation* getAnimation();
		bool isLoop();
		float getTimeScale();
		float getStartFrame();
		float getStopFrame();

		// Next animation getters
		Animation* getNextAnimation();
		float getBlendFactor();
		float getBlendFrames();
		BlendType getBlendType();
	};
	
	class SkeletonDrawable: public sf::Drawable {
	private:
		spSkeleton* skeleton;
		Animation* animation;
		float fps; // Skeleton FPS (Total FPS = skeleton FPS * anim time scale)
		float lastFrame; // Not useful yet but will be when events are implemented
		float currentFrame; // Float because 0.5 frames timestep is allowed

		float blendFactor;
		float blendFrames;
		float currentBlendFrame;
		BlendType blendType;

		std::unique_ptr<sf::VertexArray> vertexArray;
		float* worldVertices;

	public:
		SkeletonDrawable(spSkeletonData* skeleton, float fps = 30.f);
		~SkeletonDrawable();
		
		void SkeletonDrawable::setAnimation(Animation* animation, bool force = false);
		void SkeletonDrawable::setAnimation(Animation* animation, float blendFrames, BlendType blendType = BlendType::Linear, float blendFactor = 0.f, bool force = false);
		
		void update(float frames = 1.f);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// Spine animation getters & setters
		Animation* getAnimation();
		float getFPS();
		void setFPS(float fps);
		spSkeleton* getSkeleton();

		// Blending getters
		float getBlendFactor();
		float getBlendFrames();
		BlendType getBlendType();
	};

} // namespace spine
#endif // SPINE_SFML_H_
