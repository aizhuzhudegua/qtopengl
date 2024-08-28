#ifndef MESH_H
#define MESH_H
#include <QVector3D>
#include <QVector2D>
#include <QVector>

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "openglwidget.h"

struct Vertex {
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
    QVector3D Tangent;
    QVector3D Bitangent;
};
struct Texture {
    unsigned int id;
    QString type;
    aiString path;
};


class Mesh{
public:
/*  网格数据  */
   QVector<Vertex> vertices;
   QVector<unsigned int> indices;
   QVector<Texture> textures;
   /*  函数  */
   Mesh();
   Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<Texture> textures);
   void initialize();
   void Draw(QOpenGLShaderProgram &shader);
private:
   /*  渲染数据  */
   unsigned int VAO, VBO, EBO;
   /*  函数  */
   void setupMesh();

};

#endif // MESH_H
