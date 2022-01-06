#ifndef __engine_scene_components_cameraComponent__
#define __engine_scene_components_cameraComponent__


#include "engine/utils/coords.h"




struct CameraComponent
{
public:
	enum class CameraProjection
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

private:
	CameraProjection _cameraProjection = CameraProjection::ORTHOGRAPHIC;
	ClippingPlanes _clippingPlanes = { -1.0f, 1.0f };
	Coords3f _targetCoords = { 0.0f, 0.0f, 0.0f };
	Rectf _viewportRect = { -97.7f, 97.7f, -55.0f, 55.0f };
	float _fov = 60.0f;

public:
	CameraComponent() = delete;
	CameraComponent(const CameraComponent&) = default;
	CameraComponent(const CameraComponent::CameraProjection& cameraProjection, const ClippingPlanes& clippingPlanes, float viewSize);
	~CameraComponent() = default;

	inline const CameraProjection& cameraProjection() const { return _cameraProjection; }
	inline const ClippingPlanes& clippingPlanes() const		{ return _clippingPlanes; }
	inline const Coords3f& targetCoords() const				{ return _targetCoords; }
	inline const Rectf& viewportRect() const				{ return _viewportRect; }
	inline float fov() const								{ return _fov; }

	inline void setTargetCoords(const Coords3f& targetCoords) { _targetCoords = targetCoords; }

private:
	void _setOrthographicCamera(float viewportSize);
	void _setPerspectiveCamera(float fov);

public:
	friend class Renderer;
};


#endif // !__engine_scene_components_cameraComponent__