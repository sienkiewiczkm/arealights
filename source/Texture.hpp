// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include <string>
#include "OpenGL.hpp"
#include "glm/glm.hpp"

namespace fw
{

class Texture
{
public:
    Texture(GLuint textureId);
    Texture(
        std::istream& stream,
        const std::string& filepath = ""
    );
    Texture(const std::string& filepath);
    virtual ~Texture();

    const std::string& getFilePath() const;
    GLuint getTextureId() const;

    glm::ivec2 getSize(int mipLevel = 0) const;

    void bind(int slot);

private:
    void loadTextureLDR(
        unsigned char* image,
        int width,
        int height,
        int components
    );

    void loadTextureHDR(
        float* image,
        int width,
        int height,
        int components
    );

    std::string _filepath;
    GLuint _textureId;
};

}
