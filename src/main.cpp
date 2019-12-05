#include "Window.hpp"
#include "Shader.hpp"

Window* window;

double rendZoom = 2;
double midX = -0.6;
double midY = 0;
int maxIter = 100;

double rendZoomSkip ;
int curZoomNum = 20;
int curZoomExp = -1;
void mouseScrolled(float scrollX, float scrollY){
  //Sensitivity check
  rendZoomSkip += abs(scrollY);
  if(rendZoomSkip > 7){
    rendZoomSkip = 0;
    //Zooming for rendering coordinates
    if(scrollY > 0){ //Zoom out (touchpad down)
      if(curZoomExp == -1){
        if(curZoomNum < 20){
          curZoomNum++;
        }
      }else{
        if(curZoomNum < 10){
          curZoomNum++;
        }else{
          curZoomNum = 1;
          if(curZoomExp < -1){
            curZoomExp++;
          }
        }
      }
    }else{ //Zoom in (touchpad up)
      if(curZoomNum > 1){
        curZoomNum--;
      }else{
        curZoomExp--;
        curZoomNum = 10;
      }
    }
  }
  
  double prevZoom = rendZoom;
  rendZoom = pow(10.0, curZoomExp) * curZoomNum;
  //Make sure rendZoom into spot cursor is at
  midX += map<double>(window->mouseX, 0,  window->width, -prevZoom, prevZoom) - map<double>(window->mouseX, 0,  window->width, -rendZoom, rendZoom);
  midY += map<double>(window->mouseY, 0, window->height, -prevZoom, prevZoom) - map<double>(window->mouseY, 0, window->height, -rendZoom, rendZoom);
  maxIter = abs(curZoomExp-2) * 50;
}

double prevMouseX;
double prevMouseY;
double midXStore;
double midYStore;
double begMouseX;
double begMouseY;
void mousePressed(bool pressed, int button){
  if(pressed){
    begMouseX = window->mouseX;
    begMouseY = window->mouseY;
    midXStore = midX;
    midYStore = midY;
  }
}
void mouseMoved(float x, float y){
  if(window->mouseIsPressed){
    midX = midXStore - (window->mouseX - begMouseX)/window->width * rendZoom*2;
    midY = midYStore - (window->mouseY - begMouseY)/window->height * rendZoom*2;
  }
}
void keyPressed(unsigned int key){
  if(key == 'r'){
    midX = -0.6;
    midY = 0;
    curZoomExp = -1;
    curZoomNum = 20;
  }else if(key == ' '){
    window->toggleLoop();
  }else if(key == 'w'){
    window->setSize(600,600);
  }
}

unsigned int vao;
Shader shader;

unsigned int loc_zoom;
unsigned int loc_midx;
unsigned int loc_midy;
unsigned int loc_maxiter;
void setup(){
  shader = Shader("../src/shaders/FractalShader.vert", "../src/shaders/FractalShader.frag");
  //Make VertexArray for panel shader will render on.
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  float vbodat[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f,
    -1.0f, 1.0f
  };
  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbodat), vbodat, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
     0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
     2,                  // size
     GL_FLOAT,           // type
     GL_FALSE,           // normalized?
     0,                  // stride
     (void*)0            // array buffer offset
  );
  
  shader.Bind();
  loc_zoom = shader.GetUniformLocation("zoom");
  loc_midx = shader.GetUniformLocation("midX");
  loc_midy = shader.GetUniformLocation("midY");
  loc_maxiter = shader.GetUniformLocation("maxIter");
  glClearColor(0, 0, 0, 1);
}
void draw(){
  glClear(GL_COLOR_BUFFER_BIT);
  shader.Uniform(loc_zoom, (float)rendZoom);
  shader.Uniform(loc_midx, (float)midX);
  shader.Uniform(loc_midy, (float)midY);
  shader.Uniform(loc_maxiter, maxIter);
  
  //Draw panel (there is only one VertexArray so no need to bind)
  glDrawArrays(GL_TRIANGLES, 0, 6); //2 Triangles make a square (panel)
}
void exit(){
  std::cout << "User Exited Program\n";
}

int main(void){
  window = new Window(600,600, "Mandelbrot Fractal");
  
  window->mouseScrolled = mouseScrolled;
  window->mouseMoved = mouseMoved;
  window->keyPressed = keyPressed;
  window->mouseButton = mousePressed;
  window->onExit = exit;
  setup();
  window->loop(draw);
}
