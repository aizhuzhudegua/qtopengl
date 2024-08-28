#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QVector2D>
#include <QVector>

// 顶点数据
struct Vertex {
    // 位置向量
    QVector3D Position;
    // 法线向量
    QVector3D Normal;
    // 纹理坐标
    QVector2D TexCoords;
};

// 纹理
struct Texture {
    unsigned int id;
    QString type;
    QString path;
};

class Mesh
{
public:
    Mesh(QOpenGLFunctions_3_3_Core* glFuns,
         QVector<Vertex> vertices, QVector<unsigned int> indices,
         QVector<Texture> textures);
    /*
     * 网格数据
    */
    // 顶点数据
    QVector<Vertex> m_vertices;
    // 索引
    QVector<unsigned int> m_indices;
    // 材质(由纹理组成的材质漫反射或镜面光贴图)
    QVector<Texture> m_material;

    void draw(QOpenGLShaderProgram& shader);
private:// 成员函数
    void setupMesh();


private: // 成员变量
    /*  渲染数据  */
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;

    QOpenGLFunctions_3_3_Core* m_glFuns;
};

#endif // MESH_H
