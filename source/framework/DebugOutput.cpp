#include "DebugOutput.hpp"
#include "Logging.hpp"

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                   const void *userParam)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    LOG(INFO) << "Debug message (" << id << "): " <<  message;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             LOG(INFO) << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   LOG(INFO) << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: LOG(INFO) << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     LOG(INFO) << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     LOG(INFO) << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           LOG(INFO) << "Source: Other"; break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               LOG(INFO) << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: LOG(INFO) << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  LOG(INFO) << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         LOG(INFO) << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         LOG(INFO) << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              LOG(INFO) << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          LOG(INFO) << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           LOG(INFO) << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               LOG(INFO) << "Type: Other"; break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         LOG(INFO) << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       LOG(INFO) << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          LOG(INFO) << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(INFO) << "Severity: notification"; break;
    }
}

