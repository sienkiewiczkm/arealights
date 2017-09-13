// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Shaders.hpp"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>

#include "Common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Logging.hpp"

using namespace std;

namespace fw
{

Shader::Shader() : _shaderType(0), _shaderId(0) {
}

Shader::~Shader() {
  if (!_shaderId) {
    glDeleteShader(_shaderId);
    _shaderId = 0;
  }
}

void Shader::addSource(const string& source)
{
  _sources.push_back(source);
}

void Shader::addSourceFromFile(const string &filename)
{
    LOG(DEBUG) << "Loading shader source from file: " << filename;
    string shaderCode = loadASCIITextFile(filename);
    _sources.push_back(shaderCode);
    LOG(DEBUG) << "Shader loaded succesfully";
}

void Shader::compile(GLenum shaderType)
{
    vector<const char *> transformedSources;

    transform(
        _sources.begin(),
        _sources.end(),
        back_inserter(transformedSources),
        transformStringToCStr
    );

    GLuint shader = glCreateShader(shaderType);

    glShaderSource(
        shader,
        transformedSources.size(),
        transformedSources.data(),
        nullptr
    );

    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Error: Shader compilation failed" << std::endl
            << infoLog << std::endl;
        return; // todo: throw exception
    }

    _shaderType = shaderType;
    _shaderId = shader;
}

ShaderProgram::ShaderProgram() {
  _program = glCreateProgram();
}

ShaderProgram::ShaderProgram(const std::string& fileName)
{
    _program = glCreateProgram();
    /*
    Shader vs, fs;

    LOG(DEBUG) << "Loading Shabui file: \"" << fileName << "\"";

    sb::GLSLLoaderFileDependencyResolver dependencyResolver{fileName};
    sb::GLSLLoader loader{dependencyResolver};
    auto code = loader.loadFile(fileName);

    if (code.vertexShaderCode != "")
    {
        vs.addSource(code.vertexShaderCode);
        vs.compile(GL_VERTEX_SHADER);
        attach(&vs);
    }

    if (code.fragmentShaderCode != "")
    {
        fs.addSource(code.fragmentShaderCode);
        fs.compile(GL_FRAGMENT_SHADER);
        attach(&fs);
    }

    link();
    */
}

ShaderProgram::~ShaderProgram() {
  if (_program) {
    glDeleteProgram(_program);
    _program = 0;
  }
}

void ShaderProgram::attach(Shader *shader) {
  glAttachShader(_program, shader->getId());
}

void ShaderProgram::link() {
  glLinkProgram(_program);

  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(_program, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(_program, 512, NULL, infoLog);
      std::cout << "Error: Shader link" << std::endl << infoLog << std::endl;
  }
}

void ShaderProgram::use()
{
    glUseProgram(_program);
}

GLint ShaderProgram::getUniformLoc(const std::string& uniformName) const
{
    return glGetUniformLocation(_program, uniformName.c_str());
}

void ShaderProgram::setUniform(GLuint location, GLfloat v0)
{
    glUniform1f(location, v0);
}

void ShaderProgram::setUniform(GLuint location, GLint v0)
{
    glUniform1i(location, v0);
}

void ShaderProgram::setUniform(GLuint location, const glm::vec2& uniform)
{
    glUniform2fv(location, 1, glm::value_ptr(uniform));
}

void ShaderProgram::setUniform(GLuint location, const glm::vec3& uniform)
{
    glUniform3fv(location, 1, glm::value_ptr(uniform));
}

void ShaderProgram::setUniform(GLuint location, const glm::vec4& uniform)
{
    glUniform4fv(location, 1, glm::value_ptr(uniform));
}

void ShaderProgram::setUniform(GLuint location, const glm::mat4& uniform)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniform));
}

}
