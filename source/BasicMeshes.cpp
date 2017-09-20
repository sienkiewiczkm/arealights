// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "BasicMeshes.hpp"
#include "Common.hpp"

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

MeshInfo SphereGenerator::generate(
    float radius,
    int latitudeSubdivisions,
    int longtitudeSubdivisions
)
{
    std::vector<StandardVertex3D> vertices;
    std::vector<GLuint> indices;

    for (int latitude = 0;
         latitude < latitudeSubdivisions;
         ++latitude)
    {
        for (int longtitude = 0;
             longtitude < longtitudeSubdivisions;
             ++longtitude)
        {
            float theta = 2.0f * pif() *
                latitude / static_cast<float>(latitudeSubdivisions);
            float phi = pif() *
                longtitude / (static_cast<float>(longtitudeSubdivisions) - 1);

            float costheta = cosf(theta);
            float sintheta = sinf(theta);
            float cosphi = cosf(phi);
            float sinphi = sinf(phi);

            glm::vec3 position = glm::vec3(
                radius * costheta * sinphi,
                radius * sintheta * sinphi,
                radius * cosphi
            );

            vertices.push_back(StandardVertex3D(
                position,
                {},
                glm::normalize(position),
                {}
            ));
        }
    }


    for (int latitude = 0;
         latitude < latitudeSubdivisions;
         ++latitude)
    {
        for (int longtitude = 0;
             longtitude < longtitudeSubdivisions - 1;
             ++longtitude)
        {
            int baseIndex = latitude * longtitudeSubdivisions + longtitude;
            int neighbourIndex = ((latitude + 1) % latitudeSubdivisions)
                * longtitudeSubdivisions + longtitude;

            indices.push_back(baseIndex);
            indices.push_back(baseIndex+1);
            indices.push_back(neighbourIndex);

            indices.push_back(neighbourIndex);
            indices.push_back(baseIndex+1);
            indices.push_back(neighbourIndex+1);
        }
    }

    return MeshInfo(vertices, indices);
}

}

