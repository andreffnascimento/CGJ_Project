#ifndef __engine_scene_components_mirrorComponent__
#define __engine_scene_components_mirrorComponent__



#include "engine/scene/components/rigidbodyComponent.h"



struct MirrorComponent
{

private:
	Coords3f _mirrorNormal = Coords3f();

public:
	MirrorComponent() = delete;
	MirrorComponent(const MirrorComponent&) = default;
	~MirrorComponent() = default;

	inline const Coords3f& mirrorNormal() const { return _mirrorNormal; }
	inline void serMirrorNormal(const Coords3f& mirrorNormal) { _mirrorNormal = mirrorNormal; }
};


#endif // !__engine_scene_components_mirrorComponent__