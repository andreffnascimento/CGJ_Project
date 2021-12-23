#include "engine/renderer/renderer.h"




void Renderer::renderLights(const Scene& scene) const
{
	_initializeLightRendering();

	size_t nLights = 0;
	size_t lightTypes[Renderer::MAX_LIGHTS] = {};
	Coords4f lightPositions[Renderer::MAX_LIGHTS] = {};
	Coords4f lightDirections[Renderer::MAX_LIGHTS] = {};
	float lightCutOffs[Renderer::MAX_LIGHTS] = {};

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
			_formatDirectionalLight(light, transform, nLights, lightTypes, lightDirections);
			break;

		case LightComponent::LightType::POINT:
			_formatPointLight(light, transform, nLights, lightTypes, lightPositions);
			break;

		case LightComponent::LightType::SPOT:
			_formatSpotLight(light, transform, nLights, lightTypes, lightPositions, lightDirections, lightCutOffs);
			break;
		}

		nLights++;
	}

	_submitLightData(nLights, lightTypes, lightPositions, lightDirections, lightCutOffs);
}




void Renderer::_initializeLightRendering() const
{
	float lightPos[4] = { 30.0f, 30.0f, 15.0f, 1.0f };
	float res[4];
	multMatrixPoint(VIEW, lightPos, res);
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::L_POS], 1, res);
}


void Renderer::_formatDirectionalLight(const LightComponent& light, const TransformComponent& transform, 
	size_t id, size_t types[], Coords4f directions[]) const
{
	types[id] = Renderer::DIRECTIONAL_LIGHT_TYPE;
	directions[id] = { light.direction().x, light.direction().y, light.direction().z, 0.0f };
}


void Renderer::_formatPointLight(const LightComponent& light, const TransformComponent& transform, 
	size_t id, size_t types[], Coords4f positions[]) const
{
	types[id] = Renderer::POINT_LIGHT_TYPE;
	positions[id] = { transform.translation().x, transform.translation().y, transform.translation().z, 1.0f };
}


void Renderer::_formatSpotLight(const LightComponent& light, const TransformComponent& transform, 
	size_t id, size_t types[], Coords4f positions[], Coords4f directions[], float spotCutOffs[]) const
{
	types[id] = Renderer::SPOT_LIGHT_TYPE;
	positions[id] = { transform.translation().x, transform.translation().y, transform.translation().z, 1.0f };
	directions[id] = { light.direction().x, light.direction().y, light.direction().z, 0.0f };
	spotCutOffs[id] = light.cutOff();
}


void Renderer::_submitLightData(size_t nLights, size_t types[], Coords4f positions[], Coords4f directions[], float spotCutOffs[]) const
{

}
