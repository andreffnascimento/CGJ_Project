#include "engine/renderer/renderer.h"

#include <unordered_map>

#include "engine/math/AVTmathLib.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::renderParticles(const Scene& scene) const
{
	_initParticleRendering();

	std::unordered_map<EntityHandle, ParticleGeneratorComponent>& particleGenerators = scene.getSceneComponents<ParticleGeneratorComponent>();
	for (auto& particleGeneratorIterator : particleGenerators)
	{
		const ParticleGeneratorComponent& particleGenerator = particleGeneratorIterator.second;
		if (!particleGenerator.enabled())
			continue;

		_submitParticleTextureData(particleGenerator);
		_renderParticleGenerator(particleGenerator);
	}
	
	_terminateParticleRendering();
}




void Renderer::_initParticleRendering() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
}


void Renderer::_terminateParticleRendering() const
{
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}


void Renderer::_submitParticleTextureData(const ParticleGeneratorComponent& particleGenerator) const
{
	const Texture& texture = particleGenerator.texture();
	glUniform1ui(_uniformLocator[RendererUniformLocations::N_TEXTURES], (unsigned int)texture.nTextures());
	glUniform1uiv(_uniformLocator[RendererUniformLocations::TEXTURE_IDS], RendererSettings::MAX_TEXTURES_PER_MESH, texture.textureIds());
}


void Renderer::_addToParticleInstanceBuffer(RendererData::SubmitInstanceBuffer& instanceBuffer, const ParticleGeneratorComponent::ParticleData particle) const
{
	loadIdentity(MODEL);
	translate(MODEL, particle.position.x, particle.position.y, particle.position.z);
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	computeNormalMatrix3x3();
	
	float color[4] = { particle.color.x, particle.color.y, particle.color.z, particle.life };

	memcpy(instanceBuffer.pvmMatrix[instanceBuffer.nInstances], mCompMatrix[PROJ_VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(instanceBuffer.vmMatrix[instanceBuffer.nInstances], mCompMatrix[VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(instanceBuffer.normalMatrix[instanceBuffer.nInstances], mNormal3x3, 3 * 3 * sizeof(float));
	memcpy(instanceBuffer.particleColor[instanceBuffer.nInstances], color, 4 * sizeof(float));
	instanceBuffer.nInstances++;
}


void Renderer::_renderParticleGenerator(const ParticleGeneratorComponent& particleGenerator) const
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (unsigned int)RendererSettings::RendererMode::PARTICLE_RENDERER);
	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
	instanceBuffer.usesParticles = true;
	
	unsigned int i = 0;
	while (i < particleGenerator.nParticles())
	{
		const ParticleGeneratorComponent::ParticleData particle = particleGenerator.particle(i);
		if (instanceBuffer.nInstances >= RendererSettings::MAX_INSTANCES_PER_SUBMISSION)
			_submitRenderableData(particleGenerator.meshData(), instanceBuffer);
	
			if (particle.life > 0.0f)
				_addToParticleInstanceBuffer(instanceBuffer, particle);
			i++;
	
		_submitRenderableData(particleGenerator.meshData(), instanceBuffer);
	}	
}