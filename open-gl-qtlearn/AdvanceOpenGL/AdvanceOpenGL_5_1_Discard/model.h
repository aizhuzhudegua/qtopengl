#ifndef MODEL_H
#define MODEL_H
#include <QOpenGLTexture>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "mesh.h"



class Model
{
public:
    Model(QOpenGLFunctions_3_3_Core *glfuns,QString path);

    QVector<Texture> m_texturesLoaded;

    void draw(QOpenGLShaderProgram& shader);

    float m_maxX=-100.0;
    float m_maxY=-100.0;
    float m_minX=100.0;
    float m_minY=100.0;
private: // 成员函数
    // 将
    void loadModel(QString path);

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    QVector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, QString typeName);
    unsigned int getTextureIdFromFile(const char *path, const QString& directory);
private: // 成员变量
    QOpenGLFunctions_3_3_Core *m_glFuns;
    // 转换成要使用的mesh对象
    QVector<Mesh> m_meshes;
    // 目录
    QString m_directory;

};

#endif // MODEL_H
