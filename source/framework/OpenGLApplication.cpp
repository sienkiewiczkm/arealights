// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "OpenGLApplication.hpp"
#include <string>
#include "Logging.hpp"
#include "DebugOutput.hpp"

using namespace std;

namespace fw
{

OpenGLApplication::OpenGLApplication():
    _windowSize{800, 600},
    _windowTitle{"Framework Application"}
{
}

OpenGLApplication::~OpenGLApplication()
{
    destroy();
}

void OpenGLApplication::create()
{
    if (!glfwInit())
    {
        LOG(FATAL) << "Cannot initialize GLFW3";
        throw string("Cannot initialize GLFW3.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    _window = glfwCreateWindow(
        _windowSize.x,
        _windowSize.y,
        _windowTitle.c_str(),
        nullptr,
        nullptr
    );

    if (!_window)
    {
        glfwTerminate();
        LOG(FATAL) << "Cannot create GLFW3 window";
        throw string("Cannot create GLFW3 window.");
    }

    LOG(INFO) << "GLFW3 window created successfully.";

    glfwMakeContextCurrent(_window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glfwSetWindowUserPointer(_window, static_cast<void*>(this));

    glfwSetMouseButtonCallback(_window, OpenGLApplication::mouseButtonCallback);
    glfwSetCursorPosCallback(_window, OpenGLApplication::mouseMoveCallback);
    glfwSetKeyCallback(_window, OpenGLApplication::keyCallback);
    glfwSetCharCallback(_window, OpenGLApplication::charCallback);
    glfwSetScrollCallback(_window, OpenGLApplication::scrollCallback);
    glfwSetWindowSizeCallback(_window, OpenGLApplication::windowSizeCallback);
    glfwSetFramebufferSizeCallback(
        _window,
        OpenGLApplication::framebufferSizeCallback
    );

    glfwGetCursorPos(
        _window,
        &_currentMousePosition.x,
        &_currentMousePosition.y
    );

    glfwGetFramebufferSize(_window, &_framebufferSize.x, &_framebufferSize.y);

    _previousMousePosition = _currentMousePosition;

    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    onCreate();
}

void OpenGLApplication::destroy()
{
    onDestroy();
    glfwTerminate();
    LOG(INFO) << "Window destroyed and GLFW3 terminated.";
}

void OpenGLApplication::run()
{
    _currentFrameStartTime = _previousFrameStartTime =
        std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
        updateFrameTimes();

        onUpdate(_currentFrameDeltaTime);
        onRender();

        glfwSwapBuffers(_window);
    }

    destroy();
}

void OpenGLApplication::onCreate()
{
}

void OpenGLApplication::onDestroy()
{
}

void OpenGLApplication::onUpdate(
    const std::chrono::high_resolution_clock::duration& deltaTime
)
{
}

void OpenGLApplication::onRender()
{
}

bool OpenGLApplication::onMouseButton(int button, int action, int mods)
{
    return false;
}

bool OpenGLApplication::onScroll(double xoffset, double yoffset)
{
    return false;
}

bool OpenGLApplication::onKey(int key, int scancode, int action, int mods)
{
    return false;
}

bool OpenGLApplication::onChar(unsigned int c)
{
    return false;
}

bool OpenGLApplication::onResize()
{
    return false;
}

bool OpenGLApplication::onFramebufferResize(glm::ivec2 framebufferSize)
{
    _framebufferSize = framebufferSize;
    return false;
}

bool OpenGLApplication::onMouseMove(glm::dvec2 newPosition)
{
    _previousMousePosition = _currentMousePosition;
    _currentMousePosition = newPosition;
    _mouseMovement = _currentMousePosition - _previousMousePosition;
    return false;
}

void OpenGLApplication::setWindowSize(const glm::ivec2& size)
{
    if (_windowSize != size && _window != nullptr)
    {
        _windowSize = size;
        glfwSetWindowSize(_window, _windowSize.x, _windowSize.y);
    }
}

const glm::ivec2& OpenGLApplication::getWindowSize() const
{
    return _windowSize;
}

const glm::ivec2& OpenGLApplication::getFramebufferSize() const
{
    return _framebufferSize;
}

void OpenGLApplication::setWindowTitle(const std::string& title)
{
    _windowTitle = title;
    if (_window != nullptr)
    {
        glfwSetWindowTitle(_window, _windowTitle.c_str());
    }
}

const std::string& OpenGLApplication::getTitle() const
{
    return _windowTitle;
}

glm::dvec2 OpenGLApplication::getCurrentMousePosition() const
{
    return _currentMousePosition;
}

glm::dvec2 OpenGLApplication::getPreviousMousePosition() const
{
    return _previousMousePosition;
}

glm::dvec2 OpenGLApplication::getMouseMovement() const
{
    return _mouseMovement;
}

void OpenGLApplication::mouseButtonCallback(
    GLFWwindow *window, int button, int action, int mods
)
{
    auto app = static_cast<OpenGLApplication*>(
        glfwGetWindowUserPointer(window)
    );

    app->onMouseButton(button, action, mods);
}

void OpenGLApplication::mouseMoveCallback(
    GLFWwindow *window,
    double xpos,
    double ypos
)
{
    auto app = static_cast<OpenGLApplication*>(
        glfwGetWindowUserPointer(window)
    );

    app->onMouseMove({xpos, ypos});
}

void OpenGLApplication::scrollCallback(
    GLFWwindow *window, double xoffset, double yoffset
)
{
    auto app = static_cast<OpenGLApplication*>(
        glfwGetWindowUserPointer(window)
    );

    app->onScroll(xoffset, yoffset);
}

void OpenGLApplication::keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods
)
{
    auto app = static_cast<OpenGLApplication*>(
        glfwGetWindowUserPointer(window)
    );

    app->onKey(key, scancode, action, mods);
}

void OpenGLApplication::charCallback(GLFWwindow *window, unsigned int c)
{
    auto app = static_cast<OpenGLApplication*>(
        glfwGetWindowUserPointer(window)
    );

    app->onChar(c);
}

void OpenGLApplication::windowSizeCallback(
    GLFWwindow *window,
    int width,
    int height
)
{
    auto app = static_cast<OpenGLApplication*>(
        glfwGetWindowUserPointer(window)
    );

    app->setWindowSize({width, height});
    app->onResize();
}

void OpenGLApplication::framebufferSizeCallback(
    GLFWwindow *window,
    int width,
    int height
)
{
    auto app = static_cast<OpenGLApplication*>(
        glfwGetWindowUserPointer(window)
    );

    app->onFramebufferResize({width, height});
}

void OpenGLApplication::updateFrameTimes()
{
    _previousFrameStartTime = _currentFrameStartTime;
    _currentFrameStartTime = std::chrono::high_resolution_clock::now();
    _currentFrameDeltaTime =
        _currentFrameStartTime - _previousFrameStartTime;
}

}
