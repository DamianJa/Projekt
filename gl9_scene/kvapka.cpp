//
// Created by Damián on 24.11.2017.
//

#include "kvapka.h"
#include <glm/gtc/random.hpp>

#include "projectile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Kvapka::mesh;
unique_ptr<Texture> Kvapka::texture;
unique_ptr<Shader> Kvapka::shader;

Kvapka::Kvapka() {
    // Set random scale speed and rotation
    scale *= linearRand(0.1f, 0.3f);
    speed = {linearRand(-22.0f, 22.0f), linearRand(25.0f, -25.0f), linearRand(-40.0f, -65.0f)};
    rotation = ballRand(PI);


    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("kvapka01.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("asteroid.obj");
}

bool Kvapka::update(Scene &scene, float dt) {
    // Count time alive
    age += dt;

    // Animate position according to time
    position += speed * dt;
    speed.z += 5.0;

    // Rotate the object


    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.z > 6) return false;

    // Collide with scene


    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}



void Kvapka::render(Scene &scene) {
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

