#ifndef __engine_scene_components_particleGeneratorComponent__
#define __engine_scene_components_particleGeneratorComponent__


#include "engine/utils/coords.h"
#include "engine/renderer/mesh/texture.h"




struct ParticleGeneratorComponent
{
public:
	struct ParticleData
	{
		Coords3f position = Coords3f();
		Coords3f velocity = Coords3f();
		Coords3f acceleration = Coords3f();
		Coords3f color = Coords3f();
		float life;
		float fadeSpeed;
	};

private:
	unsigned int _nParticles = 0;
	ParticleData* _particles = nullptr;
	Texture _texture = Texture();


public:
	ParticleGeneratorComponent() = delete;
	ParticleGeneratorComponent(const ParticleGeneratorComponent&) = delete;
	ParticleGeneratorComponent(unsigned int nParticles);
	~ParticleGeneratorComponent();

public:
	inline const ParticleData* particles() const { return _particles; }
	inline ParticleData& particle(unsigned int particle) { return _particles[particle]; }
	inline const Texture& texture() const { return _texture; }

	inline void setTextureMode(const Texture::TextureMode& textureMode) { _texture.setTextureMode(textureMode); }
	inline void addTexture(unsigned int textureId) { _texture.addTexture(textureId); }
};


#endif // !__engine_scene_components_particleGeneratorComponent__