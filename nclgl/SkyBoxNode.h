#pragma once
#include "SceneNode.h"
class SkyBoxNode :
    public SceneNode
{
public:
    SkyBoxNode();
    void Draw(OGLRenderer& r);
    GLuint GetCubemap() { return texture; }
};

