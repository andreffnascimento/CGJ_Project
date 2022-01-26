#ifndef __engine_scene_components_lensFlareComponent__
#define __engine_scene_components_lensFlareComponent__


#define MAXELEMENTSFLARE 10
#define NTEXTURES 4

struct LensFlareComponent
{
    struct FlareElement {
        float			fDistance;        // Distance along ray from source (0.0-1.0)
        float           fSize;            // Size relative to flare envelope (0.0-1.0)
        float			matDiffuse[4];  // color
        int				textureId;
    };

    struct Flare {
        float           fScale;     // Scale factor for adjusting overall size of flare elements.
        float           fMaxSize;   // Max size of largest element, as proportion of screen width (0.0-1.0)
        int             nPieces;    // Number of elements in use
        FlareElement    element[MAXELEMENTSFLARE];
    };

private:
    const LensFlareComponent::Flare _flare;

public:
    LensFlareComponent() = delete;
    ~LensFlareComponent() = default;

    inline const LensFlareComponent::Flare flare() const { return _flare; }

    void loadFlareFile(Flare* flare, char* filename);
};

#endif // !__engine_scene_components_lensFlareComponent__

