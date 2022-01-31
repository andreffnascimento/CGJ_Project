#ifndef __engine_scene_components_modelComponent__
#define __engine_scene_components_modelComponent__


#include <vector>

#include "engine/scene/components/transformComponent.h"

#include "engine/math/transform.h"
#include "engine/renderer/mesh/geometry.h"
#include "engine/renderer/mesh/meshFromAssimp.h"
#include "engine/utils/coords.h"




struct ModelComponent
{
private:
	std::vector<struct MyMesh> _meshes = std::vector<struct MyMesh>();

	TransformComponent _modelTransform = TransformComponent();

public:
	ModelComponent() = delete;
	ModelComponent(const ModelComponent&) = default;
	ModelComponent(const char* modelPath);
	~ModelComponent() = default;

	inline const std::vector<struct MyMesh>& meshes() const		{ return _meshes; }
	inline const TransformComponent& modelTransform() const		{ return _modelTransform; }

	inline TransformComponent& modelTransform() { return _modelTransform; }

public:
	friend class Importer;
};


#endif // !__engine_scene_components_modelComponent__