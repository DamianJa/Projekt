#include "player.h"
#include "scene.h"
#include "helikoptera.h"
#include "vyhraObrazovka.h"
#include "prehraObrazovka.h"
#include "projectile.h"
#include "explosion.h"
#include "kvapka.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// shared resources
unique_ptr<Mesh> Player::mesh;
unique_ptr<Texture> Player::texture;
unique_ptr<Shader> Player::shader;




Player::Player() {
  // Scale the default model
  scale *= 0.18f;
  rotation.x = -1.5;
    rotation.y = +0.025f;


  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("jet.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("liee.obj");
}

bool Player::update(Scene &scene, float dt) {


  if(position.y > 900.0f){
    vyhra = 1;
    if(scene.nextlvl == 0){
      scene.nextlvl =1;
    }
  }


  if(vyhra == 0) {
    shader->setUniform("dt", dt);
    // Fire delay increment
    fireDelay += dt;
    position += speed * dt;





    // printf("%d\n",position.y);
    // Hit detection

    if (position.z >= 5) {
      //kvapky(scene, 15);
        //auto obj = make_unique
       // PrehraMenu a = new PrehraMenu();


        auto obj = make_unique<PrehraMenu>();
        scene.objects.push_back(move(obj));
      return false;

    }

    if (position.x >= 21.61f || position.x <= -21.61f) {
      auto explosion = make_unique<Explosion>();
      explosion->position = position;
      explosion->scale = scale * 30.0f;
      scene.objects.push_back(move(explosion));
      speed.x = linearRand(+8.0, -8.0);
      speed.z = linearRand(1.0, 8.0);
    }

    for (auto &obj : scene.objects) {
      // Ignore self in scene
      if (obj.get() == this)
        continue;


      // We only need to collide with asteroids, ignore other objects
      auto asteroid = dynamic_cast<Helikoptera *>(obj.get());


      if (!asteroid) continue;

      if (distance(position.y, asteroid->position.y) < asteroid->scale.y &&
          (position.z, asteroid->position.z) < asteroid->scale.z &&
          ((asteroid->smer == 0 && asteroid->position.x + 7.16f > position.x &&
            asteroid->position.x - 5.37f < position.x) ||
           (asteroid->smer == 1 && asteroid->position.x - 7.16f < position.x &&
            asteroid->position.x + 5.37f > position.x))) {
        // Explode
        auto explosion = make_unique<Explosion>();
        explosion->position = position;
        asteroid->fall = true;
        explosion->scale = scale * 30.0f;
        scene.objects.push_back(move(explosion));
        //position.y -= 12*dt;
        // Die

        speed.x = linearRand(+8.0, -8.0);
        speed.z = linearRand(1.0, 8.0);
      }
    }

    // Keyboard controls
    if (scene.keyboard[GLFW_KEY_LEFT]) {

      position.x += 10 * dt;
      rotation.z = -PI / 4.0f;
    } else if (scene.keyboard[GLFW_KEY_RIGHT]) {
      position.x -= 10 * dt;
      rotation.z = PI / 4.0f;
    } else {
      rotation.z = 0;
    }

    // Firing projectiles
    if (scene.keyboard[GLFW_KEY_SPACE] && fireDelay > fireRate) {
      // Reset fire delay
      fireDelay = 0;
      // Invert file offset
      fireOffset = -fireOffset;

      auto projectile = make_unique<Projectile>();
      projectile->position = position + glm::vec3(0.0f, 0.0f, 0.3f) + fireOffset;
      scene.objects.push_back(move(projectile));
    }


    scene.camera->position += speed * dt;
    scene.camera->position.x = position.x;
    scene.camera->update(dt);
    //shader->setUniform("LightDirection", scene.lightDirection);
    //shader->setUniform("LightDirection2",position);
    generateModelMatrix();
    return true;

  }else {
      auto obj = make_unique<VyhraMenu>();
       scene.objects.push_back(move(obj));

  }
}
void Player::kvapky(Scene &scene, int pieces){
    for (int i = 0; i < pieces; i++) {
        auto kvapka = make_unique<Kvapka>();

        kvapka->position = position;


        scene.objects.push_back(move(kvapka));
    }
}

void Player::render(Scene &scene) {
  shader->use();

  // Set up light
  //  shader->setUniform("Transparency", scene.transparency);
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
