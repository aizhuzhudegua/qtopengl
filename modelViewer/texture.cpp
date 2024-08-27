#include "texture.h"

Texture::Texture()
{

}

Texture::~Texture()
{
    delete m_texture;
}

void Texture::loadTexture(QImage tex)
{
    m_texture = new QOpenGLTexture(tex);
}


