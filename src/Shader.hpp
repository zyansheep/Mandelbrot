#include <iostream>

#include <vector>

#include "Functions.hpp"

#pragma once

class Shader {
public:
  Shader(){};
  Shader(const std::string& vertexPath, const std::string& fragmentPath){
    std::string VertexShaderSource = readFileSync(vertexPath);
    std::string FragmentShaderSource = readFileSync(fragmentPath);
    printf("Compiling shader : %s\n", vertexPath.c_str());
    unsigned int VertexID;
    CompileShader(VertexID, GL_VERTEX_SHADER, VertexShaderSource.c_str());
    
    printf("Compiling shader : %s\n", fragmentPath.c_str());
    unsigned int FragmentID;
    CompileShader(FragmentID, GL_FRAGMENT_SHADER, FragmentShaderSource.c_str());
    
    printf("Linking program\n");
    CompileProgram(program, {&VertexID, &FragmentID});
  }
  unsigned int program;
  
  static void CompileProgram(unsigned int &ProgramID, std::vector<unsigned int*> shaders){
    ProgramID = glCreateProgram();
    for(int i=0;i<shaders.size();i++){
      glAttachShader(ProgramID, *(shaders[i]));
    }
  	
  	glLinkProgram(ProgramID);
    
    int Result = GL_FALSE;
    int InfoLogLength;
    
  	// Check the program
  	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  	if ( InfoLogLength > 0 ){
  		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
  		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
  		printf("%s\n", &ProgramErrorMessage[0]);
  	}
  }
  static void CompileShader(unsigned int &ShaderID, unsigned int type, const std::string& source){
    ShaderID = glCreateShader(type);
    
    int Result = GL_FALSE;
  	int InfoLogLength;

  	// Compile Shader
  	char const * sourcePointer = source.c_str();
  	glShaderSource(ShaderID, 1, &sourcePointer , NULL);
  	glCompileShader(ShaderID);

  	// Check Vertex Shader
  	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
  	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  	if ( InfoLogLength > 0 ){
  		std::vector<char> ShaderErrorMessage(InfoLogLength+1);
  		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
  		printf("%s\n", &ShaderErrorMessage[0]);
  	}
  }
  
  void Bind(){
    glUseProgram(program);
  }
  int GetUniformLocation(const char* uniform_handle){
    return glGetUniformLocation(program, uniform_handle);
  }
  
  //#include "UniformFunctions.cpp" //Import uniform functions
  //Primitive
  void Uniform(int location, float toSend){
    glUniform1f(location, toSend);
  }
  void Uniform(int location, int toSend){
    glUniform1i(location, toSend);
  }
  void Uniform(int location, unsigned int toSend){
    glUniform1ui(location, toSend);
  }

};