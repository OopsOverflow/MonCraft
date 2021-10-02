#pragma once

#include "Component.hpp"
#include "gl/Shader.hpp"

enum class Crop { NONE, BEGIN, CENTER, END };


namespace ui {

	class Image : public Component {

	public:
		Image();
		virtual ~Image();

		void draw() override;

		static const spec_t CROP;
		static const spec_t TEXTURE_OFFSET;
		static const spec_t TEXTURE_SIZE;
		virtual void setStyle(prop_t const& prop) override;
		virtual prop_t getStyle(spec_t spec) const override;
		virtual style_const_t getDefaultStyle() const override;

		void setCrop(Crop crop);
		Crop getCrop() const;

		void setTextureOffset(glm::vec2 offset);
		glm::vec2 getTextureOffset() const;

		void setTextureSize(glm::vec2 size);
		glm::vec2 getTextureSize() const;

	private:
		glm::mat4 computeModel();
		glm::mat4 computeTexture();
		static Shader* shader;
		static GLuint texAtlas;

		glm::vec2 texOffset;
		glm::vec2 texSize;
		Crop crop;

		static GLuint vao, vbo;
	};

}; // namespace ui
