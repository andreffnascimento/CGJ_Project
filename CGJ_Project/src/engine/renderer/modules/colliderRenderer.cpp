#include "engine/renderer/renderer.h"

#include <memory>

#include "engine/renderer/geometry.h"

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::renderColliders(const Scene& scene) const
{
	PhysicsEngine& physicsEngine = Application::getPhysicsEngine();
	if (!physicsEngine.showColliders())
		return;

	const MeshComponent& mesh = getColliderMesh();
	const auto& colliderComponents = scene.getSceneComponents<AABBColliderComponent>();
	for (const auto& colliderIterator : colliderComponents)
	{
		EntityHandle entityId = colliderIterator.first;
		const AABBColliderComponent& collider = colliderIterator.second;
		const RigidbodyComponent& rigidbody = collider.rigidbody();
		const Entity& entity = scene.getEntityById(entityId);

		_loadMesh(mesh);		
		_applyColliderTransform(collider);
		_renderMesh(mesh);
	}
}




const MeshComponent& Renderer::getColliderMesh() const
{
	constexpr Material COLLIDER_MATERIAL = {
		{ 0.0f, 0.0f, 0.7f, 1.0f },
		{ 0.0f, 0.0f, 0.9f, 1.0f },
		{ 0.0f, 0.0f, 0.9f, 1.0f, },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		100.0f,
		0
	};

	static std::shared_ptr<MyMesh> mesh = std::make_shared<MyMesh>(createCube());
	static MeshComponent meshComponent = MeshComponent(mesh, COLLIDER_MATERIAL);
	return meshComponent;
}


void Renderer::_applyColliderTransform(const AABBColliderComponent& collider) const
{
	TransformComponent transform = TransformComponent();
	Transform::translateTo(transform, collider.rigidbody().position());
	Transform::scaleTo(transform, collider.boundingBox() * 2.0f);

	pushMatrix(MODEL);
	loadMatrix(MODEL, Transform::calculateTransformMatrix(transform));
}