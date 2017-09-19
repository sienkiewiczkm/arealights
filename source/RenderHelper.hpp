// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "OpenGLApplication.hpp"

namespace arealights
{

class RenderHelper
{
public:
    RenderHelper(fw::OpenGLApplication* glApplication);
    ~RenderHelper();

    void drawFullScreenQuad();

private:
    fw::OpenGLApplication *_glApplication;
    unsigned int _quadVAO, _quadVBO;
};

}
