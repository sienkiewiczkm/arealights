// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Texture.hpp"

#include "stb_image.h"

#include "TextureUtils.hpp"
#include "common/StreamUtils.hpp"
#include "Logging.hpp"

namespace fw
{

Texture::Texture(GLuint textureId):
    _filepath{},
    _textureId{textureId}
{
}

Texture::Texture(
    std::istream& stream,
    const std::string& filepath
):
    _filepath{filepath}
{
    auto buffer = loadStream(stream);
    bool isHDR = stbi_is_hdr_from_memory(buffer.data(), buffer.size());

    stbi_set_flip_vertically_on_load(true);
    int width, height, components;

    if (isHDR)
    {
        LOG(INFO) << "Texture " << filepath << " is high dynamic range.";

        auto image = stbi_loadf_from_memory(
            buffer.data(), buffer.size(), &width, &height, &components, 0
        );

        loadTextureHDR(image, width, height, components);
        stbi_image_free(image);
    }
    else
    {
        auto image = stbi_load_from_memory(
            buffer.data(), buffer.size(), &width, &height, &components, 0
        );

        loadTextureLDR(image, width, height, components);
        stbi_image_free(image);
    }
}

Texture::Texture(const std::string& filepath)
{
    _filepath = filepath;
    _textureId = loadTextureFromFile(filepath);
}

Texture::~Texture()
{
    LOG(INFO) << "Texture id=" << _textureId << " released.";
}

const std::string& Texture::getFilePath() const
{
    return _filepath;
}

GLuint Texture::getTextureId() const
{
    return _textureId;
}

glm::ivec2 Texture::getSize(int mipLevel) const
{
    glBindTexture(GL_TEXTURE_2D, _textureId);

    int x, y;

    glGetTexLevelParameteriv(
        GL_TEXTURE_2D,
        mipLevel,
        GL_TEXTURE_WIDTH,
        &x
    );

    glGetTexLevelParameteriv(
        GL_TEXTURE_2D,
        mipLevel,
        GL_TEXTURE_HEIGHT,
        &y
    );

    glBindTexture(GL_TEXTURE_2D, 0);

    return {x,y};
}

void Texture::loadTextureLDR(
    unsigned char* image,
    int width,
    int height,
    int components
)
{
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        (components == 3 ? GL_RGB : GL_RGBA),
        width,
        height,
        0,
        (components == 3 ? GL_RGB : GL_RGBA),
        GL_UNSIGNED_BYTE,
        image
    );

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadTextureHDR(
    float* image,
    int width,
    int height,
    int components
)
{
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // todo: fix hardcoding
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB32F,
        width,
        height,
        0,
        GL_RGB,
        GL_FLOAT,
        image
    );

    glBindTexture(GL_TEXTURE_2D, 0);
}

}
