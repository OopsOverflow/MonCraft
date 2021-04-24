#pragma once
#include "../util/Loader.hpp"
#include "../util/Shader.hpp"
#include "../camera/Camera.hpp"

class SkyBox
{
public:
	SkyBox();

	Shader skyBoxShader;
	void render(Camera& camera);

private:
	GLuint buffer;
	GLuint skyTxr;
	GLuint initSky(Shader& shader);
	GLuint loadTxr();
};
