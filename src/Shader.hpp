#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
  unsigned int ID;
  Shader(std::string vertPath, std::string fragPath){
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertPath, std::ios::in);
    if(VertexShaderStream.is_open()){
      std::stringstream sstr;
      sstr << VertexShaderStream.rdbuf();
      VertexShaderCode = sstr.str();
      VertexShaderStream.close();
    }else{
      printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertPath.c_str());
      getchar();
      //return 0;
    }
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragPath, std::ios::in);
    if(FragmentShaderStream.is_open()){
      std::stringstream sstr;
      sstr << FragmentShaderStream.rdbuf();
      FragmentShaderCode = sstr.str();
      FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertPath.c_str());
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
      glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
      printf("%s\n", &VertexShaderErrorMessage[0]);
    }
    
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragPath.c_str());
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
      glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
      printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    
    // Link the program
    printf("Linking Shader\n");
    ID = glCreateProgram();
    glAttachShader(ID, VertexShaderID);
    glAttachShader(ID, FragmentShaderID);
    glLinkProgram(ID);
    
    // Check the program
    glGetProgramiv(ID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      std::vector<char> ProgramErrorMessage(InfoLogLength+1);
      glGetProgramInfoLog(ID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
      printf("%s\n", &ProgramErrorMessage[0]);
    }
    
    glDetachShader(ID, VertexShaderID);
    glDetachShader(ID, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
  }
  
  void use(){
    glUseProgram(ID);
  }
  
  GLuint VAO, VBO;
  void initPanel(){
    //Init a panel that the shader can draw to (only call on one shader)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    float vbodat[] = {
      -1.0f, -1.0f,
      1.0f, -1.0f,
      -1.0f, 1.0f,
      1.0f, -1.0f,
      1.0f, 1.0f,
      -1.0f, 1.0f
    };
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbodat), vbodat, GL_STATIC_DRAW);
  }
  void drawPanel(){
    //Draw panel
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       2,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
  }
  void setBool(const std::string &name, bool value) const
  {         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
  }
  void setInt(const std::string &name, int value) const
  { 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
  }
  void setFloat(const std::string &name, float value) const
  { 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
  } 
  void setFloat2(const std::string &name, float value, float value2) const
  { 
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value, value2); 
  } 
  void setDouble(const std::string &name, double value) const
  { 
    glUniform1d(glGetUniformLocation(ID, name.c_str()), value); 
  }
  void setDouble2(const std::string &name, double value, double value2) const
  { 
    glUniform2d(glGetUniformLocation(ID, name.c_str()), value, value2); 
  }
};
