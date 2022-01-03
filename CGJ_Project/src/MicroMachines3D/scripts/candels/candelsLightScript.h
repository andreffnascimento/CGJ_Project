#ifndef __mm3d_scripts_candels_candelsLightScript__
#define __mm3d_scripts_candels_candelsLightScript__


#include "MicroMachines3D/common/include.h"

#include <list>




class CandelsLightScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	std::list<MeshComponent*> _meshes = std::list<MeshComponent*>();
	std::list<LightComponent*> _lights = std::list<LightComponent*>();

	bool _candelsOn = false;




public:
	CandelsLightScript() = delete;
	CandelsLightScript(const CandelsLightScript&) = default;
	CandelsLightScript(Scene* scene) : Script(scene) {}
	~CandelsLightScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		for (auto& candel : _scene->getEntitiesByTag(std::regex("^Candels:light_.*$")))
		{
			_meshes.push_back(&candel.getComponent<MeshComponent>());
			_lights.push_back(&candel.getComponent<LightComponent>());
		}
	}


	void onUpdate(float ts) override
	{
		if (_eventHandler->keyState('C').pressed() || _eventHandler->keyState('c').pressed())
		{
			_candelsOn = !_candelsOn;

			for (auto& light : _lights)
				light->setEnabled(_candelsOn);

			if (_candelsOn)
			{
				for (auto& mesh : _meshes)
					mesh->setMaterial(CANDEL_ON_MATERIAL);
			}
			else
			{
				for (auto& mesh : _meshes)
					mesh->setMaterial(CANDEL_OFF_MATERIAL);
			}


		}
			
	}

};


#endif // !__mm3d_scripts_candels_candelsLightScript__