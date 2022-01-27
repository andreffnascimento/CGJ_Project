#ifndef __engine_scene_components_fixedMirrorComponent__
#define __engine_scene_components_fixedMirrorComponent__


#include "engine/renderer/mesh/meshData.h"
#include "engine/renderer/mesh/geometry.h"
#include "engine/utils/coords.h"

 
struct  FixedMirrorComponent
{

private:
	MeshData _meshData = MeshData();

	float _screenPositionX = 0.0f;
	float _screenPositionY = 0.0f;

	Coords3f _cameraPosition = Coords3f();
	Coords3f _lookAt = Coords3f();

public:
	FixedMirrorComponent() = delete;
	FixedMirrorComponent(const FixedMirrorComponent&) = default;
	FixedMirrorComponent(const MyMesh& mesh, const Material& material);
	~FixedMirrorComponent() = default;

	inline const MeshData& meshData() const			{ return _meshData; }
	inline float screenPositionX()					{ return _screenPositionX; }
	inline float screenPositionY()					{ return _screenPositionY; }
	inline const Coords3f& lookAt() const			{ return _lookAt; }
	inline const Coords3f& cameraPosition() const	{ return _cameraPosition; }

	inline void setScreenPositionX(float screenPositionX)	{ _screenPositionX = screenPositionX; }
	inline void setScreenPositionY(float screenPositionY)	{ _screenPositionY = screenPositionY; }
	inline void setLookAt(Coords3f lookAt)				    { _lookAt = lookAt; }
	inline void setCameraPosition(Coords3f cameraPosition)  { _cameraPosition = cameraPosition; }

};


#endif // !__engine_scene_components_fixedMirrorComponent__