#ifndef __mm3d_entities_manager_orangesManager__
#define __mm3d_entities_manager_orangesManager__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/oranges/orangesManagerScript.h"




class OrangesManager : public SceneEntity
{

public:
	OrangesManager(Scene* scene)
		: SceneEntity(scene->createEntity("OrangesManager"))
	{
		std::shared_ptr<Script> script = std::make_shared<OrangesManagerScript>(scene);
		ScriptComponent& scriptComponent = addComponent<ScriptComponent>(script);
	}

};

#endif // !__mm3d_entities_manager_orangesManager__