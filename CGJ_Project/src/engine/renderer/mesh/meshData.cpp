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

void MeshData::setIsMirror(bool isMirror)
{
	_isMirror = isMirror;
}

void MeshData::setStencilEnabled(bool enabled)
{
	_stencil._enabled = enabled;
}

void MeshData::addStencilFunc(GLenum func, GLint ref, GLint mask)
{
	_stencil._func = func;
	_stencil._ref = ref;
	_stencil._mask = mask;
}

void MeshData::addStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	_stencil._fail = fail;
	_stencil._zfail = zfail;
	_stencil._zpass = zpass;
}