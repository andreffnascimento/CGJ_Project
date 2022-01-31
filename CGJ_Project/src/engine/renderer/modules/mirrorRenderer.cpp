#include "engine/renderer/renderer.h"

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];


void Renderer::_renderFixedMirrors(const Scene& scene)
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (unsigned int)RendererSettings::RendererMode::MESH_RENDERER);
	for (auto& flatMirrorsIterator : scene.getSceneComponents<FlatMirrorComponent>())
	{
		const FlatMirrorComponent& flatMirror = flatMirrorsIterator.second;
		if (!flatMirror.enabled())
			continue;

		_renderMirrorShape(flatMirror);
		_renderMirrorView(scene, flatMirror);
	}
}




void Renderer::_initMirrorShape(const WindowCoords& windowSize) const
{
	glClearStencil(0x0);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0x1);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	pushMatrix(VIEW);
	loadIdentity(VIEW);
	ortho(0, (float)windowSize.x, 0, (float)windowSize.y, -1.0f, 1.0f);
}


void Renderer::_initMirrorCamera(const FlatMirrorComponent& flatMirror)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glStencilFunc(GL_EQUAL, 1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	WindowCoords windowSize = Application::getInstance().getOriginalWindowSize();
	const TransformComponent& mirrorTransform = flatMirror.transform();
	const CameraEntity& mirrorCamera = flatMirror.camera();
	const CameraComponent& cameraComponent = mirrorCamera.getComponent<CameraComponent>();

	loadIdentity(PROJECTION);
	float ratio = (float)windowSize.x / (float)windowSize.y;
	perspective(cameraComponent.fov(), ratio, cameraComponent.clippingPlanes().near, cameraComponent.clippingPlanes().far);
	_renderCamera(mirrorCamera);
}


void Renderer::_terminateMirrorRendering()
{
	glDisable(GL_STENCIL_TEST);
	popMatrix(PROJECTION);
	popMatrix(VIEW);
}



void Renderer::_renderMirrorShape(const FlatMirrorComponent& flatMirror) const
{
	WindowCoords windowSize = Application::getInstance().getOriginalWindowSize();
	_initMirrorShape(windowSize);

	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
	_submitMeshData(flatMirror.meshData());
	_addObjectToInstanceBuffer(instanceBuffer, &flatMirror.transform());
	_submitRenderableData(flatMirror.meshData(), instanceBuffer);
}


void Renderer::_renderMirrorView(const Scene& scene, const FlatMirrorComponent& flatMirror)
{
	_initMirrorCamera(flatMirror);
	_renderSkybox();
	_renderImages(scene);
	_enableTableTop(scene); // Yikes, I know...
	_renderModels(scene, RendererSettings::RendererMode::MESH_RENDERER);
	_renderMeshes(scene, RendererSettings::RendererMode::MESH_RENDERER);
	_renderColliders(scene);
	_renderParticles(scene);
	_terminateMirrorRendering();
}

void Renderer::_enableTableTop(const Scene& scene)
{
	Entity table = scene.getEntityByTag("Table:top");
	table.getComponent<MeshComponent>().setEnabled(true);
}

void Renderer::_disableTableTop(const Scene& scene)
{
	Entity table = scene.getEntityByTag("Table:top");
	table.getComponent<MeshComponent>().setEnabled(false);
}