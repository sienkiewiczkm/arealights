// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "LinearlyTransformedCosines.hpp"
#include "../framework/Logging.hpp"

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
        "../assets/LTC.vs.glsl",
        "../assets/LTC.fs.glsl"
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

    _ltcShader->setUniform("GBufferC", 0);
    _ltcShader->setUniform("GBufferB", 1);
    _ltcShader->setUniform("GBufferA", 2);

    _ltcShader->setUniform("LTCLookupA", 3);
    _ltcShader->setUniform("LTCLookupB", 4);

    _ltcShader->setUniform("viewMatrix", _viewMatrix);
    _ltcShader->setUniform("arealightTransform", _lights[0].transformation);

    _ltcLookupMatA->bind(3);
    _ltcLookupMatB->bind(4);

    _renderHelper->drawFullScreenQuad();
}

struct alf_header
{
    char id[4];
    short version_major;
    short version_minor;
    unsigned int num_samples_angle;
    unsigned int num_samples_roughness;
};

struct alf_file
{
    alf_header header;
    std::vector<float> texture_slot1;
    std::vector<float> texture_slot2;
};

std::unique_ptr<alf_file> read_alf_file(const std::string &filename)
{
    auto file = fopen(filename.c_str(), "rb");
    if (!file) {
        return nullptr;
    }

    alf_header header;
    std::fread(&header, sizeof(alf_header), 1, file);

    if (std::strcmp(header.id, "ALF") != 0) {
        return nullptr;
    }

    auto num_samples = header.num_samples_roughness * header.num_samples_angle;
    if (num_samples <= 0) {
        return nullptr;
    }

    std::vector<float> texture_slot1;
    texture_slot1.resize(4 * header.num_samples_angle * header.num_samples_roughness);
    std::fread(&texture_slot1[0], sizeof(float)*4, num_samples, file);

    std::vector<float> texture_slot2;
    texture_slot2.resize(3 * header.num_samples_angle * header.num_samples_roughness);
    std::fread(&texture_slot2[0], sizeof(float)*3, num_samples, file);

    std::fclose(file);

    auto file_data = std::make_unique<alf_file>();
    file_data->header = std::move(header);
    file_data->texture_slot1 = std::move(texture_slot1);
    file_data->texture_slot2 = std::move(texture_slot2);
    return file_data;
}

void LinearlyTransformedCosines::loadLookupTextures()
{
    auto lookup_alf = read_alf_file("../assets/lookups/result8.ltc");

    if (!lookup_alf) {
        throw std::string("Cannot load lookup heitz-tables.ltc");
    }

    unsigned int ltcMatA, ltcMatB;
    glGenTextures(1, &ltcMatA);
    glGenTextures(1, &ltcMatB);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, ltcMatA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        lookup_alf->header.num_samples_roughness, lookup_alf->header.num_samples_angle,
        0, GL_RGBA, GL_FLOAT, lookup_alf->texture_slot1.data());

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        LOG(ERROR) << err;
    }

    glBindTexture(GL_TEXTURE_2D, ltcMatB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        lookup_alf->header.num_samples_roughness, lookup_alf->header.num_samples_angle,
        0, GL_RGB, GL_FLOAT, lookup_alf->texture_slot2.data());

    while((err = glGetError()) != GL_NO_ERROR)
    {
        LOG(ERROR) << err;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    _ltcLookupMatA = std::make_unique<fw::Texture>(ltcMatA);
    _ltcLookupMatB = std::make_unique<fw::Texture>(ltcMatB);
}

}
