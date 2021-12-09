#ifndef __ENGINE_SCENE_COMPONENTS_RENDERABLE_H__
#define __ENGINE_SCENE_COMPONENTS_RENDERABLE_H__


#include <memory>

#include "engine/scene/entity.h"
#include "engine/renderer/geometry.h"
#include "engine/utils/coords.h"


class Renderer;




class Renderable : public Entity
{
public:
	struct TransformData
	{
		Coords3f position	= { 0.0f, 0.0f, 0.0f };
		Coords3f scale		= { 1.0f, 1.0f, 1.0f };
		Coords3f rotation	= { 0.0f, 0.0f, 0.0f };
	};




protected:
	std::shared_ptr<MyMesh> _mesh;
	TransformData _transform;


protected:
	Renderable();


public:
	void setMaterial(const Material &material);
	void setPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setRotation(float x, float y, float z);


public:
	virtual void renderObject(const Renderer &renderer) const;

};


#endif // !__ENGINE_SCENE_COMPONENTS_RENDERABLE_H__
