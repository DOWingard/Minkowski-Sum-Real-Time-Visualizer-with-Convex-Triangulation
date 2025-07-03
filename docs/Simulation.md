# Simulation

I mostly took on this project to focus on the core algorithms, so rendering and interactions are somewhat of a means to an end so one can visually interact with the system.

## Simulation Interactions
* Controls how to cast polygon objects into a rendered scene
* Makes sure the Minkowski Sum is computed on relevant mouse interactions
* Commented off is the required code to change moving vertices to moving entire polyons
* Clamps vertex location to a subwindow to ensure vertices are not dragged beyond the threshold off screen and no longer "grabbable"

## Simulation Rendering
Renders both fills and outlines. Renders with "GL_TRIANGLE_FAN", so non-convex shapes get decomposed and the constituent triangles are filled.

Import features:
* Utility functions for rendering shapes, managing the scene, etc...
* Smart destructors for cleanup
