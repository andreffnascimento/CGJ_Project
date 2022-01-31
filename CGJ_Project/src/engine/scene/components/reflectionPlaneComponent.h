#ifndef __engine_scene_components_reflectionPlaneComponent__
#define __engine_scene_components_reflectionPlaneComponent__


#include "engine/scene/entity.h"
#include "engine/scene/components/transformComponent.h"
#include "engine/scene/components/meshComponent.h"




struct ReflectionPlaneComponent
{

private:
	MeshComponent* _mesh = nullptr;
	const TransformComponent* _transform = nullptr;
	bool _enabled = true;

public:
	ReflectionPlaneComponent() = delete;
	ReflectionPlaneComponent(const ReflectionPlaneComponent&) = default;
	ReflectionPlaneComponent(const Entity& entity);
	~ReflectionPlaneComponent() = default;

	inline const MeshComponent& mesh() const { return *_mesh; }
	inline const TransformComponent& transform() const { return *_transform; }
	inline bool enabled() const { return _enabled; }

	inline MeshComponent& mesh() { return *_mesh; }

	inline void setEnabled(bool enabled) { _enabled = enabled; }

};


#endif // !__engine_scene_components_reflectionPlaneComponent__