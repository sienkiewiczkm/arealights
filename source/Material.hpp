// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include <memory>
#include "framework/Texture.hpp"

/***
 * \brief Texture-based material. Helps with loading textures and binding them to current shader.
 */
class Material {
public:
    Material() = default;
    ~Material() = default;

    void setAlbedoTexture(std::string filePath);
    void setNormalTexture(std::string filePath);
    void setMetalnessTexture(std::string filePath);
    void setRoughnessTexture(std::string filePath);

    void setAlbedoTexture(std::shared_ptr<fw::Texture> texture) { _albedoTexture = texture; }
    void setNormalTexture(std::shared_ptr<fw::Texture> texture) { _normalTexture = texture; }
    void setMetalnessTexture(std::shared_ptr<fw::Texture> texture) { _metalnessTexture = texture; }
    void setRoughnessTexture(std::shared_ptr<fw::Texture> texture) { _roughnessTexture = texture; }

    void bind();

private:
    glm::vec3 _albedoConst, _normalConst;
    float _metalnessConst, _roughnessConst;

    std::shared_ptr<fw::Texture> _albedoTexture;
    std::shared_ptr<fw::Texture> _normalTexture;
    std::shared_ptr<fw::Texture> _metalnessTexture;
    std::shared_ptr<fw::Texture> _roughnessTexture;
};

