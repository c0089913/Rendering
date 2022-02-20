#pragma once
#include <map>
#include "Shader.h"
class ShaderManager
{
	std::map<const char *, Shader*> m_Shaders;
	ShaderManager();
	bool m_Init;
public:
	static ShaderManager& GetInstance();
	bool InitSuccess() { return m_Init; }
	Shader* GetShader(const char *name);
	~ShaderManager();
};

