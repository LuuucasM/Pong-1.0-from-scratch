#pragma once

#include "Shader.h"

#include <map>
#include <string>

class ResourceManager {
private:
	ResourceManager() { }
public:
	static std::map<std::string, Shader> Shaders;
	static Shader LoadShader(const char *vShaderPath, const char *fShaderPath, const char *gShaderPath, const char *name);
	static Shader& GetShader(std::string name);
	static void Delete();
};