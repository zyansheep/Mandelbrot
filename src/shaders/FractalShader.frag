#version 410 core

out vec4 FragColor;

uniform double zoom;
uniform double midX;
uniform double midY;
uniform int maxIter;

in vec2 pos; //get current pixel position

double map(double value, double min1, double max1, double min2, double max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
double map(int value, int min1, int max1, int min2, int max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main(){
  uint iter = 0;
  
  double a = map(pos.x, -1.0, 1.0, midX - zoom, midX + zoom); //Normal component of c
  double b = map(pos.y, -1.0, 1.0, midY - zoom, midY + zoom); //Complex component of c
  
  double za = 0.0;
  double zb = 0.0;
  while(iter < maxIter){
    double zaNew = za*za - zb*zb + a;
    zb = 2.0*za*zb + b;
    za = zaNew;
    
    if(abs(za + zb) > 10){
      break;
    }
    iter++;
  }
  
  double col = map(iter, 0.0f, maxIter, 0.0f,1.0f);
  FragColor = vec4(col,col,col,1);
  //FragColor = vec4((sin(time * 10.0f) + 1.0f) / 2.0f, (cos(time * 10.0f) + 1.0f) / 2.0f, 1.0f - (sin(time * 10.0f) + 1.0f) / 2.0f, 1);
} 