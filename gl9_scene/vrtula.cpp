#include <glm/gtc/random.hpp>
#include "vrtula.h"
#include "projectile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Vrtula::mesh;
unique_ptr<Texture> Vrtula::texture;
unique_ptr<Shader> Vrtula::shader;

Vrtula::Vrtula() {
    // Set random scale speed and rotation

    scale *= 1.0f;
    speed = {0.0f, 0.0f, 0.0f};
    rotation = vec3(0.0f,+1.57080f,-1.57080f);
    //rotation = ballRand(PI);
    //rotMomentum = ballRand(PI);

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("vrtula.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("vrtula1.obj");
}

bool Vrtula::update(Scene &scene, float dt) {

    age += dt;




   //     rotation.y = glm::rotate( 2*PI/180, position );

        rotation.x += dt * 10;

        // Delete when alive longer than 10s or out of visibility
        if (age > 8.0f) return false;




    generateModelMatrix();

    return true;
}

void Vrtula::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}
void Vrtula::destroy(){
    fall = true;
}

