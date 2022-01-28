#ifndef __engine_scene_components_reflectorPlaneComponent__
#define __engine_scene_components_reflectorPlaneComponent__


#include "engine/renderer/mesh/meshData.h"
#include "engine/renderer/mesh/geometry.h"
#include "engine/utils/coords.h"



struct  ReflectorPlaneComponent
{

private:
	MeshData _meshData = MeshData();

public:
	ReflectorPlaneComponent() = delete;
	ReflectorPlaneComponent(const ReflectorPlaneComponent&) = default;
	ReflectorPlaneComponent(const MyMesh& mesh, const Material& material);
	~ReflectorPlaneComponent() = default;

	inline const MeshData& meshData() const { return _meshData; }

};


#endif // !__engine_scene_components_reflectorPlaneComponent__