#ifndef __engine_scene_components_flatMirrorComponent__
#define __engine_scene_components_flatMirrorComponent__


#include "engine/scene/cameraEntity.h"
#include "engine/renderer/mesh/meshData.h"
#include "engine/renderer/mesh/geometry.h"
#include "engine/utils/coords.h"




 
struct  FlatMirrorComponent
{

private:
	MeshData _meshData = MeshData();
	const TransformComponent* _mirrorTransform = nullptr;

	CameraEntity _camera = CameraEntity();

	bool _enabled = true;

public:
	FlatMirrorComponent() = delete;
	FlatMirrorComponent(const FlatMirrorComponent&) = default;
	FlatMirrorComponent(const Entity& entity, const MyMesh& mesh, const Material& material);
	~FlatMirrorComponent() = default;

	inline const MeshData& meshData() const				{ return _meshData; }
	inline const TransformComponent& transform() const	{ return *_mirrorTransform; }
	inline const CameraEntity& camera() const			{ return _camera; }
	inline bool enabled() const							{ return _enabled; }

	inline CameraEntity& camera()				{ return _camera; }
	inline CameraComponent& cameraComponent()	{ return (CameraComponent&)_camera; }

	inline void setCamera(const CameraEntity& camera)		{ _camera = camera; }
	inline void setEnabled(bool enabled)					{ _enabled = enabled; }

};


#endif // !__engine_scene_components_flatMirrorComponent__