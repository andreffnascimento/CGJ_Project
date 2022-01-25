#ifndef __engine_scene_components_skyboxComponent__
#define __engine_scene_components_skyboxComponent__


#include "engine/renderer/mesh/meshData.h"




struct SkyboxComponent
{
private:
	MeshData _meshData = MeshData();
	bool _enabled = true;

public:
	SkyboxComponent() = delete;
	SkyboxComponent(const SkyboxComponent&) = default;
	SkyboxComponent(unsigned int textureId);
	~SkyboxComponent() = default;

	inline const MeshData& meshData() const { return _meshData; }
	inline const MyMesh& mesh()	const { return _meshData.mesh(); }
	inline const Material& material() const { return _meshData.material(); }
	inline const Texture& texture() const { return _meshData.texture(); }
	inline bool enabled() const { return _enabled; }

	inline void setEnabled(bool enabled) { _enabled = enabled; }

};


#endif // !__engine_scene_components_skyboxComponent__