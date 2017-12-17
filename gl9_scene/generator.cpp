#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "generator.h"
#include "helikoptera.h"
#include "vrtula.h"

using namespace std;
using namespace glm;
using namespace ppgso;

bool Generator::update(Scene &scene, float dt) {
  // Accumulate time
  time += dt;
    int random;
    vzdialenost += 12 * dt;
  // Add object to scene when time reaches certain level
  if (time > 2.0 && vzdialenost < 900.0f) {
      random = (rand()*(int)(dt*100000)) %2;
      auto obj = make_unique<Helikoptera>();
      auto obj1 = make_unique<Vrtula>();

      if(random == 0){

          obj->position = position;
          obj->position.x += linearRand(-20.0f, 20.0f);
          obj->position.y = vzdialenost;
          obj->speed  = vec3(-7.5f,0.0f,0.0f);
          obj->id = (int)vzdialenost;
          obj1->id = (int)vzdialenost;

          obj1->speed  = vec3(-7.5f,0.0f,0.0f);
          obj1->position = obj->position;



      }else {

          obj->position = position;
          obj->position.x += linearRand(-20.0f, 20.0f);
          obj->rotation.x = PI;
          obj->position.y = vzdialenost;
          obj->speed  = vec3(7.5f,0.0f,0.0f);
          obj->id = (int)vzdialenost;
          obj1->id = (int)vzdialenost;

          obj1->speed  =vec3(+7.5f,0.0f,0.0f);
         obj1->rotation.x = PI/180;
          obj1->position = obj->position;
      }

      obj->smer = random;
      //obj1->smer = random;

    scene.objects.push_back(move(obj));
      scene.objects.push_back(move(obj1));


    //  scene.objects.push_back(move(obj->v));


    time = linearRand(0.0f, 1.5f);
  }

  return true;
}

void Generator::render(Scene &scene) {
  // Generator will not be rendered
}
