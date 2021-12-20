#include "components.h"




void GroupComponent::expandGroup(std::unordered_set<Entity>& outExpandedGroup) const
{
	for (Entity entity : _group)
	{
		if (entity.hasComponent<GroupComponent>())
			entity.getComponent<GroupComponent>().expandGroup(outExpandedGroup);

		outExpandedGroup.emplace(entity);
	}
}




void CameraComponent::setOrthographicCamera(const ClippingPlanes& clippingPlanes, const Rectf& viewportRect)
{
	_cameraProjection = CameraProjection::ORTHOGRAPHIC;
	_clippingPlanes = clippingPlanes;
	_viewportRect = viewportRect;
}


void CameraComponent::setPerspectiveCamera(const ClippingPlanes& clippingPlanes, float fov)
{
	_cameraProjection = CameraProjection::PERSPECTIVE;
	_clippingPlanes = clippingPlanes;
	_fov = fov;
}




void MeshComponent::setMaterial(const Material& material)
{
	memcpy(_mesh->mat.ambient, material.ambient, 4 * sizeof(float));
	memcpy(_mesh->mat.diffuse, material.diffuse, 4 * sizeof(float));
	memcpy(_mesh->mat.specular, material.specular, 4 * sizeof(float));
	memcpy(_mesh->mat.emissive, material.emissive, 4 * sizeof(float));
	_mesh->mat.shininess = material.shininess;
	_mesh->mat.texCount = material.texCount;
}