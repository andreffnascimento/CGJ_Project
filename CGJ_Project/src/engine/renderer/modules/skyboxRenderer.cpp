#include "engine/renderer/renderer.h"

#include "engine/math/AVTmathLib.h"


extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];




void Renderer::_renderSkybox() const
{
	if (_skybox.skybox == nullptr || !_skybox.skybox->enabled())
		return;

	_initSkyboxRendering();
	_submitSkyboxMeshData();
	_submitSkyboxTransform();
	_submitSkyboxMeshInstance();
	_terminateSkyboxRendering();
}




void Renderer::_initSkyboxRendering() const
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (unsigned int)RendererSettings::RendererMode::SKYBOX_RENDERER);
	glDepthMask(GL_FALSE);
	glFrontFace(GL_CW);
}


void Renderer::_terminateSkyboxRendering() const
{
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CCW);
}


void Renderer::_submitSkyboxMeshData() const
{
	const Material& material = _skybox.skybox->meshData().material();
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_AMBIENT], 1, material.ambient);

	const Texture& texture = _skybox.skybox->meshData().texture();
	glUniform1ui(_uniformLocator[RendererUniformLocations::TEXTURE_N_COLOR_MAPS], (unsigned int)texture.nColorMaps());
	glUniform1uiv(_uniformLocator[RendererUniformLocations::TEXTURE_COLOR_MAP_IDS], RendererSettings::MAX_TEXTURES_PER_MESH, texture.colorMapIds());
}


void Renderer::_submitSkyboxTransform() const
{
	loadIdentity(MODEL);
	loadMatrix(MODEL, _skybox.transform->transformMatrix());
	
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(_uniformLocator[RendererUniformLocations::SKYBOX_MODEL_MATRIX], 1, GL_FALSE, mMatrix[MODEL]);
	glUniformMatrix4fv(_uniformLocator[RendererUniformLocations::INSTANCE_PVM_MATRIX], 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
}


void Renderer::_submitSkyboxMeshInstance() const
{
	const MyMesh& meshData = _skybox.skybox->meshData().mesh();
	glBindVertexArray(meshData.vao);
	glDrawElements(meshData.type, meshData.numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}