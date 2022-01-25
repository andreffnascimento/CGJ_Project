#ifndef __engine_scene_components_mirrorComponent__
#define __engine_scene_components_mirrorComponent__


#include <memory>

#include "engine/scene/components/rigidbodyComponent.h"



struct MirrorComponent
{

private:
	const RigidbodyComponent* _rigidbody = nullptr;
	const MeshComponent* _mesh = nullptr;

	Coords3f _mirrorNormal = Coords3f();

public:
	MirrorComponent() = delete;
	MirrorComponent(const MirrorComponent&) = default;
	MirrorComponent(const RigidbodyComponent& rigidbody, const MeshComponent& mesh);
	~MirrorComponent() = default;

	inline const Coords3f& mirrorNormal() const { return _mirrorNormal; }
	inline void serMirrorNormal(const Coords3f& mirrorNormal) { _mirrorNormal = mirrorNormal; }

	inline const RigidbodyComponent* rigidbody() const { return _rigidbody; }
	inline const MeshComponent* mesh() const { return _mesh; }
};


#endif // !__engine_scene_components_mirrorComponent__