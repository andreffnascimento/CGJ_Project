#ifndef __engine_scene_components_canvasComponent__
#define __engine_scene_components_canvasComponent__


#include <string>
#include <unordered_map>

#include "engine/scene/components/textComponent.h"
#include "engine/scene/components/imageComponent.h"

class Entity;




struct CanvasComponent
{

private:
	std::unordered_map<const TextComponent*, const TransformComponent*> _canvasText = std::unordered_map<const TextComponent*, const TransformComponent*>();
	std::unordered_map<const ImageComponent*, const TransformComponent*> _canvasImage = std::unordered_map<const ImageComponent*, const TransformComponent*>();
	bool _enabled = true;

public:
	CanvasComponent() = default;
	CanvasComponent(const CanvasComponent&) = default;
	~CanvasComponent() = default;

	inline const std::unordered_map<const TextComponent*, const TransformComponent*>& canvasText() const	{ return _canvasText; }
	inline const std::unordered_map<const ImageComponent*, const TransformComponent*>& canvasImage() const	{ return _canvasImage; }
	inline bool enabled() const { return _enabled; }

	inline void setEnabled(bool enabled) { _enabled = enabled; }

	Entity createTextEntity(Scene* scene, const Entity& canvasEntity, const std::string& tag);
	Entity createTextEntity(Scene* scene, const Entity& canvasEntity);

	Entity createImageEntity(Scene* scene, const Entity& canvasEntity, const std::string& tag);
	Entity createImageEntity(Scene* scene, const Entity& canvasEntity);

};


#endif // !__engine_scene_components_canvasComponent__