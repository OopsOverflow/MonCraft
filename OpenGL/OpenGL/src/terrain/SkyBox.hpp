#pragma once
#include "gl/Shader.hpp"
#include "gl/Camera.hpp"

class SkyBox
{
public:
	SkyBox();

	Shader* skyBoxShader;
	void render(Camera& camera, float rotaion);

private:
	GLuint buffer;
	GLuint skyDayTxr;
	GLuint skyNightTxr;
	GLuint initSky();
	void calcBlendFactor(float skytime);
};
