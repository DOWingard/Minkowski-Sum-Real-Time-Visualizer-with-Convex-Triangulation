#pragma once
#include "../geometry/ConvexHull.h"
/*
 * THIS DOES NOT WORK, MIGHT FIX LATER, MIGHT NOT
 *
 *
Convex decomposition to avoid O(n^2) for nonconvex case with ear clipping 
Uses Bayazit Algorithm -> 'https://mpen.ca/406/bayazit'
*/
class ConvexDecomposition {
public:
    static std::vector<Polygon> decompose(const Polygon& pgon);

private:
    static bool isReflex_(const Polygon& pgon, int i);                          // check if vertex is a reflex vertex
    static double cross_(const PointR2& a, const PointR2& b, const PointR2& c); // cross product -> scalar for 2d
    static bool left_(const PointR2& a, const PointR2& b, const PointR2& c);    // check if vec2 is left of vec1
    static bool right_(const PointR2& a, const PointR2& b, const PointR2& c);   // check if vec2 is right of vec1
    static bool canSee_(const Polygon& pgon, int i, int j);                     // check if vertex1 sees vertex2
    static std::vector<Polygon> Bayazit_(const Polygon& pgon);                  // MAIN ALGORITHM
};

inline std::vector<Polygon> ConvexDecomposition::decompose(const Polygon& pgon) {
    return Bayazit_(pgon);
}
// Given lines AB, BC, computes 2d cross product AB x AC ie area vector
inline double ConvexDecomposition::cross_(const PointR2& a, const PointR2& b, const PointR2& c) {
    PointR2 v1 = b - a;
    PointR2 v2 = c - a;
    return v1.cross(v2);
}
inline bool ConvexDecomposition::left_(const PointR2& a, const PointR2& b, const PointR2& c) {
    return cross_(a,b,c) > 0;
}
inline bool ConvexDecomposition::right_(const PointR2& a, const PointR2& b, const PointR2& c) {
    return cross_(a,b,c) < 0;
}
inline bool ConvexDecomposition::isReflex_(const Polygon& pgon, int i) {
    const int n = static_cast<int>(pgon.size());
    const PointR2& prev = pgon[(i + n - 1) % n];
    const PointR2& curr = pgon[i];
    const PointR2& next = pgon[(i + 1) % n];
    double winding = pgon.area();
    return (winding > 0) ? right_(prev, curr, next) : left_(prev, curr, next);
}
inline bool ConvexDecomposition::canSee_(const Polygon& pgon, int i, int j) {
    if (i == j || (j+1) % pgon.size() == i || (i+1) % pgon.size() == j)
    return false;

    int num_vertices = static_cast<int>(pgon.size());
    const PointR2& a = pgon[i];
    const PointR2& b = pgon[j];
    // helper lambda to check intersection
    auto segmentsIntersect = [](const PointR2& a,const PointR2& b,const PointR2& c,const PointR2& d) -> bool {
        auto ccw = [](const PointR2& A, const PointR2& B, const PointR2& C) {
            return (C.y()-A.y()) * (B.x()-A.x()) > (B.y()-A.y()) * (C.x()-A.x());
        };
        return ccw(a,c,d) != ccw(b,c,d) && ccw(a,b,c) != ccw(a,b,d);
    };
    for (int k = 0; k < num_vertices; ++k) {
        int k1 = k;
        int k2 = (k+1) % num_vertices;
        if ((k1 == i || k2 == i) || (k1 == j || k2 == j)) continue;
        if (segmentsIntersect(a,b,pgon[k1],pgon[k2])) {
            return false;
        }
    }

    if (isReflex_(pgon,i)) {
        return !left_(pgon[(i-1+num_vertices) % num_vertices], pgon[i], pgon[j]) &&
                !right_(pgon[(i+1) % num_vertices], pgon[i], pgon[j]);
    } else {
        return left_(pgon[(i-1+num_vertices) % num_vertices], pgon[i], pgon[j]) &&
                right_(pgon[(i+1) % num_vertices], pgon[i], pgon[j]);
    }
}
inline std::vector<Polygon> ConvexDecomposition::Bayazit_(const Polygon& pgon) {
    if (pgon.size() < 3) return {};
    
    std::vector<Polygon> result;
    Polygon vertices = pgon;

    while (true) {
        const int num_vertices = static_cast<int>(vertices.size());
        bool splitDone = false;

        for (int i = 0; i < num_vertices; ++i) {
            if (!isReflex_(vertices,i)) continue;
            for (int j = 0; j < num_vertices; ++j) {
                if (j == i || (j+1) % num_vertices == i || (i+1) % num_vertices == j) continue;
                if (!canSee_(vertices,i,j)) continue;

                Polygon p1, p2;
                
                const int start = std::min(i,j);
                const int end = std::max(i,j);
                const int p1_size = end-start+1;
                const int p2_size = num_vertices - p1_size + 2;

                p1.reserve(p1_size);
                p2.reserve(p2_size);

                int idx = i;
                do {
                    p1.push_back(vertices[idx]);
                    idx = (idx+1) % num_vertices;
                } while (
                    idx != (j+1) % num_vertices
                );
                int jdx = j;
                do {
                    p2.push_back(vertices[jdx]);
                    jdx = (jdx+1) % num_vertices;
                } while (
                    jdx!= (i+1) % num_vertices
                );

                if (p1.area() > 1e-6 && p2.area() > 1e-6){   //reasonable cutoff
                    auto d1 = Bayazit_(p1);
                    auto d2 = Bayazit_(p2);
                    result.insert(result.end(), d1.begin(), d1.end());
                    result.insert(result.end(), d2.begin(), d2.end());
                    splitDone = true;
                    break;
                } 
            }
            if (splitDone) break;
        }
        if (!splitDone) {
            result.push_back(vertices);
            break;
        }
    }
    return result;
}