// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "BasicMeshes.hpp"

namespace fw
{

MeshInfo::MeshInfo(
    std::vector<StandardVertex3D> vertices,
    std::vector<GLuint> indices
): vertices{vertices}, indices{indices} {
}

MeshInfo PlaneGenerator::generate(glm::vec2 size)
{
    std::vector<StandardVertex3D> vertices;

    glm::vec2 halfSize = 0.5f * size;

    for (int i = 0; i < 4; ++i)
    {
        int x = i % 2;
        int z = i / 2;

        int signX = x == 0 ? -1 : 1;
        int signZ = z == 0 ? -1 : 1;

        glm::vec3 position(signX * halfSize.x, 0.0f, signZ * halfSize.y);
        glm::vec2 texCoord(static_cast<float>(x), static_cast<float>(z));
        glm::vec3 normal{0.0f, 1.0f, 0.0f};
        glm::vec3 tangent{1.0f, 0.0f, 0.0f};

        auto vertex = StandardVertex3D{
            position,
            texCoord,
            normal,
            tangent
        };

        vertices.push_back(vertex);
    }

    std::vector<GLuint> indices = { 0, 1, 2, 1, 2, 3 };

    return MeshInfo{vertices, indices};
}

}

