#ifndef __engine_scene_components_lensFlareComponent__
#define __engine_scene_components_lensFlareComponent__


#include "engine/renderer/rendererSettings.h"




struct LensFlareComponent
{
    struct FlareElement {
        Coords4f color = Coords4f();
        float distance = 0.0f;
        float size = 1.0f;
        unsigned int colorMapId = 0;
    };

    struct FlareData {
        unsigned int nElements = 0;
        LensFlareComponent::FlareElement elements[RendererSettings::MAX_LENS_FLARE_ELEMENTS] = {};
        float maxSize = 1.0f;
        float scale = 1.0f;
    };

private:
    LensFlareComponent::FlareData _flareData = FlareData();
    Coords3f _lightPosition = Coords3f();    // lens flares only support point lights
    bool _enabled = true;

public:
    LensFlareComponent() = delete;
    LensFlareComponent(const LensFlareComponent&) = default;
    LensFlareComponent(const char* lensFlarePath);
    ~LensFlareComponent() = default;

    inline const LensFlareComponent::FlareData& flareData() const                                   { return _flareData; }
    inline const LensFlareComponent::FlareElement& flareElement(unsigned int flareElementId) const  { return _flareData.elements[flareElementId]; }
    inline const Coords3f& lightPosition() const                                                    { return _lightPosition; }
    inline bool enabled() const                                                                     { return _enabled; }

    inline void setLightPosition(const Coords3f& lightPosition) { _lightPosition = lightPosition; }
    inline void setEnabled(bool enabled)                        { _enabled = enabled; }

private:
    inline operator FlareData&() { return _flareData; }

public:
    friend class Importer;
};


#endif // !__engine_scene_components_lensFlareComponent__