#ifndef __engine_scene_components_lightComponent__
#define __engine_scene_components_lightComponent__




struct LightComponent
{
public:
	enum class LightType
	{
		DIRECTIONAL = 1,
		POINT = 2,
		SPOT = 3
	};

private:
	LightComponent::LightType _lightType = LightComponent::LightType::DIRECTIONAL;
	float _intensity = 1.0f;
	float _cutOff = 0.0f;
	bool _enabled = true;
	bool _castShadows = true;

public:
	LightComponent() = delete;
	LightComponent(const LightComponent&) = default;
	LightComponent(LightComponent::LightType lightType, float intensity);
	LightComponent(LightComponent::LightType lightType, float intensity, float cutOff);
	~LightComponent() = default;

	inline const LightType& lightType() const	{ return _lightType; }
	inline float intensity() const				{ return _intensity; }
	inline float cutOff() const					{ return _cutOff; }
	inline bool castShadows() const				{ return _castShadows; }

	inline bool isEnabled()	const				 { return _enabled; }
	inline void setEnabled(bool enabled)		 { _enabled = enabled; }
	inline void setCastShadows(bool castShadows) { _castShadows = castShadows; }

};


#endif // !__engine_scene_components_lightComponent__