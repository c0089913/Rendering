#pragma once
#include "SceneNode.h"
#include "Camera.h"
#include "HeightMap.h"

class WaterNode :
    public SceneNode
{
    float waterRotate;
    float waterCycle;
    float distortionSpeed;
    GLuint dudvTex;
    GLuint bumpTex;
public:
    WaterNode();
    virtual void Update(float dt);
    void Draw(OGLRenderer& r, Camera* camera, HeightMap *heightMap);
};

