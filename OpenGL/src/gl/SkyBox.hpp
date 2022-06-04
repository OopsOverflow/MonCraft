#pragma once

#include <GL/glew.h>
#include <cstdint>

class Camera;
class Shader;

static const float skyRotSpeed = 0.00001f;
class SkyBox
{
public:
	SkyBox();

	Shader* skyBoxShader;
	void render(Camera& camera);

	float getBlendFactor() const { return blendFactor; }
private:
	GLuint buffer;
	GLuint skyDayTxr;
	GLuint skyNightTxr;
	GLuint initSky();
	void calcBlendFactor(uint32_t skytime);

	float blendFactor;
	float rotation;
};
