// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "glm/glm.hpp"
#include "../PolygonalLight.hpp"
#include "../RenderHelper.hpp"

namespace arealights {
    class GroundTruth {
    public:
        GroundTruth(std::shared_ptr<RenderHelper> renderHelper);
        ~GroundTruth() = default;

        void setCamera(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix);
        void setLights(std::vector<PolygonalLight> lights);
        void init();
        void render();

        void restartIncrementalRendering() { _numRender = 0; }

    protected:
        float halton(int index, float base);
        glm::vec2 halton2d(int offset);

    private:
        std::shared_ptr<RenderHelper> _renderHelper;
        std::unique_ptr<fw::ShaderProgram> _shader;
        std::vector<PolygonalLight> _lights;
        glm::mat4 _viewMatrix, _projMatrix;
        unsigned int _numRender;
    };
}

