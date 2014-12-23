#version 400

in vec3 position;
in vec3 normal;
in vec2 TexCoords;

//struct lightInfo{
//   vec4 position;
//   vec3 intensity;
//};

//uniform lightInfo light;
//uniform vec3 Kd;
//uniform vec3 Ka;
//uniform vec3 Ks;
//uniform float shininess;
//uniform sampler2D bark;
//uniform sampler2D leaves;
uniform bool isBark;
out vec4 fragColour;

//vec3 ads(){
//   vec3 n = normalize(normal);
//   vec3 s = normalize(vec3(light.position) - position);
//   vec3 v = normalize(vec3(-position));
//   vec3 r = reflect(-s, n);
//   vec3 h = normalize(v + s);
//   return light.intensity * (Ka + Kd * max(dot(s,n),0.0)+ Ks * pow(max(dot(h, n), 0.0), shininess));
//}

void  main(){
   //vec4 colour = vec4(0.3, 0.5, 0.0, 1.0);
   //vec4 colour = vec4(ads(),1.0);
   if (isBark){
      fragColour = vec4(1.0, 0.0, 0.0, 1.0);//colour * texture(bark, TexCoords);
   }
   else if (!isBark  && !isBillboard){
      fragColour =vec4(0.0, 1.0, 0.0, 1.0);// colour * texture(bark, TexCoords);
//      if (texture(leaves, TexCoords).a == 0.0){
//        discard;
//      }
   }

}
