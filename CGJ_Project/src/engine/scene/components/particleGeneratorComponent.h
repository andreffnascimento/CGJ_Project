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
		Coords3f size = Coords3f();
		float life = 1.0f;
		float fadeSpeed = 0.1f;
	};

private:
	unsigned int _nParticles = 0;
	ParticleGeneratorComponent::ParticleData* _particles = nullptr;
	MeshData _meshData = MeshData();
	bool _enabled = false;

public:
	ParticleGeneratorComponent() = delete;
	ParticleGeneratorComponent(const ParticleGeneratorComponent&) = default;
	ParticleGeneratorComponent(float size, unsigned int textureId);
	~ParticleGeneratorComponent();

public:
	void initializeParticleStorage(unsigned int nParticles);

	inline unsigned int nParticles() const								{ return _nParticles;  }
	inline const ParticleData* particles() const						{ return _particles; }
	inline const ParticleData& particle(unsigned int particle)	const	{ return _particles[particle]; }
	inline const MeshData& meshData() const								{ return _meshData; }
	inline const MyMesh& mesh()	const									{ return _meshData.mesh(); }
	inline const Texture& texture() const								{ return _meshData.texture(); }
	inline bool enabled() const											{ return _enabled; }

	inline ParticleData& particle(unsigned int particle)				{ return _particles[particle]; }

	inline void setTextureMode(const Texture::TextureMode& textureMode) { _meshData.setTextureMode(textureMode); }
	inline void addColorMap(unsigned int textureId)						{ _meshData.addColorMap(textureId); }
	inline void setEnabled(bool enabled)								{ _enabled = enabled; }
};


#endif // !__engine_scene_components_particleGeneratorComponent__