// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

struct GLFWwindow;

namespace fw
{

namespace ImGuiBinding
{

    bool initialize(GLFWwindow *window, bool setGLFWCallbacks);
    void shutdown();
    bool newFrame();

    bool createDeviceObjects();
    void invalidateDeviceObjects();

    void mouseButtonCallback(
        GLFWwindow *window,
        int button,
        int action,
        int mods
    );

    void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    void keyCallback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods
    );

    void charCallback(GLFWwindow *window, unsigned int c);

}

}
