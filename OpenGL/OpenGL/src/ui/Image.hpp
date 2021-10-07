#pragma once

#include "Component.hpp"
#include "gl/Shader.hpp"

enum class Crop { NONE, BEGIN, CENTER, END };


namespace ui {

	class Image : public Component {

	public:
		Image(glm::ivec2 offset, glm::ivec2 size);
		virtual ~Image();

		void draw() override;

		static const spec_t CROP;
		static const spec_t TEXTURE_OFFSET;
		static const spec_t TEXTURE_SIZE;
		virtual void setProperty(prop_t prop) override;
		virtual prop_t getProperty(spec_t spec) const override;
		virtual style_const_t getDefaultStyle() const override;

		void setCrop(Crop crop);
		Crop getCrop() const;

		void setTextureOffset(glm::ivec2 offset);
		glm::ivec2 getTextureOffset() const;

		void setTextureSize(glm::ivec2 size);
		glm::ivec2 getTextureSize() const;
		
		void setMagFilter(GLint param);
		void setMinFilter(GLint param);
	private:
		glm::mat4 computeModel();
		glm::mat4 computeTexture();
		void calculateCropping(glm::ivec2& offset, glm::ivec2& size);
		static Shader* shader;
		static GLuint texAtlas;

		glm::ivec2 texOffset;
		glm::ivec2 texSize;
		Crop crop;

		GLint minFilter;
		GLint magFilter;

		static GLuint vao, vbo;
	};

}; // namespace ui
