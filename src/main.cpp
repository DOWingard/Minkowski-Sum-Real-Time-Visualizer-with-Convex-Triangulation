#include "simulation/Interactions.h"
#include "renderer/Renderer.h"

/*
* The whole point of the main script and renderer are to demonstrate the
* Minkowksi sum code with ear clipping triangulation for non-convex shapes
*/

// posting mouse control on main are we?
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);

// initialize scene
Renderer renderer;
Interactions scene;

// Entry Point
int main() {                            
    renderer.init(1200, 1200);

    glfwSetMouseButtonCallback(renderer.getWindow(), mouseButtonCallback);
    glfwSetCursorPosCallback(renderer.getWindow(), mousePositionCallback);

    Polygon scene_pgon1 = PolygonUtils::createDecompTestShape();
    scene_pgon1.scale(0.3);
    scene_pgon1.translate({-0.5,0.0});

    Polygon scene_pgon2 = PolygonUtils::createTriangle();
    scene_pgon2.scale(0.3, scene_pgon2.centroid());

    scene.addPolygon(scene_pgon1);
    scene.addPolygon(scene_pgon2);

    while (!renderer.shouldClose()) {
        glfwPollEvents();
        renderer.clear();

        const auto& polygons = scene.polygons();
        for (size_t i = 0; i < polygons.size(); ++i) {
                renderer.drawPolygon(polygons[i],RendererUtils::randomColor(i+1,2*i+1));
        }
        renderer.drawPolygon(scene.Sum(), glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.swapBuffers();
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
/*
* utility functions because handling scene logic here 
* is simpler for my first time C++ user brain
*/
static bool isDragging = false;
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double x,y;
        glfwGetCursorPos(window, &x, &y);
        glm::vec2 pos = RendererUtils::screenToWorld(window, x, y);
        if (action == GLFW_PRESS){
            bool mouse_bool = scene.mouseClick(pos.x,pos.y);
            isDragging = mouse_bool;
            scene.mouseClick(pos.x,pos.y);
        } 
        else if (action == GLFW_RELEASE) {
            isDragging = false;
            scene.mouseRelease();
        }
    }
}
void mousePositionCallback(GLFWwindow* window, double x, double y) {
    if (!isDragging) return;
    glm::vec2 pos = RendererUtils::screenToWorld(window, x,y);
    scene.mouseDrag(pos.x,pos.y);
}


