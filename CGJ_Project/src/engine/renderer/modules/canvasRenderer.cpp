#include "engine/renderer/renderer.h"

#include <unordered_map>

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"
#include "engine/text/avtFreeType.h"




void Renderer::renderCanvas(const Scene& scene) const
{
	_initCanvasRendering();

	const std::unordered_map<EntityHandle, CanvasComponent>& canvasComponents = scene.getSceneComponents<CanvasComponent>();
	_renderTextInstances(canvasComponents);

	_terminateCanvasRendering();
}




void Renderer::_initCanvasRendering() const
{
	int viewport[4] = {};
	glGetIntegerv(GL_VIEWPORT, viewport);
	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	ortho((float)viewport[0], viewport[0] + viewport[2] - 1.0f, (float)viewport[1], viewport[1] + viewport[3] - 1.0f, -1.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Renderer::_terminateCanvasRendering() const
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	popMatrix(PROJECTION);
}


void Renderer::_renderTextInstances(const std::unordered_map<EntityHandle, CanvasComponent>& canvasComponents) const
{
	glUseProgram(_textShader.getProgramIndex());
	for (const auto& canvasIterator : canvasComponents)
	{
		const CanvasComponent& canvas = canvasIterator.second;
		if (!canvas.enabled())
			continue;

		for (const auto& canvasTextIterator : canvas.canvasText())
		{
			const TextComponent& text = *canvasTextIterator.first;
			const Coords3f& translation = canvasTextIterator.second->translation();
			RenderText(_textShader, text, translation.x, translation.y, text.size(), text.color().x, text.color().y, text.color().z);
		}
	}
}