#ifndef __engine_scene_components_imageComponent__
#define __engine_scene_components_imageComponent__


#include <memory>

#include "engine/renderer/mesh/meshData.h"
#include "engine/renderer/mesh/geometry.h"
#include "engine/renderer/mesh/texture.h"




struct ImageComponent
{
private:
	std::shared_ptr<MeshData> _meshData = std::make_shared<MeshData>(createQuad(1.0f, 1.0f), Material());

public:
	ImageComponent() = delete;
	ImageComponent(const ImageComponent&) = default;
	ImageComponent(unsigned int imageId);
	~ImageComponent() = default;

	inline const MeshData& meshData() const	{ return *_meshData; }
	inline const MyMesh& mesh()	const		{ return _meshData->mesh(); }
	inline const Material& material() const { return _meshData->material(); }
	inline const Texture& texture() const	{ return _meshData->texture(); }

};


#endif // !__engine_scene_components_imageComponent__