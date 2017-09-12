// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "TextureUtils.hpp"
#include <stb_image.h>
#include "Logging.hpp"

using namespace std;

namespace fw
{

static GLuint uploadTextureToGPU(
    unsigned char* image, int width, int height, int components
)
{
    GLenum componentsEnum;
    switch (components)
    {
    case 3:
        componentsEnum = GL_RGB;
        break;
    case 4:
        componentsEnum = GL_RGBA;
        break;
    default:
        throw string("Components amount currently not supported.");
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, componentsEnum, width, height, 0,
        componentsEnum, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

GLuint loadTextureFromFile(const string &filename)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, components;
    unsigned char *image = stbi_load(
        filename.c_str(), &width, &height, &components, 0
    );

    if (image == nullptr)
    {
        LOG(ERROR) << "Cannot load specified texture: " << filename;
        throw string("Cannot load specified texture: " + filename);
    }

    auto texture = uploadTextureToGPU(image, width, height, components);
    stbi_image_free(image);

    LOG(INFO) << "Texture \"" << filename << "\" has been loaded successfully.";

    return texture;
}

GLuint loadTextureFromMemory(std::istream& stream)
{
    if (stream.fail())
    {
        return 0;
    }

    stream.seekg(0, stream.end);
    auto length = stream.tellg();
    stream.seekg(0, stream.beg);

    std::vector<unsigned char> buffer(length);
    stream.read(reinterpret_cast<char*>(buffer.data()), length);

    stbi_set_flip_vertically_on_load(true);

    int width, height, components;
    auto image = stbi_load_from_memory(
        buffer.data(), length, &width, &height, &components, 0
    );

    auto texture = uploadTextureToGPU(image, width, height, components);
    stbi_image_free(image);

    return texture;
}

}
