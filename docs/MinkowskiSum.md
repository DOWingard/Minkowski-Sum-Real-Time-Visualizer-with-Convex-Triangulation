# Minkowski Sum

Computes the Minkowski sum of two polygons

Relevant documentation:
* https://doc.cgal.org/latest/Minkowski_sum_2/index.html

## Sum Algorithm
Makes use of operations defined on Polygons to compute the sum and return a convex hull enclosing the shape.


When the input polygons are convex, the algorithm is O(n+m) with a fan-type optimization. Upon computing the sum, a convex hull enclosing the vertices is found.

## Convex Hull Algorithm
Simple implimentation of Andrew's Monotone Chain, related documentation can be found at:
* https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain

## Convexity
Since the algorithm is O(n^2) for non-convex it makes sense to decompose each polygon into a triangulation to maintain O(n+m).
