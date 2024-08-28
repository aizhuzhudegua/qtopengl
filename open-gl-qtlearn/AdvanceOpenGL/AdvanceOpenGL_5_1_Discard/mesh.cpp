#include "mesh.h"
#include <QDebug>

Mesh::Mesh(QOpenGLFunctions_3_3_Core *glFuns,
           QVector<Vertex> vertices, QVector<unsigned int> indices,
           std::vector<Texture> material)
{
    m_glFuns = glFuns;
    this->m_vertices = vertices;
    this->m_indices = indices;
    this->m_material = material;
    setupMesh();
}


void Mesh::setupMesh()
{
    /*
    * GL_MAX_VERTEX_ATTRIBS 对应的值至少为16个属性值
    * 返回一个值，即顶点着色器可访问的 4 分量通用顶点属性的最大数量。该值必须至少为16
    *     GLint vertexAttributeCount;
    *     m_glFuns->glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexAttributeCount);
    *     qDebug() << "GL_MAX_VERTEX_ATTRIBS count = " << vertexAttributeCount;
    */


    // 创建VAO
    m_glFuns->glGenVertexArrays(1,&m_VAO);
    // 创建VBO
    m_glFuns->glGenBuffers(1,&m_VBO);
    // 创建EBO
    m_glFuns->glGenBuffers(1,&m_EBO);

    // 绑定VAO和VBO对象
    m_glFuns->glBindVertexArray(m_VAO);
    m_glFuns->glBindBuffer(GL_ARRAY_BUFFER,m_VBO);

    // 为缓冲对象创建一个新的数据存储
    m_glFuns->glBufferData(GL_ARRAY_BUFFER,m_vertices.size()*sizeof(Vertex), &m_vertices[0],GL_STATIC_DRAW);
    // 绑定到EBO
    m_glFuns->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_EBO);
    m_glFuns->glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_indices.size()*sizeof(GLuint),&m_indices[0],GL_STATIC_DRAW);
    /*
    * 定义一个通用顶点属性数据数组：告诉显卡如何解析缓冲里的属性值
    *   index:指定要修改的通用顶点属性的索引值
    *   size:指定通用顶点属性的分量，必须为1、2、3或4，初始值为4
    *   type:指定数组中每个元素得到类型，初始值为GL_FLOAT
    *   normalized:指定在访问定点数据值时是否应将其归一化 (GL_TRUE) 或直接转换为定点值 (GL_FALSE)
    *   stride:指定数组中每个通用顶点属性的步长
    *   pointer:指定通用顶点属性的属性的起始位置
    */
    m_glFuns->glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,8 * sizeof(GLfloat),(void*)0);
    // 开启VAO管理的第一个属性值
    m_glFuns->glEnableVertexAttribArray(0);
    m_glFuns->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Normal));
    // 开启VAO管理的第二个属性值
    m_glFuns->glEnableVertexAttribArray(1);
    m_glFuns->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,TexCoords));
    // 开启VAO管理的第三个属性值
    m_glFuns->glEnableVertexAttribArray(2);
}

void Mesh::draw(QOpenGLShaderProgram& shader)
{
    for(unsigned int index = 0; index < m_material.size(); index++) {
        m_glFuns->glActiveTexture(GL_TEXTURE0 + index);
        QString name = m_material[index].type;
        if(name == "texture_diffuse")
        {
            // 设置漫反射光照通道编号
            shader.setUniformValue("material.diffuse", index);
        }
        else if(name == "texture_specular")
        {
            // 设置镜面放射光照通道编号
            shader.setUniformValue("material.specular", index);
        }
        m_glFuns->glBindTexture(GL_TEXTURE_2D, m_material[index].id);

    }
    // 设置填充方式为铺满
    m_glFuns->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_glFuns->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_glFuns->glBindVertexArray(m_VAO);
    m_glFuns->glDrawElements(GL_TRIANGLES,m_vertices.size(),GL_UNSIGNED_INT,0);
}
