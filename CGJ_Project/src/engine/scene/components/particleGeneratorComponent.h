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
	MeshData _meshData = MeshData(createQuad(1.0f, 1.0f), Material());
	bool _enabled = false;


public:
	ParticleGeneratorComponent() = delete;
	ParticleGeneratorComponent(const ParticleGeneratorComponent&) = delete;
	ParticleGeneratorComponent(unsigned int nParticles);
	~ParticleGeneratorComponent();

public:
	inline unsigned int nParticles() const					{ return _nParticles;  }
	inline const ParticleData* particles() const			{ return _particles; }
	inline ParticleData& particle(unsigned int particle)	{ return _particles[particle]; }
	inline const MeshData& meshData() const					{ return _meshData; }
	inline const MyMesh& mesh()	const						{ return _meshData.mesh(); }
	inline const Texture& texture() const					{ return _meshData.texture(); }
	inline bool enabled() const								{ return _enabled; }

	inline void setTextureMode(const Texture::TextureMode& textureMode) { _meshData.setTextureMode(textureMode); }
	inline void addTexture(unsigned int textureId)						{ _meshData.addTexture(textureId); }
	inline void setEnabled(bool enabled)								{ _enabled = enabled; }
};


#endif // !__engine_scene_components_particleGeneratorComponent__