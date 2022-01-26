#ifndef __mm3d_scripts_butter_boostersDriftScript__
#define __mm3d_scripts_butter_boostersDriftScript__


#include <unordered_set>

#include "MicroMachines3D/common/include.h"




class BoostersDriftScript : public Script
{
private:
	constexpr static float SPEED_INCREASE = 15000.0f;




private:
	std::unordered_set<RigidbodyComponent*> _driftingBodies = std::unordered_set<RigidbodyComponent*>();




public:
	BoostersDriftScript() = delete;
	BoostersDriftScript(const BoostersDriftScript&) = default;
	BoostersDriftScript(Scene* scene) : Script(scene, "BoostersDriftScript") {}
	~BoostersDriftScript() = default;


public:
	void onFixedUpdate(float ts) override
	{
		for (auto& driftingBody : _driftingBodies)
		{
			float velocityX = driftingBody->velocity().x;
			float velocityZ = driftingBody->velocity().z;
			float driftForceX = BoostersDriftScript::SPEED_INCREASE * velocityX * ts;
			float driftForceZ = BoostersDriftScript::SPEED_INCREASE * velocityZ * ts;
			driftingBody->addAbsoluteForce(Coords3f({ driftForceX, 0.0f, driftForceZ }));
		}

		_driftingBodies.clear();
	}




public:
	inline void addDriftingBody(RigidbodyComponent* rigidbody) { _driftingBodies.emplace(rigidbody); }

};


#endif // !__mm3d_scripts_booster_boostersDriftScript__