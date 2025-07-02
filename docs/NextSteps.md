# Next Steps
There are a few key bottlenecks that point to obvious next steps:

## Ear Clipping Triangulation
Ear clipping is not the best method of decomposing a polygon into convex polygons. Its not even the best method of triangulation, but in ConvexDecomp2.h, I have built a Bayazit algorithm. It does not work in its current form, but getting it running instead of ear clipping is a clear improvement that could be made.

## Dynamic Clamping
Making sure the vertices can't be moved off screen is great and easy. Making sure you can't move the vertices in such a way that the outline self-intersects, especially when grabbing a reflex vertex, is a very nuanced problem. This would be not only a difficult next step, but also a very interesting one. I know the solution is either really elegant or really literal. 

## Extend to 3-D
Obviously, extending this to work on a PointR3 system instead of PointR2 would be a great next step, but to call that a single step is a bit disrespectful of all it actually entails.

