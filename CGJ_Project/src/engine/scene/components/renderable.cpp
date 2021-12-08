#include "renderable.h"

#include "engine/renderer/renderer.h"




Renderable::Renderable()
	: Entity(), _mesh(MyMesh()), _transform(Renderable::TransformData())
{
	// empty
}


Renderable::~Renderable()
{
	for (auto &child : _children)
		delete child;
}




void Renderable::setMaterial(const Material &material)
{
	memcpy(_mesh.mat.ambient, material.ambient, 4 * sizeof(float));
	memcpy(_mesh.mat.diffuse, material.diffuse, 4 * sizeof(float));
	memcpy(_mesh.mat.specular, material.specular, 4 * sizeof(float));
	memcpy(_mesh.mat.emissive, material.emissive, 4 * sizeof(float));
	_mesh.mat.shininess = material.shininess;
	_mesh.mat.texCount = material.texCount;
}


void Renderable::setPosition(float x, float y, float z)
{
	_transform.position.x = x;
	_transform.position.y = y;
	_transform.position.z = z;
}


void Renderable::setScale(float x, float y, float z)
{
	_transform.scale.x = x;
	_transform.scale.y = y;
	_transform.scale.z = z;
}


void Renderable::setRotation(float x, float y, float z)
{
	_transform.rotation.x = x;
	_transform.rotation.y = y;
	_transform.rotation.z = z;
}




void Renderable::renderObject(const Renderer &renderer) const
{
	renderer.renderObject(_mesh, _transform);
	for (auto& child : _children)
		child->renderObject(renderer);
	renderer.terminateRenderObject();
}