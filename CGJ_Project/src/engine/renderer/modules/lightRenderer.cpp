#include "engine/renderer/renderer.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"




void Renderer::renderLights(const Scene& scene) const
{
	RendererData::SubmitLightData lightData = RendererData::SubmitLightData();

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
			_formatDirectionalLight(light, rotation.rotatePoint(Coords3f({ 0.0f, 0.0f, 1.0f })), lightData);
			break;

		case LightComponent::LightType::POINT:
			_formatPointLight(light, translation, lightData);
			break;

		case LightComponent::LightType::SPOT:
			_formatSpotLight(light, translation, rotation.rotatePoint(Coords3f({ 0.0f, 0.0f, 1.0f })), lightData);
			break;
		}

		lightData.nLights++;
	}

	lightData.ambientCoefficient = _reflectionCoefficients.ambient;
	lightData.diffuseCoefficient = _reflectionCoefficients.diffuse;
	lightData.specularCoefficient = _reflectionCoefficients.specular;
	lightData.darkTextureCoefficient = _reflectionCoefficients.darkTexture;
	_submitLightData(lightData);
}



void Renderer::_formatDirectionalLight(const LightComponent& light, const Coords3f& direction, RendererData::SubmitLightData& lightData) const
{
	float lightDirection[4] = { direction.x, direction.y, direction.z, 0.0f };
	float viewLightDirection[4] = {};
	multMatrixPoint(VIEW, lightDirection, viewLightDirection);

	lightData.lightType[lightData.nLights] = (unsigned int)LightComponent::LightType::DIRECTIONAL;
	memcpy(&lightData.lightDirection[4 * lightData.nLights], viewLightDirection, 4 * sizeof(float));
	lightData.lightIntensity[lightData.nLights] = light.intensity();
}


void Renderer::_formatPointLight(const LightComponent& light, const Coords3f& translation, RendererData::SubmitLightData& lightData) const
{
	float lightPosition[4] = { translation.x, translation.y, translation.z, 1.0f };
	float viewLightPosition[4] = {};
	multMatrixPoint(VIEW, lightPosition, viewLightPosition);

	lightData.lightType[lightData.nLights] = (unsigned int)LightComponent::LightType::POINT;
	memcpy(&lightData.lightPosition[4 * lightData.nLights], viewLightPosition, 4 * sizeof(float));
	lightData.lightIntensity[lightData.nLights] = light.intensity();
}


void Renderer::_formatSpotLight(const LightComponent& light, const Coords3f& translation, const Coords3f& direction, RendererData::SubmitLightData& lightData) const
{
	float lightPosition[4] = { translation.x, translation.y, translation.z, 1.0f };
	float viewLightPosition[4] = {};
	multMatrixPoint(VIEW, lightPosition, viewLightPosition);

	float lightDirection[4] = { direction.x, direction.y, direction.z, 0.0f };
	float viewLightDirection[4] = {};
	multMatrixPoint(VIEW, lightDirection, viewLightDirection);

	lightData.lightType[lightData.nLights] = (unsigned int)LightComponent::LightType::SPOT;
	memcpy(&lightData.lightPosition[4 * lightData.nLights], viewLightPosition, 4 * sizeof(float));
	memcpy(&lightData.lightDirection[4 * lightData.nLights], viewLightDirection, 4 * sizeof(float));
	lightData.lightIntensity[lightData.nLights] = light.intensity();
	lightData.lightCutOff[lightData.nLights] = light.cutOff();
}


void Renderer::_submitLightData(const RendererData::SubmitLightData& lightData) const
{	
	glUniform1ui(_uniformLocation[RendererData::ShaderUniformType::N_LIGHTS], lightData.nLights);
	glUniform1uiv(_uniformLocation[RendererData::ShaderUniformType::LIGHT_TYPE], lightData.nLights, lightData.lightType);
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::LIGHT_POSITION], lightData.nLights, lightData.lightPosition);
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::LIGHT_DIRECTION], lightData.nLights, lightData.lightDirection);
	glUniform1fv(_uniformLocation[RendererData::ShaderUniformType::LIGHT_INTENSITY], lightData.nLights, lightData.lightIntensity);
	glUniform1fv(_uniformLocation[RendererData::ShaderUniformType::LIGHT_CUTOFF], lightData.nLights, lightData.lightCutOff);
	glUniform1f(_uniformLocation[RendererData::ShaderUniformType::LIGHT_AMBIENT], lightData.ambientCoefficient);
	glUniform1f(_uniformLocation[RendererData::ShaderUniformType::LIGHT_DIFFUSE], lightData.diffuseCoefficient);
	glUniform1f(_uniformLocation[RendererData::ShaderUniformType::LIGHT_SPECULAR], lightData.specularCoefficient);
	glUniform1f(_uniformLocation[RendererData::ShaderUniformType::LIGHT_DARK_TEXTURE], lightData.darkTextureCoefficient);
}