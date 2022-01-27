#ifndef __mm3d_entities_manager_gameManager__
#define __mm3d_entities_manager_gameManager__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/globalLightScript.h"
#include "MicroMachines3D/scripts/manager/game/raceManagerScript.h"




class GameManager : public SceneEntity
{

public:
	GameManager(Scene* scene)
		: SceneEntity(scene->createEntity("GameManager"))
	{
		_createGlobalLight(scene);

		ScriptComponent& script = addComponent<ScriptComponent>();
		script.addScriptInstance(std::make_shared<GlobalLightScript>(scene));
		script.addScriptInstance(std::make_shared<RaceManagerScript>(scene));
	}




private:
	void _createGlobalLight(Scene* scene)
	{
		Entity globalLight = scene->createEntity("GlobalLight");
		LightComponent& lightComponent = globalLight.addComponent<LightComponent>(LightComponent::LightType::DIRECTIONAL, 2.0f);
		LensFlareComponent& lensFlare = globalLight.addComponent<LensFlareComponent>("src/MicroMachines3D/flares/sunFlare.txt");
		lensFlare.setLightPosition(PLANET_SUN_POSITION);
		Transform::rotateTo(globalLight, Coords3f({ 30.0f, 0.0f, 0.0f }));
	}
};


#endif // !__mm3d_entities_manager_gameManager__