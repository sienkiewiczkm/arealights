// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include <glad/glad.h>
#include <vector>

#include <iostream>
#include "Logging.hpp"

namespace fw
{

class IMesh
{
public:
    virtual ~IMesh() = default;
    virtual void destroy() = 0;
    virtual void render() const = 0;
};

template<typename VertexType>
class Mesh:
    public IMesh
{
public:
    Mesh();
    Mesh(
        const std::vector<VertexType> &vertices,
        const std::vector<GLuint> &indices,
        GLenum primitiveType = GL_TRIANGLES
    );

    Mesh(const Mesh<VertexType> &mesh) = delete;
    Mesh(Mesh<VertexType> &&mesh);

    virtual ~Mesh();

    virtual void destroy();
    virtual void render() const;

protected:
    GLenum _primitiveType;
    GLuint _vao, _vbo, _ebo;
    int _numElements;

    void createBuffers(
        const std::vector<VertexType> &vertices,
        const std::vector<GLuint> &indices
    );

    void destroyBuffers();
};

template <typename VertexType>
Mesh<VertexType>::Mesh():
    _numElements{0},
    _vao{0},
    _vbo{0},
    _ebo{0},
    _primitiveType{GL_TRIANGLES}
{
}

template <typename VertexType>
Mesh<VertexType>::Mesh(
    const std::vector<VertexType> &vertices,
    const std::vector<GLuint> &indices,
    GLenum primitiveType
):
    _numElements{0},
    _vao{0},
    _vbo{0},
    _ebo{0},
    _primitiveType{primitiveType}
{
    createBuffers(vertices, indices);
}

template <typename VertexType>
Mesh<VertexType>::Mesh(Mesh<VertexType> &&mesh) :
    _vao(std::move(mesh._vao)),
    _vbo(std::move(mesh._vbo)),
    _ebo(std::move(mesh._ebo)),
    _numElements(std::move(mesh._numElements)),
    _primitiveType{std::move(mesh._primitiveType)}
{
    mesh._vao = mesh._vbo = mesh._ebo = 0;
}

template <typename VertexType>
Mesh<VertexType>::~Mesh()
{
    destroy();
}

template <typename VertexType>
void Mesh<VertexType>::destroy()
{
    destroyBuffers();
}

template <typename VertexType>
void Mesh<VertexType>::render() const
{
    glBindVertexArray(_vao);
    glDrawElements(_primitiveType, _numElements, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

template <typename VertexType>
void Mesh<VertexType>::createBuffers(
    const std::vector<VertexType> &vertices,
    const std::vector<GLuint> &indices
)
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    LOG(DEBUG) << "Creating buffers (vao=" << _vao << " vbo=" << _vbo
        << " ebo=" << _ebo << ") for " << vertices.size() << " vertices and "
        << indices.size() << " indices";

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType),
        vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(GLuint),
        indices.data(), GL_STATIC_DRAW);

    VertexType::setupAttribPointers();

    glBindVertexArray(0);

    _numElements = indices.size();

}

template <typename VertexType>
void Mesh<VertexType>::destroyBuffers()
{
    LOG(DEBUG) << "Destroying buffers (vao=" << _vao << " vbo=" << _vbo
        << " ebo=" << _ebo << ").";

    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_ebo) glDeleteBuffers(1, &_ebo);
    if (_vao) glDeleteVertexArrays(1, &_vao);
    _vao = _vbo = _ebo = 0;
}

}
