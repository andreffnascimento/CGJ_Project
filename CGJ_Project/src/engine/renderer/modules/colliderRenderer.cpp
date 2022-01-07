#include "engine/renderer/renderer.h"

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

	const MeshData& meshData = getColliderMesh().meshData();
	_submitMeshData(meshData);

	RendererData::SubmitInstanceData colliderInstanceBuffer = RendererData::SubmitInstanceData();
	const auto& colliderComponents = scene.getSceneComponents<AABBColliderComponent>();
	for (const auto& colliderIterator : colliderComponents)
	{
		if (colliderInstanceBuffer.nRenderableInstances >= RendererSettings::MAX_RENDERABLE_INSTANCES_SUBMISSION)
			_submitRenderableData(meshData, colliderInstanceBuffer);

		const AABBColliderComponent& collider = colliderIterator.second;
		_formatColliderInstanceBuffer(colliderInstanceBuffer, collider);
	}

	_submitRenderableData(meshData, colliderInstanceBuffer);
}


const MeshComponent& Renderer::getColliderMesh() const
{
	constexpr Material COLLIDER_MATERIAL = {
		{ 0.0f, 0.0f, 0.7f, 1.0f },
		{ 0.0f, 0.0f, 0.9f, 1.0f },
		{ 0.0f, 0.0f, 0.9f, 1.0f, },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		100.0f,
	};

	static MeshComponent mesh = MeshComponent(createCube(), COLLIDER_MATERIAL);
	return mesh;
}


void Renderer::_formatColliderInstanceBuffer(RendererData::SubmitInstanceData& renderableInstanceBuffer, const AABBColliderComponent& collider) const
{
	_applyColliderTransform(collider);

	memcpy(renderableInstanceBuffer.pvmMatrix[renderableInstanceBuffer.nRenderableInstances], mCompMatrix[PROJ_VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(renderableInstanceBuffer.vmMatrix[renderableInstanceBuffer.nRenderableInstances], mCompMatrix[VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(renderableInstanceBuffer.normalMatrix[renderableInstanceBuffer.nRenderableInstances], mNormal3x3, 3 * 3 * sizeof(float));

	renderableInstanceBuffer.nRenderableInstances++;
}


void Renderer::_applyColliderTransform(const AABBColliderComponent& collider) const
{
	TransformComponent transform = TransformComponent();
	Transform::translateTo(transform, collider.rigidbody().position());
	Transform::scaleTo(transform, collider.boundingBox() * 2.0f);

	pushMatrix(MODEL);
	loadMatrix(MODEL, Transform::calculateTransformMatrix(transform));
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	computeNormalMatrix3x3();
}