#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>

#pragma once

static std::string readFileSync(std::string file_path){
  // Read the Vertex Shader code from the file
	std::string Data;
	std::ifstream DataStream(file_path, std::ios::in);
	if(DataStream.is_open()){
		std::stringstream sstr;
		sstr << DataStream.rdbuf();
		Data = sstr.str();
		DataStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory?\n", file_path.c_str());
	}
  return Data;
}

template<typename Unknown>
static Unknown map(Unknown value, Unknown min1, Unknown max1, Unknown min2, Unknown max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
static void println(std::string in){
  //std::cout << in << std::endl;
}
template<typename Unknown>
static std::string str(Unknown obj){
  return std::to_string(obj);
}