#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
    m_Init = false;
    m_Shaders.insert({"terrain", new Shader("terrainVertex.glsl", "terrainFragment.glsl") });
    m_Shaders.insert({"material", new Shader("texturedvertex.glsl", "texturedfragment.glsl") });
    m_Shaders.insert({ "animation", new Shader("skinningtexturedvertex.glsl", "texturedfragment.glsl") });
    m_Shaders.insert({"water", new Shader("reflectVertex.glsl", "waterreflectFragment.glsl") });
    m_Shaders.insert({"skybox", new Shader("skyboxVertex.glsl", "skyboxFragment.glsl") });

    m_Shaders.insert({"light", new Shader("lightvert.glsl","lightfrag.glsl")});
    
    m_Shaders.insert({"combine", new Shader("combinevert.glsl","combinefrag.glsl")});

    m_Shaders.insert({ "postprocess", new Shader("combinevert.glsl","processFrag.glsl") });

    m_Shaders.insert({ "scene", new Shader("combinevert.glsl","finalfrag.glsl") });

    
    for(auto &shader : m_Shaders)    
    {
        if (!(shader.second->LoadSuccess()))
        {
            return;
        }
    }
    m_Init = true;
}
ShaderManager& ShaderManager::GetInstance()
{
    static ShaderManager m_Instance;
    return m_Instance;
}

Shader* ShaderManager::GetShader(const char* name)
{
    return m_Shaders.at(name);
}

ShaderManager::~ShaderManager()
{
    for (auto& shader : m_Shaders)
    {
        delete shader.second;
    }
}
