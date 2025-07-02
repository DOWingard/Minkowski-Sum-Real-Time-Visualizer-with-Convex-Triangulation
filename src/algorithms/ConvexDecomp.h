#pragma once
#include <numeric>
#include "../geometry/ConvexHull.h"

class ConvexDecomposition {
public:
    static std::vector<Polygon> decompose(const Polygon& pgon);
    static bool isReflex(const Polygon& pgon, int i);     // check if vertex is a reflex vertex -> public for mouse interactions with reflex vertices

private:
    static bool isConvex_(const PointR2& prev, const PointR2& curr, const PointR2& next, double winding);
    static bool pointInTriangle_(const PointR2& p, const PointR2& a, const PointR2& b, const PointR2& c);
    static double cross_(const PointR2& a, const PointR2& b, const PointR2& c); // cross product -> scalar for 2d
    static bool right_(const PointR2& a, const PointR2& b, const PointR2& c);   // check if vec2 is right of vec1
    static bool left_(const PointR2& a, const PointR2& b, const PointR2& c);    // you get the idea
};
// Ear Clipping 
inline std::vector<Polygon> ConvexDecomposition::decompose(const Polygon& pgon) {
    std::vector<Polygon> result;
    const int n = static_cast<int>(pgon.size());
    if (n < 3) return result;

    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);

    double winding = pgon.area(); // Positive = CCW

    while (indices.size() >= 3) {
        bool earFound = false;

        for (size_t i = 0; i < indices.size(); ++i) {
            int i0 = indices[(i + indices.size() - 1) % indices.size()];
            int i1 = indices[i];
            int i2 = indices[(i + 1) % indices.size()];

            const PointR2& a = pgon[i0];
            const PointR2& b = pgon[i1];
            const PointR2& c = pgon[i2];

            if (!isConvex_(a, b, c, winding)) continue;

            bool hasInternalPoint = false;
            for (int j : indices) {
                if (j == i0 || j == i1 || j == i2) continue;
                if (pointInTriangle_(pgon[j], a, b, c)) {
                    hasInternalPoint = true;
                    break;
                }
            }

            if (!hasInternalPoint) {
                std::vector<PointR2> points = {a, b, c };
                result.push_back(Polygon(points));
                indices.erase(indices.begin() + i);
                earFound = true;
                break;
            }
        }

        if (!earFound) break; // Shouldn't happen for valid simple polygons
    }

    return result;
}
//  //  // Taken from Bayazit_ method in ConvexDecomp2.h 
// isReflex() made public so it can be used for mouse interactions with reflex vertices

// Given lines AB, BC, computes 2d cross product AB x AC ie area vector
inline double ConvexDecomposition::cross_(const PointR2& a, const PointR2& b, const PointR2& c) {
    PointR2 v1 = b - a;
    PointR2 v2 = c - a;
    return v1.cross(v2);
}
inline bool ConvexDecomposition::right_(const PointR2& a, const PointR2& b, const PointR2& c) {
    return cross_(a,b,c) < 0;
}
inline bool ConvexDecomposition::left_(const PointR2& a, const PointR2& b, const PointR2& c) {
    return cross_(a,b,c) > 0;
}
inline bool ConvexDecomposition::isReflex(const Polygon& pgon, int i) {
    const int n = static_cast<int>(pgon.size());
    const PointR2& prev = pgon[(i + n - 1) % n];
    const PointR2& curr = pgon[i];
    const PointR2& next = pgon[(i + 1) % n];
    double winding = pgon.area();
    return (winding > 0) ? right_(prev, curr, next) : left_(prev, curr, next);
}

//  //  //
inline bool ConvexDecomposition::isConvex_(
    const PointR2& prev, const PointR2& curr, const PointR2& next, double winding) {
    PointR2 v1 = curr - prev;
    PointR2 v2 = next - curr;
    double cross = v1.cross(v2);
    return (winding > 0) ? (cross > 0) : (cross < 0);
}

inline bool ConvexDecomposition::pointInTriangle_(
    const PointR2& p, const PointR2& a, const PointR2& b, const PointR2& c) {
    auto sign = [](const PointR2& p1, const PointR2& p2, const PointR2& p3) {
        return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
    };
    bool b1 = sign(p, a, b) < 0.0;
    bool b2 = sign(p, b, c) < 0.0;
    bool b3 = sign(p, c, a) < 0.0;
    return (b1 == b2) && (b2 == b3);
}

