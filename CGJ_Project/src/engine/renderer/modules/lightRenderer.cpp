#include "engine/renderer/renderer.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"




void Renderer::_renderLights(const Scene& scene) const
{
	RendererData::SubmitLightBuffer lightBuffer = RendererData::SubmitLightBuffer();

	const auto& lightComponents = scene.getSceneComponents<LightComponent>();
	if (lightComponents.size() > RendererSettings::MAX_LIGHTS)
		throw std::string("The renderer is not able to process all the game lights");

	for (const auto& lightIterator : lightComponents)
	{
		Entity lightEntity = scene.getEntityById(lightIterator.first);
		const LightComponent& light = lightIterator.second;

		if (!light.isEnabled())
			continue;

		Coords3f translation;
		Quaternion rotation;
		Coords3f scale;
		Transform::decomposeTransformMatrix(lightEntity, translation, rotation, scale);

		switch (light.lightType())
		{
		case LightComponent::LightType::DIRECTIONAL:
			_formatDirectionalLight(light, rotation.rotatePoint(Coords3f({ 0.0f, 0.0f, 1.0f })), lightBuffer);
			break;

		case LightComponent::LightType::POINT:
			_formatPointLight(light, translation, lightBuffer);
			break;

		case LightComponent::LightType::SPOT:
			_formatSpotLight(light, translation, rotation.rotatePoint(Coords3f({ 0.0f, 0.0f, 1.0f })), lightBuffer);
			break;
		}

		lightBuffer.nLights++;
	}

	lightBuffer.ambientCoefficient = _reflectionCoefficients.ambient;
	lightBuffer.diffuseCoefficient = _reflectionCoefficients.diffuse;
	lightBuffer.specularCoefficient = _reflectionCoefficients.specular;
	lightBuffer.darkTextureCoefficient = _reflectionCoefficients.darkTexture;
	_submitLightData(lightBuffer);
}



void Renderer::_formatDirectionalLight(const LightComponent& light, const Coords3f& direction, RendererData::SubmitLightBuffer& lightBuffer) const
{
	float lightDirection[4] = { direction.x, direction.y, direction.z, 0.0f };
	float viewLightDirection[4] = {};
	multMatrixPoint(VIEW, lightDirection, viewLightDirection);

	lightBuffer.lightType[lightBuffer.nLights] = (unsigned int)LightComponent::LightType::DIRECTIONAL;
	memcpy(&lightBuffer.lightDirection[4 * lightBuffer.nLights], viewLightDirection, 4 * sizeof(float));
	lightBuffer.lightIntensity[lightBuffer.nLights] = light.intensity();
}


void Renderer::_formatPointLight(const LightComponent& light, const Coords3f& translation, RendererData::SubmitLightBuffer& lightBuffer) const
{
	float lightPosition[4] = { translation.x, translation.y, translation.z, 1.0f };
	float viewLightPosition[4] = {};
	multMatrixPoint(VIEW, lightPosition, viewLightPosition);

	lightBuffer.lightType[lightBuffer.nLights] = (unsigned int)LightComponent::LightType::POINT;
	memcpy(&lightBuffer.lightPosition[4 * lightBuffer.nLights], viewLightPosition, 4 * sizeof(float));
	lightBuffer.lightIntensity[lightBuffer.nLights] = light.intensity();
}


void Renderer::_formatSpotLight(const LightComponent& light, const Coords3f& translation, const Coords3f& direction, RendererData::SubmitLightBuffer& lightBuffer) const
{
	float lightPosition[4] = { translation.x, translation.y, translation.z, 1.0f };
	float viewLightPosition[4] = {};
	multMatrixPoint(VIEW, lightPosition, viewLightPosition);

	float lightDirection[4] = { direction.x, direction.y, direction.z, 0.0f };
	float viewLightDirection[4] = {};
	multMatrixPoint(VIEW, lightDirection, viewLightDirection);

	lightBuffer.lightType[lightBuffer.nLights] = (unsigned int)LightComponent::LightType::SPOT;
	memcpy(&lightBuffer.lightPosition[4 * lightBuffer.nLights], viewLightPosition, 4 * sizeof(float));
	memcpy(&lightBuffer.lightDirection[4 * lightBuffer.nLights], viewLightDirection, 4 * sizeof(float));
	lightBuffer.lightIntensity[lightBuffer.nLights] = light.intensity();
	lightBuffer.lightCutOff[lightBuffer.nLights] = light.cutOff();
}


void Renderer::_submitLightData(const RendererData::SubmitLightBuffer& lightBuffer) const
{	
	glUniform1ui(_uniformLocator[RendererUniformLocations::N_LIGHTS], lightBuffer.nLights);
	glUniform1uiv(_uniformLocator[RendererUniformLocations::LIGHT_TYPE], lightBuffer.nLights, lightBuffer.lightType);
	glUniform4fv(_uniformLocator[RendererUniformLocations::LIGHT_POSITION], lightBuffer.nLights, lightBuffer.lightPosition);
	glUniform4fv(_uniformLocator[RendererUniformLocations::LIGHT_DIRECTION], lightBuffer.nLights, lightBuffer.lightDirection);
	glUniform1fv(_uniformLocator[RendererUniformLocations::LIGHT_INTENSITY], lightBuffer.nLights, lightBuffer.lightIntensity);
	glUniform1fv(_uniformLocator[RendererUniformLocations::LIGHT_CUTOFF], lightBuffer.nLights, lightBuffer.lightCutOff);
	glUniform1f(_uniformLocator[RendererUniformLocations::LIGHT_AMBIENT], lightBuffer.ambientCoefficient);
	glUniform1f(_uniformLocator[RendererUniformLocations::LIGHT_DIFFUSE], lightBuffer.diffuseCoefficient);
	glUniform1f(_uniformLocator[RendererUniformLocations::LIGHT_SPECULAR], lightBuffer.specularCoefficient);
	glUniform1f(_uniformLocator[RendererUniformLocations::LIGHT_DARK_TEXTURE], lightBuffer.darkTextureCoefficient);
}