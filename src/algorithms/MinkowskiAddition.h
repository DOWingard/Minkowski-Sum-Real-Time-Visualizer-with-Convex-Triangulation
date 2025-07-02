#pragma once
#include "ConvexDecomp.h"

/*
Minkowski sum computation for polygons in R^2
*/

class MinkowskiSum {
public:
    // essential polygon methods
    static Polygon computeSum(const Polygon& pgon1, const Polygon& pgon2);
    static Polygon computeDifference(const Polygon& pgon1, const Polygon& pgon2);
    // intersection test
    static bool intersects(const Polygon& pgon1, const Polygon& pgon2);
private:
    static constexpr double EPS_ = 1e-14 ;
    // private polygon functions
    static Polygon computeConvex_(const Polygon& pgon1, const Polygon& pgon2);
    static std::vector<PointR2> getEdgeVectors_(const Polygon& pgon);
    static Polygon reflect_(const Polygon& pgon);
};
// MAIN ALGORITHMS
// Minkowski sum A + B - optimized if it finds convex polygons  ***
Polygon MinkowskiSum::computeSum(const Polygon& polygon1, const Polygon& polygon2) {
    Polygon pgon1 = polygon1;
    Polygon pgon2 = polygon2;
    pgon1.forceCCW();
    pgon2.forceCCW();
    if (pgon1.empty() || pgon2.empty() ) return Polygon{}; 
    if (pgon1.isConvex() && pgon2.isConvex()) {
        
        return computeConvex_(pgon1,pgon2);
    }
    // Decompose them if they aren't convex
    std::vector<Polygon> decomp1 = pgon1.isConvex() ? std::vector<Polygon>{pgon1}
                                                    : ConvexDecomposition::decompose(pgon1);
    std::vector<Polygon> decomp2 = pgon1.isConvex() ? std::vector<Polygon>{pgon2}
                                                    : ConvexDecomposition::decompose(pgon2);
    // Apply sum to decomposed polygons
    std::vector<PointR2> Points;
    for (const Polygon& polygon_piece_1 : decomp1) {
        for (const Polygon& polygon_piece_2 : decomp2) {
            Polygon partialSum = computeConvex_(polygon_piece_1,polygon_piece_2);
            const auto& verts = partialSum.vertices();
            Points.insert(Points.end(), verts.begin(), verts.end());
        }
    }
    // find convex hull of merged set of vertices
    return ConvexHull::computeHulls(std::move(Points));
}
// Minkowski difference A - B - computes Minkowski sum A + (-B)
Polygon MinkowskiSum::computeDifference(const Polygon& pgon1, const Polygon& pgon2) {
    return computeSum(pgon1, reflect_(pgon2));
}
// Intersection test
bool MinkowskiSum::intersects(const Polygon& pgon1, const Polygon& pgon2) {
    return computeDifference(pgon1, pgon2).hasPoint(PointR2{0.0, 0.0}); // any intersecting vertices return (0,0)
}
// *** convex polygon algorithm - O(n+m)
Polygon MinkowskiSum::computeConvex_(const Polygon& pgon1, const Polygon& pgon2) {
    const auto& vertices_1 = pgon1.vertices();
    const auto& vertices_2 = pgon2.vertices();
    if (vertices_1.size() <= 1) {
        Polygon result = pgon2;
        if (!vertices_1.empty()) result.translate(vertices_1[0]);
        return result;
    }
    if (vertices_2.size() <= 1) {
        Polygon result = pgon1;
        if (!vertices_2.empty()) result.translate(vertices_2[0]);
        return result;
    }
    auto edges_1 = getEdgeVectors_(pgon1);
    auto edges_2 = getEdgeVectors_(pgon2);
    std::vector<PointR2> result;
    result.reserve(edges_1.size() + edges_2.size());

    PointR2 current = vertices_1[0] + vertices_2[0];
    result.push_back(current);

    size_t i1 = 0, i2 = 0;
    while (i1 < edges_1.size() && i2 < edges_2.size()) {
        double cross = edges_1[i1].cross(edges_2[i2]) ; // more like doublecrossing my beloved Fortran :(
        if (cross > EPS_) {
            current += edges_1[i1++];
        }
        else if (cross < -EPS_) {
            current += edges_2[i2++];
        }
        else {
            current += edges_1[i1++] + edges_2[i2++];
        }
        result.push_back(current);
    }

    while (i1 < edges_1.size()) {
        current += edges_1[i1++];
        result.push_back(current);
    }
    while (i2 < edges_2.size()) {
        current += edges_2[i2++];
        result.push_back(current);
    }
    if (!result.empty()) result.pop_back(); 
    return Polygon{std::move(result)};
}
// get the edges  as vertex pairs
std::vector<PointR2> MinkowskiSum::getEdgeVectors_(const Polygon& pgon) {
    const auto& vertices = pgon.vertices();
    std::vector<PointR2> edges ;
    edges.reserve(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        const size_t i_next = (i+1) % vertices.size();
        edges.push_back(vertices[i_next] - vertices[i]);
    }
    return edges;
}
// reflect polygon through point
Polygon MinkowskiSum::reflect_(const Polygon& pgon) {
    const auto& vertices = pgon.vertices();
    std::vector<PointR2> reflected;
    reflected.reserve(vertices.size());
    for (auto ri = vertices.rbegin(); ri != vertices.rend(); ++ri) {
        reflected.emplace_back(-ri->x(), -ri->y());
    }
    return Polygon{std::move(reflected)};
}
// Utils
namespace MinkowskiUtils {
    // Scales polygon uniformly by wrapping polygon function
    inline Polygon scale(Polygon& pgon, const double scale) {
        if (scale == 0.0) return pgon;
        pgon.scale(scale, pgon.centroid());
    }
    // Robotics configuration space generating function
    // given input shape of "robot", all objects in the plane space can be scaled so
    // robot trajectory can be treated as point trajectory through configuration space for collision detection
    inline Polygon configRepresentation(const Polygon& object, const Polygon& robot) {
        return MinkowskiSum::computeDifference(object,robot);
    }

}

