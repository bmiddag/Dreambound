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

#include <spine/spine-sfml.h>
#include <spine/extension.h>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

using namespace sf;

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path){
	Texture* texture = new Texture();
	if (!texture->loadFromFile(path)) return;
	texture->setSmooth(true);
	self->rendererObject = texture;
	Vector2u size = texture->getSize();
	self->width = size.x;
	self->height = size.y;
}

void _spAtlasPage_disposeTexture (spAtlasPage* self){
	delete (Texture*)self->rendererObject;
}

char* _spUtil_readFile (const char* path, int* length){
	return _readFile(path, length);
}

/**/

namespace spine {

SkeletonDrawable::SkeletonDrawable (spSkeletonData* skeletonData, spAnimationStateData* stateData) :
				timeScale(1),
				vertexArray(new VertexArray(Triangles, skeletonData->boneCount * 4)),
				worldVertices(0) {
	spBone_setYDown(true);
	worldVertices = MALLOC(float, SPINE_MESH_VERTEX_COUNT_MAX);
	skeleton = spSkeleton_create(skeletonData);
	state = spAnimationState_create(stateData);
}

SkeletonDrawable::~SkeletonDrawable () {
	delete vertexArray;
	FREE(worldVertices);
	spAnimationState_dispose(state);
	spSkeleton_dispose(skeleton);
}

void SkeletonDrawable::update (float deltaTime) {
	spSkeleton_update(skeleton, deltaTime);
	spAnimationState_update(state, deltaTime * timeScale);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);
}

void SkeletonDrawable::draw (RenderTarget& target, RenderStates states) const {
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
