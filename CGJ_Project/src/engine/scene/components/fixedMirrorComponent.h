#ifndef __engine_scene_components_fixedMirrorComponent__
#define __engine_scene_components_fixedMirrorComponent__


#include "engine/renderer/mesh/meshData.h"
#include "engine/renderer/mesh/geometry.h"
#include "engine/utils/coords.h"

 
struct  FixedMirrorComponent
{

private:
	MeshData _meshData = MeshData();
	Coords3f _cameraPosition = Coords3f();
	Coords3f _lookAt = Coords3f();

	bool _enabled = true;

public:
	FixedMirrorComponent() = delete;
	FixedMirrorComponent(const FixedMirrorComponent&) = default;
	FixedMirrorComponent(const MyMesh& mesh, const Material& material);
	~FixedMirrorComponent() = default;

	inline const MeshData& meshData() const			{ return _meshData; }
	inline const Coords3f& lookAt() const			{ return _lookAt; }
	inline const Coords3f& cameraPosition() const	{ return _cameraPosition; }
	inline bool enabled() const						{ return _enabled; }

	inline void setLookAt(Coords3f lookAt)				    { _lookAt = lookAt; }
	inline void setCameraPosition(Coords3f cameraPosition)  { _cameraPosition = cameraPosition; }
	inline void setEnabled(bool enabled)					{ _enabled = enabled; }

};


#endif // !__engine_scene_components_fixedMirrorComponent__