// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "GroundTruth.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace arealights {

    GroundTruth::GroundTruth(std::shared_ptr<RenderHelper> renderHelper):
        _renderHelper{renderHelper}, _numRender{0} {
    }

    void GroundTruth::setCamera(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix) {
        _viewMatrix = viewMatrix;
        _projMatrix = projMatrix;
    }

    void GroundTruth::setLights(std::vector<PolygonalLight> lights) {
        _lights = lights;
    }

    void GroundTruth::init() {
        _shader = _renderHelper->makeSimpleShader(
                "../assets/GroundTruthGTR.vs.glsl",
                "../assets/GroundTruthGTR.fs.glsl"
        );
    }

    void GroundTruth::render() {
        if (_lights.size() == 0) {
            return;
        }

        // arealight corners
        std::vector<glm::vec3> arealightCorners;

        for (auto i = 0; i < 2; ++i) {
            for (auto j = 0; j < 2; ++j) {
                float x = (i == 0) ? j : (1-j);
                float y = i;

                x -= 0.5f;
                y -= 0.5f;

                glm::vec4 pointLightPosition{x, y, 0.0f, 1.0f};
                auto worldLightPosition = _lights[0].transformation * pointLightPosition;
                glm::vec3 lightViewPosition = _viewMatrix * worldLightPosition;

                arealightCorners.push_back(lightViewPosition);
            }
        }

        const int numSamplesPerFrame = 128;
        int numSamplesAlreadyCalculated = numSamplesPerFrame * _numRender;
        int totalSamplesAfterFrame = numSamplesAlreadyCalculated + numSamplesPerFrame;
        float sourceFactor = static_cast<float>(numSamplesPerFrame) / totalSamplesAfterFrame;

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
        glBlendColor(0, 0, 0, sourceFactor);

        glDisable(GL_DEPTH_TEST);

        _shader->use();

        _shader->setUniform("TargetTexture", 0);
        _shader->setUniform("NormalTexture", 1);
        _shader->setUniform("PositionTexture", 2);

        std::vector<glm::vec2> randomParametersForSamples;
        for (int i = 0; i < numSamplesPerFrame; ++i) {
            randomParametersForSamples.push_back(halton2d(numSamplesPerFrame*_numRender+i));
        }

        auto shaderId = _shader->getId();

        auto rpLoc = glGetUniformLocation(shaderId, "RandomParameters");
        glUniform2fv(rpLoc, randomParametersForSamples.size(), glm::value_ptr(randomParametersForSamples.front()));

        auto rectArealightLoc = glGetUniformLocation(shaderId, "RectangularArealight");
        glUniform3fv(rectArealightLoc, 4, glm::value_ptr(arealightCorners.front()));

        _renderHelper->drawFullScreenQuad();

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        ++_numRender;
    }

    float GroundTruth::halton(int index, float base) {
        // Details:
        //   https://en.wikipedia.org/wiki/Halton_sequence
        auto result = 0.0f;
        auto f = 1.0f;

        for (auto i = static_cast<float>(index); i > 0.0f; i = std::floorf(i/base))
        {
            f = f / base;
            result += f * fmodf(i, base);
        }

        return result;
    }

    glm::vec2 GroundTruth::halton2d(int offset) {
        return glm::vec2{
                halton(offset, 2.0f),
                halton(offset, 5.0f)
        };
    }

}
