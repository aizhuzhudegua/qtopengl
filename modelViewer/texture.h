#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLTexture>
#include "object.h"

class Texture:public Object
{
public:
    Texture();
    ~Texture();
    void loadTexture(QImage tex);

    QOpenGLTexture *m_texture;
private:

};

#endif // TEXTURE_H
