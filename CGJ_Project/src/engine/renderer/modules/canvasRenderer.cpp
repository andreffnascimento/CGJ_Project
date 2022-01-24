#include "engine/renderer/renderer.h"

#include <unordered_map>

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"
#include "engine/text/avtFreeType.h"




void Renderer::renderCanvas(const Scene& scene) const
{
	_initCanvasRendering();

	const std::unordered_map<EntityHandle, CanvasComponent>& canvasComponents = scene.getSceneComponents<CanvasComponent>();
	_renderImageInstances(canvasComponents);
	_renderTextInstances(canvasComponents);

	_terminateCanvasRendering();
}




void Renderer::_initCanvasRendering() const
{
	WindowCoords originalWindowSize = Application::getInstance().getOriginalWindowSize();

	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	loadIdentity(VIEW);
	ortho(0, (float)originalWindowSize.x, 0, (float)originalWindowSize.y, -1.0f, 1.0f);

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
	loadIdentity(MODEL);
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
			if (text.enabled())
				RenderText(_textShader, text, translation.x, translation.y, text.size(), text.color().x, text.color().y, text.color().z);
		}
	}
}


void Renderer::_renderImageInstances(const std::unordered_map<EntityHandle, CanvasComponent>& canvasComponents) const
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (unsigned int)RendererSettings::RendererMode::IMAGE_RENDERER);
	for (const auto& canvasIterator : canvasComponents)
	{
		const CanvasComponent& canvas = canvasIterator.second;
		if (!canvas.enabled())
			continue;

		auto canvasImageIterator = canvas.canvasImage().cbegin();
		while (canvasImageIterator != canvas.canvasImage().cend())
		{
			RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
			const ImageComponent* originalImage = canvasImageIterator->first;
			_submitMeshData(originalImage->meshData());

			while (canvasImageIterator != canvas.canvasImage().cend() && canvasImageIterator->first == originalImage)
			{ 
				if (instanceBuffer.nInstances >= RendererSettings::MAX_INSTANCES_PER_SUBMISSION)
					_submitRenderableData(originalImage->meshData(), instanceBuffer);

				const TransformComponent* transform = canvasImageIterator->second;
				if (originalImage->enabled())
					_addToInstanceBuffer(instanceBuffer, transform);
				canvasImageIterator++;
			}

			_submitRenderableData(originalImage->meshData(), instanceBuffer);
		}
	}
}