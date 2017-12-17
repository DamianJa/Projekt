#pragma once
#include <memory>
#include <glm/gtc/random.hpp>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "vrtula.h"
#include "object.h"

/*!
 * Simple asteroid object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 * It will move down along the Y axis and self delete when reaching below -10
 */
class Helikoptera final : public Object {
private:
  // Static resources (Shared between instances)
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;



  // Age of the object in seconds
  float age{0.0f};
    float stupenotocenia ;

  // Speed and rotational momentum


  /*!
   * Split the asteroid into multiple pieces and spawn an explosion object.
   *
   * @param scene - Scene to place pieces and explosion into
   * @param explosionPosition - Initial position of the explosion
   * @param explosionScale - Scale of the explosion
   * @param pieces - Asteroid pieces to generate
   */
  void explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale, int pieces);
    void kvapky(Scene &scene, int pieces);

public:
    int otacanie = 0;
    int smer;
    int id;
    glm::vec3 speed;
    bool fall = false;
  /*!
   * Create new asteroid
   */
  Helikoptera();

  /*!
   * Update asteroid
   * @param scene Scene to interact with
   * @param dt Time delta for animation purposes
   * @return
   */
  bool update(Scene &scene, float dt) override;

  /*!
   * Render asteroid
   * @param scene Scene to render in
   */
  void render(Scene &scene) override;

private:
};

