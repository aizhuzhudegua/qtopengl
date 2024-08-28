#include "model.h"
#include <QDir>
#include <QDebug>

Model::Model(QOpenGLFunctions_3_3_Core* glfuns,QString path):
    m_glFuns(glfuns)
{
    loadModel(path);
}

void Model::draw(QOpenGLShaderProgram& shader)
{
    for(unsigned int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].draw(shader);
    }
}

void Model::loadModel(QString path)
{
    Assimp::Importer import;
    // 将模型文件加载到scene
    const aiScene *scene = import.ReadFile(path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << "ERROR::ASSIMP::" << import.GetErrorString();
        return;
    }
    // 获取模型所在目录
    m_directory = QString(path.toStdString().substr(0, path.toStdString().find_last_of('/')).c_str());
    // 处理节点
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // 处理节点的所有mesh
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // 处理节点中的子节点
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    std::vector<Texture> textures;

    // 处理顶点位置
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        if(m_maxX<mesh->mVertices[i].x)
            m_maxX=mesh->mVertices[i].x;
        if(m_maxY<mesh->mVertices[i].y)
            m_maxY=mesh->mVertices[i].y;
        if(m_minX>mesh->mVertices[i].x)
            m_minX=mesh->mVertices[i].x;
        if(m_minY>mesh->mVertices[i].y)
            m_minY=mesh->mVertices[i].y;

        Vertex vertex;
        // 处理顶点位置
        QVector3D vector;
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.Position = vector;

        // 处理法线
        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vertex.Normal = vector;

        // 处理纹理坐标
        if(mesh->mTextureCoords[0]) // 有纹理坐标？
        {
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.TexCoords = vec;
        } else
            vertex.TexCoords = QVector2D(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // 处理索引
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // 处理材质
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        QVector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        QVector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(m_glFuns,vertices, indices, textures);
}

QVector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, QString typeName)
{
    QVector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // 这个是为了避免重复的加载texture
        bool skip = false;
        for(unsigned int j = 0; j < m_texturesLoaded.size(); j++)
        {
            if(m_texturesLoaded[j].path == QString(str.C_Str()))
            {
                textures.push_back(m_texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            Texture texture;
            texture.id = getTextureIdFromFile(str.C_Str(), m_directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_texturesLoaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::getTextureIdFromFile(const char *path, const QString& directory)
{
    QString filename(path);
    filename = directory + '/' + filename;

    QOpenGLTexture * texture=new QOpenGLTexture(QImage(filename).mirrored());
    if(texture==NULL)
    {
        qDebug()<<"texture is NULL";
    }
    else
    {
        qDebug()<<filename<<"loaded";
    }
    return texture->textureId();
}
