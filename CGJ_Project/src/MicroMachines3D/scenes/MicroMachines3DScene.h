#ifndef __mm3d_scenes_MicroMachines3D__
#define __mm3d_scenes_MicroMachines3D__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/entities/manager/gameManager.h"
#include "MicroMachines3D/entities/manager/cameraManager.h"

#include "MicroMachines3D/entities/world/table.h"
#include "MicroMachines3D/entities/world/floatingCube.h"
#include "MicroMachines3D/entities/world/car.h"
#include "MicroMachines3D/entities/world/cheeriosTrack.h"
#include "MicroMachines3D/entities/world/candels.h"
#include "MicroMachines3D/entities/world/oranges.h"
#include "MicroMachines3D/entities/world/butters.h"
#include "MicroMachines3D/entities/world/boosters.h"
#include "MicroMachines3D/entities/world/checkeredFlag.h"
#include "MicroMachines3D/entities/world/planets.h"
#include "MicroMachines3D/entities/world/skybox.h"
#include "MicroMachines3D/entities/world/rearViewMirror.h"

#include "MicroMachines3D/entities/world/floatingCube.h"

#include "MicroMachines3D/entities/hud/playingScreen.h"
#include "MicroMachines3D/entities/hud/pauseScreen.h"
#include "MicroMachines3D/entities/hud/gameoverScreen.h"

#include "MicroMachines3D/entities/particles/fireworks.h"




class MicroMachines3DScene : public Scene
{
	
public:
	MicroMachines3DScene() : Scene() {}




private:
	void setupScene() override
	{
		RendererSettings::ReflectionCoefficients reflectionCoefficients = RendererSettings::ReflectionCoefficients();
		reflectionCoefficients.ambient = 0.5f;
		reflectionCoefficients.diffuse = 1.0f;
		reflectionCoefficients.specular = 1.0f;
		reflectionCoefficients.darkTexture = 0.1f;

		RendererSettings::Fog fog = RendererSettings::Fog();
		fog.mode = RendererSettings::Fog::FogMode::EXP;
		fog.color = Coords4f{ 0.27f, 0.07f, 0.03f, 1.0f };
		fog.density = 0.025f;
		fog.active = true;

		Renderer::setReflectionCoefficients(reflectionCoefficients);
		Renderer::setFog(fog);
		Renderer::setEnvironmentalMappingBlendingAmount(0.9f);

		


		GameManager gameManager = GameManager(this);
		CameraManager cameraManager = CameraManager(this);

		Skybox skybox = Skybox(this);

		Table table = Table(this);
		FloatingCube floater = FloatingCube(this);

		Car car = Car(this);
		CheeriosTrack cheeriosTrack = CheeriosTrack(this);
		Candels candels = Candels(this);
		Oranges oranges = Oranges(this);
		Butters butters = Butters(this);
		Boosters boosters = Boosters(this);
		CheckeredFlag flag = CheckeredFlag(this);
		Planets planets = Planets(this);
		RearViewMirror mirror = RearViewMirror(this);

		PlayingScreen playingScreen = PlayingScreen(this);
		PauseScreen pauseScreen = PauseScreen(this);
		GameoverScreen gameoverScreen = GameoverScreen(this);

		Fireworks fireworks = Fireworks(this);
	}
};


#endif // !__mm3d_scenes_MicroMachines3D__