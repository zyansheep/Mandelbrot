//#include <cstdio>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

class Window{
  //We are using go-style naming (Capitalize member variables that are used in other classes)
private:
  GLFWwindow* window;
public:
  int width;
  int height;
  std::string title;
  
  bool shouldClose = false;
  bool isLooping = true;
  
  //Event functions
  void(*mouseButton)(bool, int);
  void(*mouseMoved)(float, float);
  void(*mouseScrolled)(float,float);
  void(*keyPressed)(unsigned int);
  void(*onExit)();
  void(*onResize)(int, int);
  Window(int w, int h, std::string t){
    width = w;
    height = h;
    title = t;
    
    //Initialize GLFW
    if (!glfwInit()){
      std::cout << "[Window] Failed to init GLFW" << std::endl;
      return;
    }
    
    //Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window){
      glfwTerminate();
      std::cout << "[Window] Failed to create Window" << std::endl;
      return;
    }
    
    glfwSetWindowUserPointer(window, this); //tell glfw to store this class's pointer
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    //Mouse callbacks
    glfwSetScrollCallback(window, mouseScrolledCallback);
    glfwSetCursorPosCallback(window, mouseMovedCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorEnterCallback(window, mouseEnteredCallback);
    glfwSetCharCallback(window, keyPressedCallback);
    glfwSetWindowCloseCallback(window, windowCloseCallback);
    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      std::cout << "[Window] Failed to initialize GLAD" << std::endl;
      return;
    }
    
    windowResizeCallback(window, width, height);
  }
  
  void setSize(int w, int h){
    width = w;
    height = h;
    glfwSetWindowSize(window, w, h);
  }
  void setTitle(std::string t){
    title = t;
    glfwSetWindowTitle(window, t.c_str());
  }
  
  void toggleLoop(){
    isLooping = !isLooping;
  }
  void exit(){
    shouldClose = true;
  }
  void (*drawFunc)();
  void loop(void (&func)()){
    drawFunc = func;
    while (!shouldClose){
      scrollX = 0; //scroll delta X & Y
      scrollY = 0;
      
      if(isLooping){
        (*func)();
      }
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      /* Poll for and process events */
      glfwPollEvents();
      
      mouseXD = mouseX - mouseXP;
      mouseYD = mouseY - mouseYP;
      
      mouseXP = mouseX;
      mouseYP = mouseY;
    }
    glfwTerminate();
  }
  float scrollX, scrollY;
  static void mouseScrolledCallback(GLFWwindow* window, double xoffset, double yoffset){
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    w->scrollX = xoffset;
    w->scrollY = yoffset;
    if(w->mouseScrolled){
      w->mouseScrolled(xoffset, yoffset);
    }
  }
  float mouseX, mouseY;
  float mouseXD, mouseYD; //change in mouse position
  float mouseXP, mouseYP; //change in mouse position
  bool mouseInWindow = false;
  static void mouseMovedCallback(GLFWwindow* window, double x, double y){
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    w->mouseX = x;
    w->mouseY = w->height - y;
    if(w->mouseMoved){
      w->mouseMoved(w->mouseX, w->mouseY);
    }
  }
  bool mouseIsPressed = false;
  static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    if(action == GLFW_PRESS){
      w->mouseIsPressed = true;
      if(w->mouseButton){
        w->mouseButton(w->mouseIsPressed, button);
      }
    }else if (action == GLFW_RELEASE){
      w->mouseIsPressed = false;
      if(w->mouseButton){
        w->mouseButton(w->mouseIsPressed, button);
      }
    }
  }
  static void mouseEnteredCallback(GLFWwindow* window, int entered){
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    if(entered){
      w->mouseInWindow = true;
    }else{
      w->mouseInWindow = false;
    }
  }
  unsigned int keyCode;
  static void keyPressedCallback(GLFWwindow* window, unsigned int key){
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    w->keyCode = key;
    if(w->keyPressed){
      w->keyPressed(key);
    }
  }
  static void windowCloseCallback(GLFWwindow* window){
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    if(w->onExit){
      w->onExit();
    }
    w->shouldClose = true;
  }
  int biggerDimension = 0;
  int smallerDimension = 0;
  static void windowResizeCallback(GLFWwindow* window, int width, int height){
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    if(w->width < w->height){
      w->smallerDimension = w->width;
      w->biggerDimension = w->height;
    }else{
      w->smallerDimension = w->height;
      w->biggerDimension = w->width;
    }
  }
  bool doDrawOnDragResize = true;
  void toggleDrawOnDragResize(){
    doDrawOnDragResize = !doDrawOnDragResize;
  }
  static void framebufferResizeCallback(GLFWwindow* window, int width, int height){
    //std::cout <<"Framebuffer Size: " <<width<<"x"<<height<<std::endl;
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    w->width = width;
    w->height = height;
    if(w->onResize){
      w->onResize(width, height);
    }
    if(w->doDrawOnDragResize){
      w->drawFunc();
      glfwSwapBuffers(window);
    }
    glViewport(0, 0, width, height);
  }
};