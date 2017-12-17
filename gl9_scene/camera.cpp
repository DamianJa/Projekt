#include <glm/glm.hpp>

#include "camera.h"
#include "scene.h"

using namespace std;
using namespace glm;
using namespace ppgso;

Camera::Camera(float fow, float ratio, float near, float far) {
  float fowInRad = (PI/180.0f) * fow;

  projectionMatrix = perspective(fowInRad, ratio, near, far);
}

void Camera::update(float dt) {
    //position.y += 12*dt;




  if(pohlad == 1){
    viewMatrix = lookAt(vec3(position.x,position.y+10,position.z-30), vec3(position.x,position.y+10,position.z)-back, up);
  }else{
    viewMatrix = lookAt(vec3(position.x,position.y-20,position.z-20), vec3(position.x,position.y+10,position.z+20)-back, up);
  }

}
