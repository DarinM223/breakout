#ifndef DEBUG_H
#define DEBUG_H

#include <GL/glew.h>
#include <iostream>

GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif
