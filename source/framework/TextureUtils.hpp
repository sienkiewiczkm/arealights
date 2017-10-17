// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include <glad/glad.h>
#include <string>

namespace fw
{

GLuint loadTextureFromFile(const std::string &filename);
GLuint loadTextureFromMemory(std::istream& stream);

}
