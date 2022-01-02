#include "engine/renderer/renderer.h"

#include <GL/glew.h>







void Renderer::renderLights(const Scene& scene) const
{
	Renderer::LightData lightData = Renderer::LightData();

	const auto& lightComponents = scene.getSceneComponents<LightComponent>();
	if (lightComponents.size() > Renderer::MAX_LIGHTS)
		throw std::string("The renderer is not able to process all the game lights");

	for (const auto& iterator : lightComponents)
	{
		const LightComponent& light = iterator.second;
		const TransformComponent& transform = scene.getEntityById(iterator.first).getComponent<TransformComponent>();
		if (!light.isEnabled())
			continue;

		switch (light.lightType())
		{
		case LightComponent::LightType::DIRECTIONAL:
			_formatDirectionalLight(light, transform, lightData);
			break;

		case LightComponent::LightType::POINT:
			_formatPointLight(light, transform, lightData);
			break;

		case LightComponent::LightType::SPOT:
			_formatSpotLight(light, transform, lightData);
			break;
		}

		lightData.nLights++;
	}

	const ReflectionCoefficients& reflectionCoefficients = scene.reflectionCoefficients();
	lightData.ambientCoefficient = reflectionCoefficients.ambient;
	lightData.diffuseCoefficient = reflectionCoefficients.diffuse;
	lightData.specularCoefficient = reflectionCoefficients.specular;
	_submitLightData(lightData);
}




void Renderer::_formatDirectionalLight(const LightComponent& light, const TransformComponent& transform, Renderer::LightData& lightData) const
{
	float lightDirection[4] = { light.direction().x, light.direction().y, light.direction().z, 0.0f };
	float viewLightDirection[4] = {};
	multMatrixPoint(VIEW, lightDirection, viewLightDirection);

	lightData.lightTypes[lightData.nLights] = Renderer::DIRECTIONAL_LIGHT_TYPE;
	memcpy(&lightData.lightDirections[4 * lightData.nLights], viewLightDirection, 4 * sizeof(float));
	lightData.lightIntensities[lightData.nLights] = light.intensity();
}


void Renderer::_formatPointLight(const LightComponent& light, const TransformComponent& transform, Renderer::LightData& lightData) const
{
	float lightPosition[4] = { transform.translation().x, transform.translation().y, transform.translation().z, 1.0f };
	float viewLightPosition[4] = {};
	multMatrixPoint(VIEW, lightPosition, viewLightPosition);

	lightData.lightTypes[lightData.nLights] = Renderer::POINT_LIGHT_TYPE;
	memcpy(&lightData.lightPositions[4 * lightData.nLights], viewLightPosition, 4 * sizeof(float));
	lightData.lightIntensities[lightData.nLights] = light.intensity();
}


void Renderer::_formatSpotLight(const LightComponent& light, const TransformComponent& transform, Renderer::LightData& lightData) const
{
	float lightPosition[4] = { transform.translation().x, transform.translation().y, transform.translation().z, 1.0f };
	float viewLightPosition[4] = {};
	multMatrixPoint(VIEW, lightPosition, viewLightPosition);

	float lightDirection[4] = { light.direction().x, light.direction().y, light.direction().z, 0.0f };
	float viewLightDirection[4] = {};
	multMatrixPoint(VIEW, lightDirection, viewLightDirection);

	lightData.lightTypes[lightData.nLights] = Renderer::SPOT_LIGHT_TYPE;
	memcpy(&lightData.lightPositions[4 * lightData.nLights], viewLightPosition, 4 * sizeof(float));
	memcpy(&lightData.lightDirections[4 * lightData.nLights], viewLightDirection, 4 * sizeof(float));
	lightData.lightIntensities[lightData.nLights] = light.intensity();
	lightData.lightCutOffs[lightData.nLights] = light.cutOff();
}


void Renderer::_submitLightData(const Renderer::LightData& lightData) const
{
	GLint loc;
	constexpr const char* N_LIGHTS_ADDRESS = "lighting.nLights";
	constexpr const char* LIGHT_TYPES_ADDRESS = "lighting.lightTypes";
	constexpr const char* LIGHT_POSITIONS_ADDRESS = "lighting.lightPositions";
	constexpr const char* LIGHT_DIRECTIONS_ADDRESS = "lighting.lightDirections";
	constexpr const char* LIGHT_INTENSITIES_ADDRESS = "lighting.lightIntensities";
	constexpr const char* LIGHT_CUTOFFS_ADDRESS = "lighting.lightCutOffs";
	constexpr const char* LIGHT_AMBIENT_ADDRESS = "lighting.ambientCoefficient";
	constexpr const char* LIGHT_DIFFUSE_ADDRESS = "lighting.diffuseCoefficient";
	constexpr const char* LIGHT_SPECULAR_ADDRESS = "lighting.specularCoefficient";

	loc = glGetUniformLocation(_shader.getProgramIndex(), N_LIGHTS_ADDRESS);
	glUniform1ui(loc, lightData.nLights);

	loc = glGetUniformLocation(_shader.getProgramIndex(), LIGHT_TYPES_ADDRESS);
	glUniform1uiv(loc, lightData.nLights, lightData.lightTypes);

	loc = glGetUniformLocation(_shader.getProgramIndex(), LIGHT_POSITIONS_ADDRESS);
	glUniform4fv(loc, lightData.nLights, lightData.lightPositions);

	loc = glGetUniformLocation(_shader.getProgramIndex(), LIGHT_DIRECTIONS_ADDRESS);
	glUniform4fv(loc, lightData.nLights, lightData.lightDirections);

	loc = glGetUniformLocation(_shader.getProgramIndex(), LIGHT_INTENSITIES_ADDRESS);
	glUniform1fv(loc, lightData.nLights, lightData.lightIntensities);

	loc = glGetUniformLocation(_shader.getProgramIndex(), LIGHT_CUTOFFS_ADDRESS);
	glUniform1fv(loc, lightData.nLights, lightData.lightCutOffs);

	loc = glGetUniformLocation(_shader.getProgramIndex(), LIGHT_AMBIENT_ADDRESS);
	glUniform1f(loc, lightData.ambientCoefficient);

	loc = glGetUniformLocation(_shader.getProgramIndex(), LIGHT_DIFFUSE_ADDRESS);
	glUniform1f(loc, lightData.diffuseCoefficient);

	loc = glGetUniformLocation(_shader.getProgramIndex(), LIGHT_SPECULAR_ADDRESS);
	glUniform1f(loc, lightData.specularCoefficient);
}