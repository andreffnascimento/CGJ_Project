#ifndef __engine_scene_components_meshComponent__
#define __engine_scene_components_meshComponent__


#include <memory>

#include "engine/renderer/mesh/meshData.h"
#include "engine/renderer/mesh/geometry.h"
#include "engine/renderer/mesh/texture.h"

class Entity;




struct MeshComponent
{

private:
	std::shared_ptr<MeshData> _meshData = nullptr;
	const Entity* _entity = nullptr;
	bool _enabled = true;

public:
	MeshComponent() = delete;
	MeshComponent(const MeshComponent& mesh);
	MeshComponent(const MyMesh& mesh, const Material& material);						// initializes the mesh but doesn't set it up for rendering
	MeshComponent(const MyMesh& mesh, const Material& material, const Entity& entity);	// initializes the mesh and sets it up for rendering
	MeshComponent(const MeshComponent& mesh, const Entity& entity);						// sets the mesh up for rendering
	~MeshComponent() = default;

	inline const MeshData& meshData() const { return *_meshData; }
	inline const MyMesh& mesh()	const		{ return _meshData->mesh(); }
	inline const Material& material() const { return _meshData->material(); }
	inline const Texture& texture() const	{ return _meshData->texture(); }
	inline bool enabled() const				{ return _enabled; }

	inline void setMaterial(const Material& material)					{ _meshData->setMaterial(material); }
	inline void setTextureMode(const Texture::TextureMode& textureMode) { _meshData->setTextureMode(textureMode); }
	inline void addTexture(unsigned int textureId)						{ _meshData->addColorMap(textureId); }
	inline void addNormal(unsigned int normalId)						{ _meshData->addNormalMap(normalId); }
	inline void setEnabled(bool enabled)								{ _enabled = enabled; }

	inline operator const MeshData&() const { return *_meshData; }
};


#endif // !__engine_scene_components_meshComponent__