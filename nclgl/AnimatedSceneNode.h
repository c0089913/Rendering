#pragma once
#include "MaterialSceneNode.h"
class MeshAnimation;

class AnimatedSceneNode :
    public MaterialSceneNode
{

    MeshAnimation* animation;
    int currentFrame;
    float frameTime;
public:
    AnimatedSceneNode(const char* shader, const char* name, const char *animationName, Vector3 position, Vector3 scale = Vector3(1, 1, 1));
    virtual void Draw(OGLRenderer& r);
    virtual void Update(float dt);
};

