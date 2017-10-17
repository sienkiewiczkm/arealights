// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Vertices.hpp"

namespace fw
{

VertexColor::VertexColor()
{
}

VertexColor::VertexColor(
    glm::vec3 position,
    glm::vec3 color
):
    position{position},
    color{color}
{
}

void VertexColor::setupAttribPointers()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(VertexColor),
        (GLvoid*)0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(VertexColor),
        (GLvoid*)offsetof(VertexColor, color)
    );
}

StandardVertex2D::StandardVertex2D()
{
}

StandardVertex2D::StandardVertex2D(
    glm::vec2 position,
    glm::vec2 texCoord,
    glm::vec3 color
):
    position{position},
    texCoord{texCoord},
    color{color}
{
}

void StandardVertex2D::setupAttribPointers()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(StandardVertex2D),
        (GLvoid*)0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(StandardVertex2D),
        (GLvoid*)offsetof(StandardVertex2D, texCoord)
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(StandardVertex2D),
        (GLvoid*)offsetof(StandardVertex2D, color)
    );
}

StandardVertex3D::StandardVertex3D()
{
}

StandardVertex3D::StandardVertex3D(
    glm::vec3 position,
    glm::vec2 texCoords,
    glm::vec3 normal,
    glm::vec3 tangent
):
    position{position},
    texCoords{texCoords},
    normal{normal},
    tangent{tangent}
{
}

void StandardVertex3D::setupAttribPointers()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(StandardVertex3D),
        (GLvoid*)0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(StandardVertex3D),
        (GLvoid*)offsetof(StandardVertex3D, texCoords)
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(StandardVertex3D),
        (GLvoid*)offsetof(StandardVertex3D, normal)
    );

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(StandardVertex3D),
        (GLvoid*)offsetof(StandardVertex3D, tangent)
    );
}

VertexNormalTexCoords::VertexNormalTexCoords()
{
}

VertexNormalTexCoords::VertexNormalTexCoords(
    glm::vec3 position,
    glm::vec3 normal,
    glm::vec2 texCoords
) : position(position), normal(normal), texCoords(texCoords)
{
}

void VertexNormalTexCoords::setupAttribPointers()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexNormalTexCoords), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexNormalTexCoords),
        (GLvoid*)offsetof(VertexNormalTexCoords, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(VertexNormalTexCoords),
        (GLvoid*)offsetof(VertexNormalTexCoords, texCoords));
}

Vertex3D2TexCoord::Vertex3D2TexCoord()
{
}

Vertex3D2TexCoord::Vertex3D2TexCoord(
    glm::vec3 position,
    glm::vec2 texCoord1,
    glm::vec2 texCoord2
) :
    position(position),
    texCoord1(texCoord1),
    texCoord2(texCoord2)
{
}

void Vertex3D2TexCoord::setupAttribPointers()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3D2TexCoord),
        reinterpret_cast<GLvoid*>(0)
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3D2TexCoord),
        reinterpret_cast<GLvoid*>(offsetof(Vertex3D2TexCoord, texCoord1))
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3D2TexCoord),
        reinterpret_cast<GLvoid*>(offsetof(Vertex3D2TexCoord, texCoord2))
    );
}

Vertex3DNormal2TexCoord::Vertex3DNormal2TexCoord()
{
}

Vertex3DNormal2TexCoord::Vertex3DNormal2TexCoord(
    glm::vec3 position,
    glm::vec3 normal,
    glm::vec2 texCoord1,
    glm::vec2 texCoord2
) :
    position(position),
    normal(normal),
    texCoord1(texCoord1),
    texCoord2(texCoord2)
{
}

void Vertex3DNormal2TexCoord::setupAttribPointers()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3DNormal2TexCoord),
        reinterpret_cast<GLvoid*>(0)
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3DNormal2TexCoord),
        reinterpret_cast<GLvoid*>(offsetof(Vertex3DNormal2TexCoord, normal))
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3DNormal2TexCoord),
        reinterpret_cast<GLvoid*>(offsetof(Vertex3DNormal2TexCoord, texCoord1))
    );

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3DNormal2TexCoord),
        reinterpret_cast<GLvoid*>(offsetof(Vertex3DNormal2TexCoord, texCoord2))
    );
}

}
