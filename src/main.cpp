#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "Level.h"

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

void keyCallback(GLFWwindow *, int, int, int, int);

int main() {
  ResourceManager manager{};
  Game breakout{SCREEN_WIDTH, SCREEN_HEIGHT, manager};

  glfwInit();
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout",
                                        nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, &breakout);

  glewExperimental = GL_TRUE;
  glewInit();
  glGetError();

  glfwSetKeyCallback(window, keyCallback);

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  breakout.init();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    breakout.updateTime(glfwGetTime());
    breakout.processInput();
    breakout.update();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    breakout.render();

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mode) {
  Game *breakout = reinterpret_cast<Game *>(glfwGetWindowUserPointer(window));
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      breakout->setKey(key, GL_TRUE);
    } else if (action == GLFW_RELEASE) {
      breakout->setKey(key, GL_FALSE);
    }
  }
}
