#ifndef __engine_renderer_mesh_meshData__
#define __engine_renderer_mesh_meshData__


#include "engine/renderer/mesh/geometry.h"
#include "engine/renderer/mesh/texture.h"




class MeshData
{

private:
	MyMesh _mesh = MyMesh();
	Texture _texture = Texture();



public:
	MeshData() = default;
	MeshData(const MeshData& meshData) = default;
	MeshData(const MyMesh& mesh, const Material& material);
	~MeshData() = default;


public:
	inline const MyMesh& mesh() const { return _mesh; }
	inline const Material& material() const { return _mesh.mat; }
	inline const Texture& texture()	  const	{ return _texture ; }


public:
	void setMaterial(const Material& material);

public:
	inline void setTextureMode(const Texture::TextureMode& textureMode) { return _texture.setTextureMode(textureMode); }
	inline void addColorMap(unsigned int colorMapId)					{ return _texture.addColorMap(colorMapId); }
	inline void addNormalMap(unsigned int normalMapId)					{ return _texture.addNormalMap(normalMapId); }

};


#endif // !__engine_renderer_mesh_meshData__