#pragma once
#include "../util/Loader.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

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

