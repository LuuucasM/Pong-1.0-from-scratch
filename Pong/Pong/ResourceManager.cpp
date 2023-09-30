#include "ResourceManager.h"

#include <string>

Shader ResourceManager::LoadShader(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath, const char *name) {
	//retrive code from files
	std::string vShaderSource;
	std::string fShaderSource;
	std::string gShaderSource;

	std::ifstream vShaderFile, fShaderFile, gShaderFile;

	std::stringstream vShaderStream, fShaderStream, gShaderStream;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vShaderPath);
		fShaderFile.open(fShaderPath);
		

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vShaderSource = vShaderStream.str();
		fShaderSource = fShaderStream.str();

		if (gShaderPath)
		{
			gShaderFile.open(gShaderPath);
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			gShaderSource = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE NOT SUCCESSFULY READ" << e.what() << std::endl;
	}
	const char *vShaderCode = vShaderSource.c_str();
	const char *fShaderCode = fShaderSource.c_str();
	const char* gShaderCode = gShaderSource.c_str();
	Shader shader;
	shader.Init(vShaderCode, fShaderCode, gShaderPath != nullptr ? gShaderCode : nullptr, name);
	Shaders[name] = shader;
	return shader;
}
Shader& ResourceManager::GetShader(std::string name) {
	return Shaders[name];
}
void ResourceManager::Delete() {
	for (auto i : Shaders) {
		i.second.Delete();
	}
}