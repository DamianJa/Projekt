// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Helikoptera and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "generator.h"


#include "prvamapa.h"

using namespace std;
using namespace glm;
using namespace ppgso;

const unsigned int SIZE = 1000;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public Window {
private:
  Scene scene;
    Scene scene2;

  bool animate = true;

  /*!
   * Reset and initialize the game scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initScene() {
    scene.objects.clear();

    // Create a camera
      auto camera = make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);

      camera->position.z = - 18;
      scene.camera = move(camera);

    // Add space background




    // Add generator to scene
    auto generator = make_unique<Generator>();
    generator->position.y = 10.0f;
    scene.objects.push_back(move(generator));

    // Add player to the scene
    auto player = make_unique<Player>();
    player->position.y = -6;
    scene.objects.push_back(move(player));
    //
   // auto prvamapa = make_unique<Prvamapa>();
   // prvamapa->position = vec3(0.0f,0.0f,0.0f);
   // prvamapa->rotation.x = -1.5;
   // scene.objects.push_back(move(prvamapa));
      scene.objects.push_back(make_unique<Prvamapa>());

  }




    void initScene2() {
      scene2.objects.clear();

      // Create a camera
      auto camera = make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);

      camera->position.z = - 18;
      scene2.camera = move(camera);

      // Add space background




      // Add generator to scene
      auto generator = make_unique<Generator>();
      generator->position.y = 10.0f;
      scene2.objects.push_back(move(generator));

      // Add player to the scene
      auto player = make_unique<Player>();
      player->position.y = -6;
      scene2.objects.push_back(move(player));
      //
      // auto prvamapa = make_unique<Prvamapa>();
      // prvamapa->position = vec3(0.0f,0.0f,0.0f);
      // prvamapa->rotation.x = -1.5;
      // scene.objects.push_back(move(prvamapa));
      scene2.objects.push_back(make_unique<Prvamapa>());

    }
    void sceneClear(){
      scene.objects.clear();
    }
    void scene2Clear(){
      scene2.objects.clear();
    }

public:
  /*!
   * Construct custom game window
   */
  SceneWindow() : Window{"gl9_scene", 2000, 1400} {
    //hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    initScene();
  }

  /*!
   * Handles pressed key when the window is focused
   * @param key Key code of the key being pressed/released
   * @param scanCode Scan code of the key being pressed/released
   * @param action Action indicating the key state change
   * @param mods Additional modifiers to consider
   */
  void onKey(int key, int scanCode, int action, int mods) override {
    scene.keyboard[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      initScene();
    }

    // Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS) {

    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
      if(scene.camera->pohlad ==0){
        scene.camera->pohlad =1;
      }else{
        scene.camera->pohlad =0;
      }
    }

  }

  /*!
   * Handle cursor position changes
   * @param cursorX Mouse horizontal position in window coordinates
   * @param cursorY Mouse vertical position in window coordinates
   */
  void onCursorPos(double cursorX, double cursorY) override {
    scene.cursor.x = cursorX;
    scene.cursor.y = cursorY;
  }

  /*!
   * Handle cursor buttons
   * @param button Mouse button being manipulated
   * @param action Mouse bu
   * @param mods
   */
  void onMouseButton(int button, int action, int mods) override {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
      scene.cursor.left = action == GLFW_PRESS;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      scene.cursor.right = action == GLFW_PRESS;
    }
  }

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta
    float dt = animate ? (float) glfwGetTime() - time : 0;

    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update and render all objects
    scene.update(dt);
    scene.render();
  }
};

int main() {
  // Initialize our window
  SceneWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
