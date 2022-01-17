#ifndef __engine_scene_components_canvasComponent__
#define __engine_scene_components_canvasComponent__


#include <unordered_map>

#include "engine/scene/components/textComponent.h"

class Entity;




struct CanvasComponent
{

private:
	std::unordered_map<const TextComponent*, const TransformComponent*> _canvasText = std::unordered_map<const TextComponent*, const TransformComponent*>();
	bool _enabled = true;

public:
	CanvasComponent() = default;
	CanvasComponent(const CanvasComponent&) = default;
	~CanvasComponent() = default;

	inline const std::unordered_map<const TextComponent*, const TransformComponent*>& canvasText() const { return _canvasText; }
	inline bool enabled() const { return _enabled; }

	inline void addText(const TextComponent* text, const TransformComponent* transform) { _canvasText[text] = transform; }
	inline void setEnabled(bool enabled) { _enabled = enabled; }

	void addText(Entity textEntity);

};


#endif // !__engine_scene_components_canvasComponent__