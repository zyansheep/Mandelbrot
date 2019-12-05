#include "Window.hpp"
#include "Functions.hpp"

Shader* shader;
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
void draw(){
  //setColor(255,0,255);
  //drawRect(100,100,500,500);
  clear();
  println("Sending zoom");
  shader->setDouble("zoom", rendZoom);
  println("Sending midx");
  shader->setDouble("midX", midX);
  println("Sending midy");
  shader->setDouble("midY", midY);
  println("Sending maxiter");
  shader->setInt("maxIter", maxIter);
  
  shader->drawPanel();
}
void exit(){
  std::cout << "User Exited Program\n";
}
int main(void){
  window = new Window(600,600, "Mandelbrot Fractal");
  
  shader = new Shader("shaders/FractalShader.vert", "shaders/FractalShader.frag");
  shader->initPanel();
  shader->use();
  
  window->mouseScrolled = mouseScrolled;
  window->mouseMoved = mouseMoved;
  window->keyPressed = keyPressed;
  window->mouseButton = mousePressed;
  window->onExit = exit;
  
  window->loop(draw);
}
