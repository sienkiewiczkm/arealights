// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include <vector>
#include "glm/glm.hpp"
#include "OpenGL.hpp"
#include "Vertices.hpp"

namespace fw
{

struct MeshInfo
{
public:
    MeshInfo(std::vector<StandardVertex3D> vertices, std::vector<GLuint> indices);
    std::vector<StandardVertex3D> vertices;
    std::vector<GLuint> indices;
};

class PlaneGenerator
{
public:
    PlaneGenerator() {}
    virtual ~PlaneGenerator() {}
    static MeshInfo generate(glm::vec2 size);
};

class SphereGenerator
{
public:
    SphereGenerator() {}
    virtual ~SphereGenerator() {}
    static MeshInfo generate(
        float radius,
        int latitudeSubdivisions,
        int longtitudeSubdivisions
    );
};

}
