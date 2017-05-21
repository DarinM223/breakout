#include "PostProcessor.h"

RenderManager::RenderManager(PostProcessor& processor) : processor_(processor) {
  glBindFramebuffer(GL_FRAMEBUFFER, processor_.msfbo_);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

RenderManager::~RenderManager() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, processor_.msfbo_);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, processor_.fbo_);
  glBlitFramebuffer(0, 0, processor_.width_, processor_.height_, 0, 0,
                    processor_.width_, processor_.height_, GL_COLOR_BUFFER_BIT,
                    GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PostProcessor::PostProcessor(Shader& shader, int width, int height)
    : width_(width),
      height_(height),
      shader_(shader),
      texture_(nullptr, Dimensions{width, height}, TextureOptions{}) {
  glGenFramebuffers(1, &msfbo_);
  glGenFramebuffers(1, &fbo_);
  glGenRenderbuffers(1, &rbo_);

  glBindFramebuffer(GL_FRAMEBUFFER, msfbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width_, height_);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, rbo_);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw MsfboInitException{};
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture_.id(), 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw FboInitException{};
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Initialize render data
  {
    GLuint vbo;
    GLfloat vertices[] = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f,
                          -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                          1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f};
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  shader_.use();
  shader_.setInteger("scene", 0);

  GLfloat offset = 1.0f / 300.0f;
  GLfloat offsets[9][2] = {
      {-offset, offset},  {0.0f, offset},  {offset, offset},
      {-offset, 0.0f},    {0.0f, 0.0f},    {offset, 0.0f},
      {-offset, -offset}, {0.0f, -offset}, {offset, -offset}};
  glUniform2fv(glGetUniformLocation(shader_.program(), "offsets"), 9,
               (GLfloat*)offsets);

  GLint edgeKernel[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
  glUniform1iv(glGetUniformLocation(shader_.program(), "edge_kernel"), 9,
               edgeKernel);

  GLfloat blurKernel[9] = {1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16,
                           2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16};
  glUniform1fv(glGetUniformLocation(shader_.program(), "blur_kernel"), 9,
               blurKernel);
}

PostProcessor::PostProcessor(PostProcessor&& other)
    : shader_(other.shader_), texture_(std::move(other.texture_)) {
  confuse = other.confuse;
  chaos = other.chaos;
  shake = other.shake;

  width_ = other.width_;
  height_ = other.height_;

  msfbo_ = other.msfbo_;
  fbo_ = other.fbo_;
  rbo_ = other.rbo_;
  vao_ = other.vao_;
  other.valid_ = false;
}

PostProcessor& PostProcessor::operator=(PostProcessor&& other) {
  this->release();
  confuse = other.confuse;
  chaos = other.chaos;
  shake = other.shake;

  width_ = other.width_;
  height_ = other.height_;

  shader_ = std::move(other.shader_);
  texture_ = std::move(other.texture_);

  msfbo_ = other.msfbo_;
  fbo_ = other.fbo_;
  rbo_ = other.rbo_;
  vao_ = other.vao_;
  other.valid_ = false;
  return *this;
}

void PostProcessor::render(GLfloat time) {
  shader_.use();
  shader_.setFloat("time", time);
  shader_.setInteger("confuse", confuse);
  shader_.setInteger("chaos", chaos);
  shader_.setInteger("shake", shake);

  glActiveTexture(GL_TEXTURE0);
  texture_.bind();
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
