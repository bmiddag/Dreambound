#include <spine/spine-sfml.h>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
//#include <iostream>

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

using namespace sf;

namespace spine {
	SkeletonDrawable::SkeletonDrawable(spSkeletonData* skeletonData, float fps) :
		fps(fps),
		vertexArray(new VertexArray(Triangles, skeletonData->boneCount * 4)),
		worldVertices(0) {
		spBone_setYDown(true);
		worldVertices = MALLOC(float, SPINE_MESH_VERTEX_COUNT_MAX);
		skeleton = spSkeleton_create(skeletonData);
		spSkeleton_setToSetupPose(skeleton);
		spSkeleton_updateWorldTransform(skeleton);
		skeleton->flipX = false;
		skeleton->flipY = false;
		//state = spAnimationState_create(stateData);

		animation = NULL;
		lastFrame = 0.f;
		currentFrame = 0.f;
		blendFactor = 0.f;
		blendFrames = 0.f;
		currentBlendFrame = 0.f;
		blendType = BlendType::Linear;
	}

	SkeletonDrawable::~SkeletonDrawable() {
		FREE(worldVertices);
		spSkeleton_dispose(skeleton);
	}

	void SkeletonDrawable::correctBonesRotation() {
		for (int i = 0; i < skeleton->boneCount; i++) {
			skeleton->bones[i]->rotation = spine::getRotation(skeleton->bones[i]->rotation);
			spBone_updateWorldTransform(skeleton->bones[i], skeleton->flipX, skeleton->flipY);
		}
	}

	void SkeletonDrawable::setAnimation(Animation* animation, bool force) {
		if (force || (this->animation != animation && animation != NULL)) {
			this->animation = animation;
			lastFrame = 0.f;
			currentFrame = 0.f;
			blendFactor = 0.f;
			blendFrames = 0.f;
			currentBlendFrame = 0.f;
			if (animation != NULL) {
				spAnimation_apply(animation->getAnimation(), skeleton, lastFrame, currentFrame, animation->isLoop(), NULL, NULL);
				correctBonesRotation();
			}
		}
	}

	void SkeletonDrawable::setAnimation(Animation* animation, float blendFrames, BlendType blendType, float blendFactor, bool force) {
		if (force || (this->animation != animation && animation != NULL)) {
			this->animation = animation;
			this->blendFrames = blendFrames;
			this->blendType = blendType;
			if (blendType == spine::BlendType::Constant) {
				this->blendFactor = blendFactor;
			} else this->blendFactor = 0.f;
			lastFrame = 0.f;
			currentFrame = 0.f;
			currentBlendFrame = 0.f;
		}
	}

	// Spine animation getters
	Animation* SkeletonDrawable::getAnimation() {
		return animation;
	}

	float SkeletonDrawable::getFPS() {
		return fps;
	}

	void SkeletonDrawable::setFPS(float fps) {
		this->fps = fps;
	}

	spSkeleton* SkeletonDrawable::getSkeleton() {
		return skeleton;
	}

	// Blending getters
	float SkeletonDrawable::getBlendFactor() {
		return blendFactor;
	}

	float SkeletonDrawable::getBlendFrames() {
		return blendFrames;
	}

	BlendType SkeletonDrawable::getBlendType() {
		return blendType;
	}

	// Step function for a Spine skeleton
	void SkeletonDrawable::update(float frames) {
		spSkeleton_update(skeleton, (fps * frames / 30.f) / 30.f);

		// Skeleton pose updating begins here
		if (animation != NULL) {

			if (animation->isLoop() && ((currentFrame) * animation->getTimeScale()) >= animation->getStopFrame()) {
				//std::cout << "Current frame: " << currentFrame << "\n";
				//std::cout << "Blend factor: " << blendFactor << "\n";
				//std::cout << "Blend frames: " << blendFrames << "\n";
				lastFrame -= (animation->getStopFrame() - animation->getStartFrame()) / animation->getTimeScale();
				currentFrame -= (animation->getStopFrame() - animation->getStartFrame()) / animation->getTimeScale();
				if (animation->getNextAnimation() != NULL) {
					if (animation->getBlendFrames() > 0.f) {
						setAnimation(animation->getNextAnimation(), animation->getBlendFrames(), animation->getBlendType(), animation->getBlendFactor());
					} else {
						setAnimation(animation->getNextAnimation());
					}
				}
			}
			if (currentBlendFrame < blendFrames) {
				//std::cout << "Blend factor: " << blendFactor << "\n";
				spAnimation_mix(animation->getAnimation(), skeleton, (lastFrame * animation->getTimeScale()) / 30.f, (currentFrame * animation->getTimeScale()) / 30.f, animation->isLoop(), NULL, NULL, blendFactor);
				if (blendType == BlendType::Linear) {
					blendFactor = currentBlendFrame / blendFrames;
				} else if (blendType == BlendType::Smooth) {
					if ((currentBlendFrame / blendFrames) <= 0.5f) {
						blendFactor = pow((2.f*currentBlendFrame) / blendFrames, 2) / 2.f;
					} else {
						blendFactor = 1.f - pow((2.f*(blendFrames - currentBlendFrame) / blendFrames), 2) / 2.f;
					}
				} else if (blendType == BlendType::SlowStart) {
					blendFactor = pow(currentBlendFrame / blendFrames, 2);
				} else if (blendType == BlendType::SlowEnd) {
					blendFactor = 1.f - pow(((blendFrames - currentBlendFrame) / blendFrames), 2);
				}
				currentBlendFrame += fps * frames / 30.f;
			} else {
				blendFrames = 0.f;
				spAnimation_apply(animation->getAnimation(), skeleton, (lastFrame * animation->getTimeScale()) / 30.f, (currentFrame * animation->getTimeScale()) / 30.f, animation->isLoop(), NULL, NULL);
				correctBonesRotation();
			}

			lastFrame = currentFrame;
			currentFrame += fps * frames / 30.f;
		}

		// Skeleton pose updating ends here
		spSkeleton_updateWorldTransform(skeleton);
	}

	void SkeletonDrawable::draw(RenderTarget& target, RenderStates states) const {
		vertexArray->clear();
		states.blendMode = BlendAlpha;

		sf::Vertex vertices[4];
		sf::Vertex vertex;
		for (int i = 0; i < skeleton->slotCount; ++i) {
			spSlot* slot = skeleton->drawOrder[i];
			spAttachment* attachment = slot->attachment;
			if (!attachment) continue;
			Texture* texture = 0;
			if (attachment->type == SP_ATTACHMENT_REGION) {
				spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
				texture = (Texture*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
				spRegionAttachment_computeWorldVertices(regionAttachment, slot->skeleton->x, slot->skeleton->y, slot->bone, worldVertices);

				Uint8 r = static_cast<Uint8>(skeleton->r * slot->r * 255);
				Uint8 g = static_cast<Uint8>(skeleton->g * slot->g * 255);
				Uint8 b = static_cast<Uint8>(skeleton->b * slot->b * 255);
				Uint8 a = static_cast<Uint8>(skeleton->a * slot->a * 255);

				Vector2u size = texture->getSize();
				vertices[0].color.r = r;
				vertices[0].color.g = g;
				vertices[0].color.b = b;
				vertices[0].color.a = a;
				vertices[0].position.x = worldVertices[SP_VERTEX_X1];
				vertices[0].position.y = worldVertices[SP_VERTEX_Y1];
				vertices[0].texCoords.x = regionAttachment->uvs[SP_VERTEX_X1] * size.x;
				vertices[0].texCoords.y = regionAttachment->uvs[SP_VERTEX_Y1] * size.y;

				vertices[1].color.r = r;
				vertices[1].color.g = g;
				vertices[1].color.b = b;
				vertices[1].color.a = a;
				vertices[1].position.x = worldVertices[SP_VERTEX_X2];
				vertices[1].position.y = worldVertices[SP_VERTEX_Y2];
				vertices[1].texCoords.x = regionAttachment->uvs[SP_VERTEX_X2] * size.x;
				vertices[1].texCoords.y = regionAttachment->uvs[SP_VERTEX_Y2] * size.y;

				vertices[2].color.r = r;
				vertices[2].color.g = g;
				vertices[2].color.b = b;
				vertices[2].color.a = a;
				vertices[2].position.x = worldVertices[SP_VERTEX_X3];
				vertices[2].position.y = worldVertices[SP_VERTEX_Y3];
				vertices[2].texCoords.x = regionAttachment->uvs[SP_VERTEX_X3] * size.x;
				vertices[2].texCoords.y = regionAttachment->uvs[SP_VERTEX_Y3] * size.y;

				vertices[3].color.r = r;
				vertices[3].color.g = g;
				vertices[3].color.b = b;
				vertices[3].color.a = a;
				vertices[3].position.x = worldVertices[SP_VERTEX_X4];
				vertices[3].position.y = worldVertices[SP_VERTEX_Y4];
				vertices[3].texCoords.x = regionAttachment->uvs[SP_VERTEX_X4] * size.x;
				vertices[3].texCoords.y = regionAttachment->uvs[SP_VERTEX_Y4] * size.y;

				vertexArray->append(vertices[0]);
				vertexArray->append(vertices[1]);
				vertexArray->append(vertices[2]);
				vertexArray->append(vertices[0]);
				vertexArray->append(vertices[2]);
				vertexArray->append(vertices[3]);

			} else if (attachment->type == SP_ATTACHMENT_MESH) {
				spMeshAttachment* mesh = (spMeshAttachment*)attachment;
				if (mesh->verticesCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
				texture = (Texture*)((spAtlasRegion*)mesh->rendererObject)->page->rendererObject;
				spMeshAttachment_computeWorldVertices(mesh, slot->skeleton->x, slot->skeleton->y, slot, worldVertices);

				Uint8 r = static_cast<Uint8>(skeleton->r * slot->r * 255);
				Uint8 g = static_cast<Uint8>(skeleton->g * slot->g * 255);
				Uint8 b = static_cast<Uint8>(skeleton->b * slot->b * 255);
				Uint8 a = static_cast<Uint8>(skeleton->a * slot->a * 255);
				vertex.color.r = r;
				vertex.color.g = g;
				vertex.color.b = b;
				vertex.color.a = a;

				Vector2u size = texture->getSize();
				for (int i = 0; i < mesh->trianglesCount; ++i) {
					int index = mesh->triangles[i] << 1;
					vertex.position.x = worldVertices[index];
					vertex.position.y = worldVertices[index + 1];
					vertex.texCoords.x = mesh->uvs[index] * size.x;
					vertex.texCoords.y = mesh->uvs[index + 1] * size.y;
					vertexArray->append(vertex);
				}

			} else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH) {
				spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)attachment;
				if (mesh->uvsCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
				texture = (Texture*)((spAtlasRegion*)mesh->rendererObject)->page->rendererObject;
				spSkinnedMeshAttachment_computeWorldVertices(mesh, slot->skeleton->x, slot->skeleton->y, slot, worldVertices);

				Uint8 r = static_cast<Uint8>(skeleton->r * slot->r * 255);
				Uint8 g = static_cast<Uint8>(skeleton->g * slot->g * 255);
				Uint8 b = static_cast<Uint8>(skeleton->b * slot->b * 255);
				Uint8 a = static_cast<Uint8>(skeleton->a * slot->a * 255);
				vertex.color.r = r;
				vertex.color.g = g;
				vertex.color.b = b;
				vertex.color.a = a;

				Vector2u size = texture->getSize();
				for (int i = 0; i < mesh->trianglesCount; ++i) {
					int index = mesh->triangles[i] << 1;
					vertex.position.x = worldVertices[index];
					vertex.position.y = worldVertices[index + 1];
					vertex.texCoords.x = mesh->uvs[index] * size.x;
					vertex.texCoords.y = mesh->uvs[index + 1] * size.y;
					vertexArray->append(vertex);
				}
			}

			if (texture) {
				// SMFL doesn't handle batching for us, so we'll just force a single texture per skeleton.
				states.texture = texture;

				BlendMode blend = slot->data->additiveBlending ? BlendAdd : BlendAlpha;
				if (states.blendMode != blend) {
					target.draw(*vertexArray, states);
					vertexArray->clear();
					states.blendMode = blend;
				}
			}
		}
		target.draw(*vertexArray, states);
	}

} /* namespace spine */