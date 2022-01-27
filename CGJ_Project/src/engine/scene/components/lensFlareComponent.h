#ifndef __engine_scene_components_lensFlareComponent__
#define __engine_scene_components_lensFlareComponent__


#include "engine/renderer/rendererSettings.h"




struct LensFlareComponent
{
    struct FlareElement {
        Coords4f color = Coords4f();
        float distance = 0.0f;
        float size = 1.0f;
        unsigned int textureId = 0;
    };

    struct FlareData {
        unsigned int nElements = 0;
        LensFlareComponent::FlareElement elements[RendererSettings::MAX_FLARE_ELEMENTS];
        float maxElementSize = 1.0f;
        float scale = 1.0f;
    };

private:
    LensFlareComponent::FlareData _flareData = FlareData();

public:
    LensFlareComponent() = delete;
    LensFlareComponent(const LensFlareComponent&) = default;
    LensFlareComponent(const char* lensFlarePath);
    ~LensFlareComponent() = default;

    inline const LensFlareComponent::FlareData& flareData() const                                   { return _flareData; }
    inline const LensFlareComponent::FlareElement& flareElement(unsigned int flareElementId) const  { return _flareData.elements[flareElementId]; }

private:
    inline operator FlareData&() { return _flareData; }

public:
    friend class Importer;
};


#endif // !__engine_scene_components_lensFlareComponent__