#ifndef __mm3d_scripts_butter_buttersDriftScript__
#define __mm3d_scripts_butter_buttersDriftScript__


#include <unordered_set>

#include "MicroMachines3D/common/include.h"




class ButtersDriftScript : public Script
{
private:
	constexpr static float SPEED_DECREASE = 200000.0f;




private:
	std::unordered_set<RigidbodyComponent*> _driftingBodies = std::unordered_set<RigidbodyComponent*>();




public:
	ButtersDriftScript() = delete;
	ButtersDriftScript(const ButtersDriftScript&) = default;
	ButtersDriftScript(Scene* scene) : Script(scene, "ButtersDriftScript") {}
	~ButtersDriftScript() = default;


public:
	void onFixedUpdate(float ts) override
	{
		for (auto& driftingBody : _driftingBodies)
		{
			float velocityX = driftingBody->velocity().x;
			float velocityZ = driftingBody->velocity().z;
			float driftForceX = ButtersDriftScript::SPEED_DECREASE * velocityX * ts;
			float driftForceZ = ButtersDriftScript::SPEED_DECREASE * velocityZ * ts;
			driftingBody->addAbsoluteForce(Coords3f({ -driftForceX, 0.0f, -driftForceZ }));
		}

		_driftingBodies.clear();
	}




public:
	inline void addDriftingBody(RigidbodyComponent* rigidbody) { _driftingBodies.emplace(rigidbody); }

};


#endif // !__mm3d_scripts_butter_buttersDriftScript__