#include "engine/renderer/renderer.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/l3DBillboard.h"
#include "engine/math/transform.h"

extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::renderImages(const Scene& scene) const
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (unsigned int)RendererSettings::RendererMode::IMAGE_RENDERER);
	_initImageRendering();
	_renderImageMeshInstances(scene);
	_terminateImageRendering();
}




void Renderer::_initImageRendering() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Renderer::_terminateImageRendering() const
{
	glDisable(GL_BLEND);
}


void Renderer::_renderImageMeshInstances(const Scene& scene) const
{
	Coords3f cameraPos;
	Transform::decomposeTransformMatrix(scene.activeCamera(), cameraPos, Quaternion(), Coords3f());

	for (const auto& imageIterator : _imageMeshInstances)
	{
		RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
		const MeshData* meshData = imageIterator.first;
		const std::unordered_map<const ImageComponent*, const TransformComponent*> meshInstances = imageIterator.second;
		_submitMeshData(*meshData);

		for (const auto& meshInstanceIterator : meshInstances)
		{
			const ImageComponent* image = meshInstanceIterator.first;
			const TransformComponent* transform = meshInstanceIterator.second;
			if (!image->enabled())
				continue;

			if (instanceBuffer.nInstances >= RendererSettings::MAX_INSTANCES_PER_SUBMISSION)
				_submitRenderableData(*meshData, instanceBuffer);

			_addImageToInstanceBuffer(instanceBuffer, transform, image->type(), cameraPos);
		}

		_submitRenderableData(*meshData, instanceBuffer);
	}
}


void Renderer::_addImageToInstanceBuffer(RendererData::SubmitInstanceBuffer& instanceBuffer, const TransformComponent* transform, const ImageComponent::ImageType& imageType, const Coords3f& cameraPos) const
{
	loadIdentity(MODEL);
	loadMatrix(MODEL, transform->transformMatrix());

	if (imageType == ImageComponent::ImageType::CYLINDRICAL_BILLBOARD)
	{
		l3dBillboardCylindricalBegin(cameraPos, Coords3f({ transform->transformMatrix()[3][0], transform->transformMatrix()[3][1], transform->transformMatrix()[3][2] }));
		computeDerivedMatrix(PROJ_VIEW_MODEL);
	}
	else if (imageType == ImageComponent::ImageType::SPHERICAL_BILLBOARD)
	{
		l3dBillboardSphericalBegin(cameraPos, Coords3f({ transform->transformMatrix()[3][0], transform->transformMatrix()[3][1], transform->transformMatrix()[3][2] }));
		computeDerivedMatrix(PROJ_VIEW_MODEL);
	}
	else if (imageType == ImageComponent::ImageType::FAST_CYLINDRICAL_BILLBOARD)
	{
		computeDerivedMatrix(VIEW_MODEL);
		BillboardCheatCylindricalBegin(transform->scale().x, transform->scale().y);
		computeDerivedMatrix_PVM();
	}
	else if (imageType == ImageComponent::ImageType::FAST_SPHERICAL_BILLBOARD)
	{
		computeDerivedMatrix(VIEW_MODEL);
		BillboardCheatSphericalBegin(transform->scale().x, transform->scale().y);
		computeDerivedMatrix_PVM();
	}

	computeNormalMatrix3x3();

	memcpy(instanceBuffer.pvmMatrix[instanceBuffer.nInstances], mCompMatrix[PROJ_VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(instanceBuffer.vmMatrix[instanceBuffer.nInstances], mCompMatrix[VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(instanceBuffer.normalMatrix[instanceBuffer.nInstances], mNormal3x3, 3 * 3 * sizeof(float));
	instanceBuffer.nInstances++;
}