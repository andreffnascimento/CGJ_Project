#include "meshData.h"




MeshData::MeshData(const MyMesh& mesh, const Material& material)
	: _mesh(mesh)
{
	setMaterial(material);
}




void MeshData::setMaterial(const Material& material)
{
	Material& meshMaterial = _mesh.mat;
	memcpy(meshMaterial.ambient, material.ambient, 4 * sizeof(float));
	memcpy(meshMaterial.diffuse, material.diffuse, 4 * sizeof(float));
	memcpy(meshMaterial.specular, material.specular, 4 * sizeof(float));
	memcpy(meshMaterial.emissive, material.emissive, 4 * sizeof(float));
	meshMaterial.shininess = material.shininess;
}


void MeshData::setTextureMode(const Texture::TextureMode& textureMode)
{
	_texture._textureMode = textureMode;
}

void MeshData::addTexture(unsigned int textureId)
{
	_texture._textureIds[_texture._nTextures++] = textureId;
}

void MeshData::addNormal(unsigned int normalId) 
{
	_texture._normalIds[_texture._nNormals++] = normalId;
}