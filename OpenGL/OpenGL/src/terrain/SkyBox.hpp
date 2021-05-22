#pragma once
#include "../gl/TextureLoader.hpp"
#include "../gl/Shader.hpp"
#include "../gl/Camera.hpp"

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
