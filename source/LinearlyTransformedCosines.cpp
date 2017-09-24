// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "LinearlyTransformedCosines.hpp"
#include "Logging.hpp"
#include "computed/ltc_fit.hpp"

namespace arealights
{

LinearlyTransformedCosines::LinearlyTransformedCosines(
    std::shared_ptr<RenderHelper> renderHelper
): _renderHelper{renderHelper}
{
}

LinearlyTransformedCosines::~LinearlyTransformedCosines()
{
}

void LinearlyTransformedCosines::init()
{
    _ltcShader = _renderHelper->makeSimpleShader(
        "../assets/LTC.vs",
        "../assets/LTC.fs"
    );

    loadLookupTextures();
}

void LinearlyTransformedCosines::setCamera(const glm::mat4& viewMatrix)
{
    _viewMatrix = viewMatrix;
}

void LinearlyTransformedCosines::setLights(std::vector<PolygonalLight> lights)
{
    _lights = lights;
}

void LinearlyTransformedCosines::render()
{
    if (_lights.size() == 0)
    {
        return;
    }

    _ltcShader->use();

    _ltcShader->setUniform("TargetTexture", 0);
    _ltcShader->setUniform("NormalTexture", 1);
    _ltcShader->setUniform("PositionTexture", 2);
    _ltcShader->setUniform("LTCLookupA", 3);
    _ltcShader->setUniform("LTCLookupB", 4);

    _ltcShader->setUniform("viewMatrix", _viewMatrix);
    _ltcShader->setUniform("arealightTransform", _lights[0].transformation);

    _ltcLookupMatA->bind(3);
    _ltcLookupMatB->bind(4);

    _renderHelper->drawFullScreenQuad();
}

void LinearlyTransformedCosines::loadLookupTextures()
{
    unsigned int ltcMatA, ltcMatB;
    std::vector<float> textureDataA, textureDataB;

    glGenTextures(1, &ltcMatA);
    glGenTextures(1, &ltcMatB);

    textureDataA.resize(4 * ltc_fit::size * ltc_fit::size);
    textureDataB.resize(3 * ltc_fit::size * ltc_fit::size);

    for (auto theta = 0; theta < ltc_fit::size; ++theta)
    {
        for (auto alpha = 0; alpha < ltc_fit::size; ++alpha)
        {
            auto matIdx = ltc_fit::size * theta + alpha;
            auto baseA = 4 * matIdx;
            auto baseB = 3 * matIdx;

            const auto& mat = ltc_fit::tabMinv[matIdx];

            textureDataA[baseA+0] = mat[0][0];
            textureDataA[baseA+1] = mat[0][2];
            textureDataA[baseA+2] = mat[1][1];
            textureDataA[baseA+3] = mat[2][0];

            textureDataB[baseB+0] = mat[2][2];
            textureDataB[baseB+1] = ltc_fit::tabMagnitude[matIdx];
            textureDataB[baseB+2] = ltc_fit::tabFresnel[matIdx];
        }
    }

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, ltcMatA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ltc_fit::size, ltc_fit::size,
        0, GL_RGBA, GL_FLOAT, textureDataA.data());

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        LOG(ERROR) << err;
    }

    glBindTexture(GL_TEXTURE_2D, ltcMatB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ltc_fit::size, ltc_fit::size,
        0, GL_RGB, GL_FLOAT, textureDataB.data());

    while((err = glGetError()) != GL_NO_ERROR)
    {
        LOG(ERROR) << err;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    _ltcLookupMatA = std::make_unique<fw::Texture>(ltcMatA);
    _ltcLookupMatB = std::make_unique<fw::Texture>(ltcMatB);
}

}
