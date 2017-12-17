//
// Created by Damián on 14.11.2017.
//

#include <glm/gtc/random.hpp>
#include "prvamapa.h"
#include "projectile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Prvamapa::mesh;
unique_ptr<Texture> Prvamapa::texture;
unique_ptr<Shader> Prvamapa::shader;

Prvamapa::Prvamapa() {
    // Set random scale speed and rotation
    scale *= 5.0f;
   // speed = {linearRand(-2.0f, 2.0f), linearRand(-5.0f, -10.0f), 0.0f};
    rotation.x = -1.57080f;
   // rotation.z = 3;
    position.z = 10.5f;
    
    //rotMomentum = ballRand(PI);

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("mapa2.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("mapa3.obj");
}

bool Prvamapa::update(Scene &scene, float dt) {
    // Count time alive
    generateModelMatrix();


    return true;
}

void Prvamapa::explode(Scene &scene, vec3 explosionPosition, vec3 explosionScale, int pieces) {
    // Generate explosion
    auto explosion = make_unique<Explosion>();
    explosion->position = explosionPosition;
    explosion->scale = explosionScale;
    explosion->speed = speed / 2.0f;
    scene.objects.push_back(move(explosion));

    // Generate smaller asteroids
    for (int i = 0; i < pieces; i++) {
        auto prvamapa = make_unique<Prvamapa>();
        prvamapa->speed = speed + vec3(linearRand(-3.0f, 3.0f), linearRand(0.0f, -5.0f), 0.0f);;
        prvamapa->position = position;
        prvamapa->rotMomentum = rotMomentum;
        float factor = (float) pieces / 2.0f;
        prvamapa->scale = scale / factor;
        scene.objects.push_back(move(prvamapa));
    }
}

void Prvamapa::render(Scene &scene) {
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

