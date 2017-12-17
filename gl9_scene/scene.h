#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
  public:
    /*!
     * Update all objects in the scene
     * @param time
     */

    void update(float time);

    /*!
     * Render all objects in the scene
     */
    void render();

    // Camera object
    std::unique_ptr<Camera> camera;
    //Camera camera;

    // All objects to be rendered in scene
    std::list< std::unique_ptr<Object> > objects;

    // Keyboard state
    std::map< int, int > keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{-40.0f, -10.0f, -80.0f};
    glm::vec3 lightDirection2{40.0f, -10.0f, -80.0f};
    float intensity = 0.7f;
    float intensity2 = 0.3f;
    int nextlvl = 0;


    // Store cursor state
    struct {
      double x, y;
      bool left, right;
    } cursor;
};

#endif // _PPGSO_SCENE_H
