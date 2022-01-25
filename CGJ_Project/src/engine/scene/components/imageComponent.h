#ifndef __engine_scene_components_imageComponent__
#define __engine_scene_components_imageComponent__


#include <memory>

#include "engine/renderer/mesh/meshData.h"
#include "engine/renderer/mesh/geometry.h"
#include "engine/renderer/mesh/texture.h"




struct ImageComponent
{
	enum class ImageType
	{
		BILLBOARD = 1,
		CANVAS_IMAGE = 2,
	};


private:
	std::shared_ptr<MeshData> _meshData = nullptr;
	const Entity* _entity = nullptr;
	bool _enabled = true;

public:
	ImageComponent() = delete;
	ImageComponent(const ImageComponent&) = default;
	ImageComponent(const Entity& entity, const ImageComponent::ImageType& imageType = ImageComponent::ImageType::BILLBOARD);
	~ImageComponent() = default;

	inline const MeshData& meshData() const	{ return *_meshData; }
	inline const MyMesh& mesh()	const		{ return _meshData->mesh(); }
	inline const Material& material() const { return _meshData->material(); }
	inline const Texture& texture() const	{ return _meshData->texture(); }
	inline bool enabled() const				{ return _enabled; }

	inline void addImage(unsigned int imageId)	{ _meshData->addTexture(imageId); }
	inline void setEnabled(bool enabled)		{ _enabled = enabled; }

	void setBlendColor(const Coords4f& blendColor);

};


#endif // !__engine_scene_components_imageComponent__