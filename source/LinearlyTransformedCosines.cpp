// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "LinearlyTransformedCosines.hpp"
#include "Logging.hpp"
#include "ltc.inc"

namespace arealights
{

LinearlyTransformedCosines::LinearlyTransformedCosines(
    std::shared_ptr<RenderHelper> renderHelper
): _renderHelper{renderHelper}, _ltcMat{}, _ltcMag{}
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
    _ltcShader->setUniform("ltc_mat", 3);
    _ltcShader->setUniform("ltc_mag", 4);

    _ltcShader->setUniform("viewMatrix", _viewMatrix);
    _ltcShader->setUniform("arealightTransform", _lights[0].transformation);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _ltcMat);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _ltcMag);

    _renderHelper->drawFullScreenQuad();
}

void LinearlyTransformedCosines::loadLookupTextures()
{
    glGenTextures(1, &_ltcMat);
    glGenTextures(1, &_ltcMag);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _ltcMat);

    std::vector<float> textureData;
    textureData.resize(4 * ltclookup::size * ltclookup::size);

    for (auto i = 0; i < ltclookup::size; ++i)
    {
        for (auto j = 0; j < ltclookup::size; ++j)
        {
            auto matIdx = ltclookup::size * j + i;
            auto base = 4 * matIdx;
            const auto& mat = ltclookup::tabM[matIdx];

            const auto& a = mat[0][0];
            const auto& b = mat[0][2];
            const auto& c = mat[1][1];
            const auto& d = mat[2][0];

            textureData[base+0] = a;
            textureData[base+1] = -b;
            textureData[base+2] = (a-b*d)/c;
            textureData[base+3] = -d;
        }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        ltclookup::size,
        ltclookup::size,
        0,
        GL_RGBA,
        GL_FLOAT,
        textureData.data()
    );

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        LOG(ERROR) << err;
    }

    glBindTexture(GL_TEXTURE_2D, _ltcMat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_R,
        ltclookup::size,
        ltclookup::size,
        0,
        GL_R,
        GL_FLOAT,
        ltclookup::tabAmplitude
    );
}

}
