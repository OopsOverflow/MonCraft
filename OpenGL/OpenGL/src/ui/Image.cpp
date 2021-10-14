#include "Image.hpp"
#include "debug/Debug.hpp"
#include "gl/ResourceManager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ui;
using namespace glm;

MAKE_TYPE(Crop);

const spec_t Image::CROP = MAKE_SPEC("Image::crop", Crop);
const spec_t Image::TEXTURE_OFFSET = MAKE_SPEC("Image::textureOffset", ivec2);
const spec_t Image::TEXTURE_SIZE = MAKE_SPEC("Image::textureSize", ivec2);

static const GLfloat quad[6][2] = {
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },

    { 1.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
};

Shader* Image::shader = nullptr;
GLuint Image::texAtlas = 0;
GLuint Image::vao = 0;
GLuint Image::vbo = 0;


Image::Image(ivec2 offset, ivec2 size)
 : minFilter(GL_NEAREST),
   magFilter(GL_NEAREST)
{
    setTextureOffset(offset);
    setTextureSize(size);

    if (shader == nullptr) {
        shader = ResourceManager::getShader("image");
        texAtlas = ResourceManager::getTexture("imageAtlas");

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, quad, GL_STATIC_DRAW);
            glVertexAttribPointer(VERTEX_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(VERTEX_POSITION);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            GLint texSampler = shader->getUniform("uTexture");
            glUniform1i(texSampler, 0);
            glActiveTexture(GL_TEXTURE0);
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, texAtlas);
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

std::unique_ptr<Image> Image::create(ivec2 offset, ivec2 size) {
  auto comp = std::unique_ptr<Image>(new Image(offset, size));
  comp->initialize();
  return comp;
}

Image::~Image() {
    ASSERT_GL_MAIN_THREAD();
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Image::setProperty(prop_t prop) {
    if (prop.spec == Image::CROP) {
        setCrop(prop.value->get<Crop>());
    }
    else if (prop.spec == Image::TEXTURE_OFFSET) {
        setTextureOffset(prop.value->get<ivec2>());
    }
    else if (prop.spec == Image::TEXTURE_SIZE) {
        setTextureSize(prop.value->get<ivec2>());
    }
    else {
        Component::setProperty(prop);
    }
}

prop_t Image::getProperty(spec_t spec) const {
    if (spec == Image::CROP) {
        return make_prop(spec, getCrop());
    }
    else if (spec == Image::TEXTURE_OFFSET) {
        return make_prop(spec, getTextureOffset());
    }
    else if (spec == Image::TEXTURE_SIZE) {
        return make_prop(spec, getTextureSize());
    }
    else {
        return Component::getProperty(spec);
    }
}

style_const_t Image::getDefaultStyle() const {
    static style_const_t style = Style::make_style(
        Component::getDefaultStyle(),
        CROP, Crop::NONE
    );
    return style;
}


void Image::draw() {
    shader->activate();
    shader->bindTexture(TEXTURE_COLOR, texAtlas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    mat4 model = computeModel();
    mat4 textureCoords = computeTexture();

    glBindVertexArray(vao);
    glUniformMatrix4fv(shader->getUniform(MATRIX_MODEL), 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(shader->getUniform("m_texture"), 1, GL_FALSE, value_ptr(textureCoords));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    Component::draw();
}

mat4 Image::computeModel() {
    auto p1 = getAbsoluteOrigin();
    auto model = mat4(1.f);
    model = translate(model, vec3(p1, 0.f));
    model = scale(model, vec3(getAbsoluteSize(), 1.f));
    return model;
}

mat4 Image::computeTexture() {

    ivec2 offset = getTextureOffset();
    ivec2 size = getTextureSize();
    calculateCropping(offset, size);
    int w, h;
    int miplevel = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

    auto texture = mat4(1.f);
    texture = translate(texture, vec3(offset.x / (float)w, offset.y/(float)h , 0.0f));
    texture = scale(texture, vec3(size.x / (float)w, size.y / (float)h, 1.f));
    return texture;
}

void Image::calculateCropping(ivec2& offset, ivec2& size) {
    if (getCrop() != Crop::NONE) {
        ivec2 absoluteSize = getAbsoluteSize();
        ivec2 newOffset = offset;
        ivec2 newSize = size;
        if (absoluteSize.x / (float)absoluteSize.y < size.x / (float)size.y) {
            newSize.x = absoluteSize.x * size.y / (float)absoluteSize.y;
        }
        else {
            newSize.y = absoluteSize.y * size.x / (float)absoluteSize.x;
        }
        if (getCrop() == Crop::END)
            newOffset = offset + size - newSize;
        if (getCrop() == Crop::CENTER)
            newOffset = vec2(offset) + vec2(size - newSize) / 2.0f;
        offset = newOffset;
        size = newSize;
    }
}
void Image::setCrop(Crop crop) {
    setStyle(CROP, crop);
}

Crop Image::getCrop() const {
    return getStyle<Crop>(CROP);
}

void Image::setTextureOffset(ivec2 offset) {
    setStyle(TEXTURE_OFFSET, offset);
}

ivec2 Image::getTextureOffset() const {
    return getStyle<ivec2>(TEXTURE_OFFSET);
}

void Image::setTextureSize(ivec2 size) {
    setStyle(TEXTURE_SIZE, size);
}

ivec2 Image::getTextureSize() const {
    return getStyle<ivec2>(TEXTURE_SIZE);
}

void Image::setMagFilter(GLint param) {
    magFilter = param;
}

void Image::setMinFilter(GLint param) {
    minFilter = param;
}
