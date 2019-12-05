#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>

#include "Shader.hpp"

static void setColor(int r, int g, int b, int a = 255){
  glColor4f((1.0f/255) * r, (1.0f/255) * g, (1.0f/255) * b, (1.0f/255) * a);
}
static void drawRect(int x0, int y0, int x1, int y1){
  glBegin(GL_QUADS);
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex2f(-0.5f, 0.5f); //Top Left corner
  glVertex2f(0.5f, 0.5f); //Top Right corner
  glVertex2f(0.5f, -0.5f); //Button Right corner
  glVertex2f(-0.5f, -0.5f); //Bottom Left corner
  glEnd();
}
template<typename Unknown>
static Unknown map(Unknown value, Unknown min1, Unknown max1, Unknown min2, Unknown max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
static void println(std::string in){
  std::cout << in << std::endl;
}
template<typename Unknown>
static std::string str(Unknown obj){
  return std::to_string(obj);
}
static void clear(float r, float g, float b, float a = 1){
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}
static void clear(int a = 1){
  clear(0,0,0,a);
}