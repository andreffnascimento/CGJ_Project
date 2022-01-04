#ifndef __mm3d_entities_manager_gameManager__
#define __mm3d_entities_manager_gameManager__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/globalLightScript.h"




class GameManager : public SceneEntity
{

public:
	GameManager(Scene* scene)
		: SceneEntity(scene->createEntity("GameManager"))
	{
		Entity globalLight = _createGlobalLight(scene->createEntity("GlobalLight"));

		std::shared_ptr<Script> globalLightScript = std::make_shared<GlobalLightScript>(scene);
		addComponent<ScriptComponent>(globalLightScript);
	}




private:
	Entity _createGlobalLight(Entity entity)
	{
		LightComponent& lightComponent = entity.addComponent<LightComponent>(LightComponent::LightType::DIRECTIONAL, 2.0f);
		Transform::rotateTo(entity, Coords3f({ 90.0f, 0.0f, 0.0f }));
		return entity;	
	}
};


#endif // !__mm3d_entities_manager_gameManager__