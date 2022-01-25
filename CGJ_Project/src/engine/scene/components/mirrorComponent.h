#ifndef __engine_scene_components_mirrorComponent__
#define __engine_scene_components_mirrorComponent__


#include <memory>

#include "engine/scene/components/rigidbodyComponent.h"



struct MirrorComponent
{

private:
	RigidbodyComponent* _rigidbody = nullptr;
	MeshComponent* _mesh = nullptr;

	Coords3f _mirrorNormal = { 0.0f, 0.0f, 1.0f };

public:
	MirrorComponent() = delete;
	MirrorComponent(const MirrorComponent&) = default;
	MirrorComponent(RigidbodyComponent& rigidbody, MeshComponent& mesh, Coords3f mirrorNormal);
	~MirrorComponent() = default;

	inline const Coords3f& mirrorNormal() const { return _mirrorNormal; }
	inline void mirrorNormal(const Coords3f& mirrorNormal) { _mirrorNormal = mirrorNormal; }

	inline RigidbodyComponent* rigidbody() const { return _rigidbody; }
	inline MeshComponent* mesh() const { return _mesh; }
};


#endif // !__engine_scene_components_mirrorComponent__