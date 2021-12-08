#ifndef __MM3D_SCENES_MICROMACHINES3D_H__
#define __MM3D_SCENES_MICROMACHINES3D_H__


#include "engine/scene/scene.h"



class MicroMachines3DScene : public Scene
{

public:
	MicroMachines3DScene();


private:
	void _init() override;


private:
	void _initCameras();
	void _initTable();
};


#endif // !__MM3D_SCENES_MICROMACHINES3D_H__
