// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Material.hpp"
#include "framework/Logging.hpp"

void Material::setAlbedoTexture(std::string filePath) {
    setAlbedoTexture(std::make_unique<fw::Texture>(filePath));
}

void Material::setNormalTexture(std::string filePath) {
    setNormalTexture(std::make_unique<fw::Texture>(filePath));
}

void Material::setMetalnessTexture(std::string filePath) {
    setMetalnessTexture(std::make_unique<fw::Texture>(filePath));
}

void Material::setRoughnessTexture(std::string filePath) {
    setRoughnessTexture(std::make_unique<fw::Texture>(filePath));

}
void Material::bind() {
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    if (currentProgram == 0) {
        LOG(INFO) << "Material cannot be activated. Active program could not be found.";
        return;
    }

    auto albedoLoc = glGetUniformLocation(currentProgram, "AlbedoTexture");
    auto normalLoc = glGetUniformLocation(currentProgram, "NormalTexture");
    auto metalLoc = glGetUniformLocation(currentProgram, "MetalnessTexture");
    auto roughLoc = glGetUniformLocation(currentProgram, "RoughnessTexture");

    if ((albedoLoc == -1) || (normalLoc == -1) || (metalLoc == -1) || (roughLoc == -1)) {
        LOG(ERROR) << "Material could not be bound. Required uniforms not found.";
        LOG(INFO) << "Albedo: " << albedoLoc << "Normal: " << normalLoc << "Metal: " << metalLoc
                  << "Roughness" << roughLoc;
        return;
    }

    glUniform1i(albedoLoc, 0);
    glUniform1i(normalLoc, 1);
    glUniform1i(metalLoc, 2);
    glUniform1i(roughLoc, 3);

    _albedoTexture->bind(0);
    _normalTexture->bind(1);
    _metalnessTexture->bind(2);
    _roughnessTexture->bind(3);
}


