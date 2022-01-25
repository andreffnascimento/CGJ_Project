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
		NONE,
		CYLINDRICAL_BILLBOARD,
		SPHERICAL_BILLBOARD,
		FAST_CYLINDRICAL_BILLBOARD,
		FAST_SPHERICAL_BILLBOARD,
		CANVAS_IMAGE
	};

private:
	const ImageComponent::ImageType _type = ImageComponent::ImageType::NONE;
	std::shared_ptr<MeshData> _meshData = nullptr;
	const Entity* _entity = nullptr;
	bool _enabled = true;

public:
	ImageComponent() = delete;
	ImageComponent(const ImageComponent&);
	ImageComponent(const Entity& entity, const ImageComponent::ImageType& type = ImageComponent::ImageType::NONE);
	~ImageComponent() = default;

	inline const ImageComponent::ImageType& type() const	{ return _type; }
	inline const MeshData& meshData() const					{ return *_meshData; }
	inline const MyMesh& mesh()	const						{ return _meshData->mesh(); }
	inline const Material& material() const					{ return _meshData->material(); }
	inline const Texture& texture() const					{ return _meshData->texture(); }
	inline bool enabled() const								{ return _enabled; }

	inline void addImage(unsigned int imageId)	{ _meshData->addTexture(imageId); }
	inline void setEnabled(bool enabled)		{ _enabled = enabled; }

	void setBlendColor(const Coords4f& blendColor);

};


#endif // !__engine_scene_components_imageComponent__