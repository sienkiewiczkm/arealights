// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "PolygonalLight.hpp"
#include "RenderHelper.hpp"
#include "framework/Shaders.hpp"
#include "framework/Texture.hpp"

namespace arealights
{

class LinearlyTransformedCosines
{
public:
    LinearlyTransformedCosines(std::shared_ptr<RenderHelper> renderHelper);
    ~LinearlyTransformedCosines();

    void init();

    void setCamera(const glm::mat4& viewMatrix);
    void setLights(std::vector<PolygonalLight> lights);
    void render();

    void setFlux(float flux) { _flux = flux; }
    float getFlux() { return _flux; }

private:
    void loadLookupTextures();

    std::shared_ptr<RenderHelper> _renderHelper;

    std::unique_ptr<fw::ShaderProgram> _ltcShader;

    std::unique_ptr<fw::Texture> _ltcLookupMatA;
    std::unique_ptr<fw::Texture> _ltcLookupMatB;

    std::vector<PolygonalLight> _lights;
    glm::mat4 _viewMatrix;

    float _flux;
};

}
