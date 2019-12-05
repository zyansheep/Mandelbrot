#version 410 core

out vec4 FragColor;

uniform float zoom;
uniform float midX;
uniform float midY;
uniform int maxIter;

in vec2 pos; //get current pixel position

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
float map(int value, int min1, int max1, int min2, int max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main(){
  uint iter = 0;
  
  float a = map(pos.x, -1.0, 1.0, midX - zoom, midX + zoom); //Normal component of c
  float b = map(pos.y, -1.0, 1.0, midY - zoom, midY + zoom); //Complex component of c
  
  float za = 0.0;
  float zb = 0.0;
  while(iter < maxIter){
    float zaNew = za*za - zb*zb + a;
    zb = 2.0*za*zb + b;
    za = zaNew;
    
    if(abs(za + zb) > 10){
      break;
    }
    iter++;
  }
  
  float col = map(iter, 0.0f, maxIter, 0.0f,1.0f);
  FragColor = vec4(col,col,col,1);
  //FragColor = vec4((sin(time * 10.0f) + 1.0f) / 2.0f, (cos(time * 10.0f) + 1.0f) / 2.0f, 1.0f - (sin(time * 10.0f) + 1.0f) / 2.0f, 1);
} 