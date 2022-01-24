#ifndef __engine_renderer_mesh_meshData__
#define __engine_renderer_mesh_meshData__


#include "engine/renderer/mesh/geometry.h"
#include "engine/renderer/mesh/texture.h"




class MeshData
{

private:
	MyMesh _mesh;
	Texture _texture;




public:
	MeshData() = delete;
	MeshData(const MeshData& meshData) = default;
	MeshData(const MyMesh& mesh, const Material& material);
	~MeshData() = default;


public:
	inline const MyMesh& mesh() const		{ return _mesh; }
	inline const Material& material() const { return _mesh.mat; }
	inline const Texture& texture() const	{ return _texture; }


public:
	void setMaterial(const Material& material);

public:
	inline void setTextureMode(const Texture::TextureMode& textureMode) { return _texture.setTextureMode(textureMode); }
	inline void addTexture(unsigned int textureId)						{ return _texture.addTexture(textureId); }
	inline void addNormal(unsigned int normalId)						{ return _texture.addNormal(normalId); }

};


#endif // !__engine_renderer_mesh_meshData__