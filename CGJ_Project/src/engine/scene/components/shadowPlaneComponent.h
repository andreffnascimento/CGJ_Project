#ifndef __engine_scene_components_shadowPlaneComponent__
#define __engine_scene_components_shadowPlaneComponent__


#include "engine/scene/entity.h"
#include "engine/scene/components/transformComponent.h"
#include "engine/scene/components/meshComponent.h"




struct ShadowPlaneComponent
{

private:
	MeshComponent* _mesh = nullptr;
	const TransformComponent* _transform = nullptr;
	bool _enabled = true;

public:
	ShadowPlaneComponent() = delete;
	ShadowPlaneComponent(const ShadowPlaneComponent&) = default;
	ShadowPlaneComponent(const Entity& entity);
	~ShadowPlaneComponent() = default;

	inline const MeshComponent& mesh() const			{ return *_mesh; }
	inline const TransformComponent& transform() const	{ return *_transform; }
	inline bool enabled() const							{ return _enabled; }

	inline MeshComponent& mesh() { return *_mesh; }

	inline void setEnabled(bool enabled) { _enabled = enabled; }

};


#endif // !__engine_scene_components_shadowPlaneComponent__