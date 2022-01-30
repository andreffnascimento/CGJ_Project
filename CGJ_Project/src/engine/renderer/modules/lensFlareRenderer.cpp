#include "engine/renderer/renderer.h"

#include "engine/math/AVTmathLib.h"
#include "engine/utils/mathUtils.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];



#include <iostream>
void Renderer::_renderLensFlares(const Scene& scene) const 
{
	int viewport[4] = {};
	glGetIntegerv(GL_VIEWPORT, viewport);
	_initLensFlareRendering();

	for (auto& lensFlareIterator : scene.getSceneComponents<LensFlareComponent>())
	{
		const LensFlareComponent& lensFlare = lensFlareIterator.second;
		if (!lensFlare.enabled())
			continue;
		
		Coords3f cameraTranslation;
		Transform::decomposeTransformMatrix(scene.activeCamera(), cameraTranslation, Quaternion(), Coords3f());
		const Coords3f& cameraLookAt = (scene.activeCamera().getComponent<CameraComponent>().targetCoords() - cameraTranslation).normalized();
		if (cameraLookAt.dot(lensFlare.lightPosition()) < 0)
			continue;

		WindowCoords flareProjectedPosition = _calculateLensFlareProjectedPos(lensFlare, viewport);
		pushMatrix(PROJECTION);
		loadIdentity(PROJECTION);
		pushMatrix(VIEW);
		loadIdentity(VIEW);
		ortho((float)viewport[0], (float)viewport[0] + (float)viewport[2] - 1.0f, (float)viewport[1], (float)viewport[1] + (float)viewport[3] - 1.0f, -1.0f, 1.0f);
		_renderLensFlare(lensFlare.flareData(), flareProjectedPosition, viewport);
		popMatrix(VIEW);
		popMatrix(PROJECTION);
	}

	_terminateLensFlareRendering();
}




void Renderer::_initLensFlareRendering() const
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (unsigned int)RendererSettings::RendererMode::LENS_FLARE_RENDERER);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Renderer::_terminateLensFlareRendering() const
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}


WindowCoords Renderer::_calculateLensFlareProjectedPos(const LensFlareComponent& lensFlare, int viewport[4]) const
{
	loadIdentity(MODEL);
	computeDerivedMatrix(PROJ_VIEW_MODEL);

	float lightScreenPos[3] = {};
	float lightPos[4] = { lensFlare.lightPosition().x, lensFlare.lightPosition().y, lensFlare.lightPosition().z, 1.0f };
	if (!project(lightPos, lightScreenPos, viewport))
		throw std::string("Unable to get the lens flare projected light in the screen!");

	return { 
		(unsigned int)clamp((int)lightScreenPos[0], viewport[0], viewport[0] + viewport[2] - 1),
		(unsigned int)clamp((int)lightScreenPos[1], viewport[1], viewport[1] + viewport[3] - 1)
	};
}


void Renderer::_addLensFlareToInstanceBuffer(RendererData::SubmitInstanceBuffer& instanceBuffer, WindowCoords flarePosition, int width, int height) const
{
	loadIdentity(MODEL);
	translate(MODEL, (float)((float)flarePosition.x - (float)width * 0.0f), (float)((float)flarePosition.y - (float)height * 0.0f), 0.0f);
	scale(MODEL, (float)width, (float)height, 1.0f);
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	computeNormalMatrix3x3();

	memcpy(instanceBuffer.pvmMatrix[instanceBuffer.nInstances],    mCompMatrix[PROJ_VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(instanceBuffer.vmMatrix[instanceBuffer.nInstances],     mCompMatrix[VIEW_MODEL],      4 * 4 * sizeof(float));
	memcpy(instanceBuffer.normalMatrix[instanceBuffer.nInstances], mNormal3x3,                   3 * 3 * sizeof(float));
	instanceBuffer.nInstances++;
}


void Renderer::_submitLensFlareBuffer(const Coords4f& color, unsigned int colorMapId, float scaleDistance) const
{
	glUniform4f(_uniformLocator[RendererUniformLocations::LENS_FLARE_COLOR], color.x, color.y, color.z, color.w * scaleDistance);
	glUniform1ui(_uniformLocator[RendererUniformLocations::LENS_FLARE_COLOR_MAP_ID], colorMapId);
}


void Renderer::_renderLensFlare(const LensFlareComponent::FlareData& lensFlareData, const WindowCoords& flareProjectedPosition, int viewport[4]) const
{
	WindowCoords screenSize = { (unsigned int)(viewport[0] + viewport[2] - 1), (unsigned int)(viewport[1] + viewport[3] - 1) };
	WindowCoords viewportCenter = { (unsigned int)(viewport[0] + (int)(0.5f * (float)viewport[2]) - 1), (unsigned int)(viewport[1] + (int)(0.5f * (float)viewport[3]) - 1) };

	float maxFlareDistance = (float)std::sqrt(viewportCenter.x * viewportCenter.x + viewportCenter.y * viewportCenter.y);
	float flareDistance = (float)std::sqrt((flareProjectedPosition.x - viewportCenter.x) * (flareProjectedPosition.x - viewportCenter.x) + 
		                                   (flareProjectedPosition.y - viewportCenter.y) * (flareProjectedPosition.y - viewportCenter.y));

	float scaleDistance = (maxFlareDistance - flareDistance) / maxFlareDistance;
	int flareMaxSize = (int)(viewport[2] * lensFlareData.maxSize);
	int flareScale = (int)(viewport[2] * lensFlareData.scale);

	WindowCoords destination = {
		(unsigned int)clamp(viewportCenter.x + (viewportCenter.x - flareProjectedPosition.x), viewport[0], screenSize.x),
		(unsigned int)clamp(viewportCenter.y + (viewportCenter.y - flareProjectedPosition.y), viewport[1], screenSize.y)
	};

	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
	for (unsigned int i = 0; i < lensFlareData.nElements; i++)
	{
		LensFlareComponent::FlareElement flareElement = lensFlareData.elements[i];
		WindowCoords flarePosition = {
			(unsigned int)clamp((int)((1.0f - flareElement.distance) * flareProjectedPosition.x + flareElement.distance * destination.x), viewport[0], screenSize.x),
			(unsigned int)clamp((int)((1.0f - flareElement.distance) * flareProjectedPosition.y + flareElement.distance * destination.y), viewport[1], screenSize.y)
		};

		int width = std::min((int)(scaleDistance * flareScale * flareElement.size), flareMaxSize);
		int height = (int)((float)viewport[3] / (float)viewport[2] * (float)width);

		if (width > 1)
		{
			static MeshData lensFlareMesh = MeshData(createQuad(1.0f, 1.0f), Material());
			_submitLensFlareBuffer(flareElement.color, flareElement.colorMapId, scaleDistance);
			_addLensFlareToInstanceBuffer(instanceBuffer, flarePosition, width, height);
			_submitRenderableData(lensFlareMesh, instanceBuffer);
		}
	}	
}