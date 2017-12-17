#include <glm/gtc/random.hpp>
#include "helikoptera.h"
#include "projectile.h"
#include "explosion.h"
#include "kvapka.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Helikoptera::mesh;
unique_ptr<Texture> Helikoptera::texture;
unique_ptr<Shader> Helikoptera::shader;


Helikoptera::Helikoptera() {
  // Set random scale speed and rotation


  scale *= 1.0f;
  speed = {0.0f, 0.0f, 0.0f};
  rotation = vec3(0.0f,1.57080f,-1.57080f);


  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("helikoptera.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("helikoptera.obj");
}

bool Helikoptera::update(Scene &scene, float dt) {
  // Count time alive
  age += dt;


  // Animate position according to time



    if(otacanie == 0 && fall == false && ((position.x >= 21.61f  && smer == 1) || (position.x <= -21.61f && smer == 0))){
       speed = vec3(0.0f,0.0f,0.0f);
        otacanie = 1;
        stupenotocenia = 0.0f;

    }
    position += speed * dt;
    if(otacanie>0){

        //rotation.y += 0.08726646;
        rotation.x += 0.01745329;
        stupenotocenia += 0.01745329;

        if(stupenotocenia >= PI ){

            smer += 1;
            smer = smer % 2;
            if(smer == 0){
                speed  = vec3(-7.5f,0.0f,0.0f);

            }else{

                speed  = vec3(+7.5f,0.0f,0.0f);
            }
            otacanie = 0;
            stupenotocenia = 0;
        }
    }


    if(fall == false){
        if (position.y < -10 ||age > 8.0f) return false;

        // Collide with scene
        for (auto &obj : scene.objects) {

            //cout << typeid(*obj).name() << endl;
            // Ignore self in scene
            if (obj.get() == this) continue;

            // We only need to collide with asteroids and projectiles, ignore other objects
            auto helikoptera = dynamic_cast<Helikoptera*>(obj.get()); // dynamic_pointer_cast<Helikoptera>(obj);
            auto projectile = dynamic_cast<Projectile*>(obj.get()); //dynamic_pointer_cast<Projectile>(obj);
            auto vrtula = dynamic_cast<Vrtula*>(obj.get());



            if(vrtula && vrtula->id == id){
                vrtula->position = position;
            }


            if (!helikoptera && !projectile ) continue;




            // When colliding with other asteroids make sure the object is older than .5s
            // This prevents excessive collisions when asteroids explode.
            if (helikoptera && age < 0.5f) continue;

            // Compare distance to approximate size of the helikoptera estimated from scale.
            if (distance(position.y, obj->position.y) < (obj->scale.y + scale.y) && ((smer == 0 && obj->position.x+3.5f> position.x && obj->position.x-5.0f< position.x) ||
                                                                                     (smer == 1 && obj->position.x-3.5f< position.x && obj->position.x+5.0f> position.x)))  {
                int pieces = 3;

                // Too small to split into pieces
                if (scale.y < 0.5) pieces = 0;

                // The projectile will be destroyed
                if (projectile) projectile->destroy();







                // Generate smaller asteroids

                explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale), pieces);

                fall = true;

            }
        }
    }else if(fall == true){
        for (auto &obj : scene.objects) {
            auto vrtula = dynamic_cast<Vrtula*>(obj.get());
            if(vrtula && vrtula->id == id){
                vrtula->position = position;
            }
        }
        if (position.z >= 5){
            kvapky(scene,15);

        }
        speed.z += 10 * dt;
    }

  generateModelMatrix();

  return true;
}

void Helikoptera::explode(Scene &scene, vec3 explosionPosition, vec3 explosionScale, int pieces) {
  // Generate explosion
  auto explosion = make_unique<Explosion>();
  explosion->position = explosionPosition;
  explosion->scale = explosionScale;
  explosion->speed = speed / 2.0f;
  scene.objects.push_back(move(explosion));

  // Generate smaller asteroids

}

void Helikoptera::kvapky(Scene &scene, int pieces){
    for (int i = 0; i < pieces; i++) {
        auto kvapka = make_unique<Kvapka>();

        kvapka->position = position;


        scene.objects.push_back(move(kvapka));
    }
}

void Helikoptera::render(Scene &scene) {
  shader->use();

  // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightDirection2", scene.lightDirection2);

    shader->setUniform("intenzita", scene.intensity);
    shader->setUniform("intenzita2", scene.intensity2);


    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

