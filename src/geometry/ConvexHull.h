#pragma once
#include "Polygon.h"
#include <iterator>
#include <optional>

/*
Class object for smallest enclosing polygon via Andrew's monotone chain at O(n log n) time - amChain **
*/

class ConvexHull {
    public:
        // AMChain method
        static Polygon computeHulls(std::vector<PointR2> vertices);
        // Utility methods
        static bool isValidHull(const Polygon& pgon) noexcept;
        static Polygon mergeHulls(const Polygon& hull1, const Polygon& hull2);

    private:
        static constexpr double EPS_ = 1e-14;
        static Polygon amChain_(std::vector<PointR2>vertices);
        static double orientation_(const PointR2& a, const PointR2& b, const PointR2& c) noexcept;
};
// ALGORITHMS
// computes convex hull via amChain_
Polygon ConvexHull::computeHulls(std::vector<PointR2> vertices) {
    const auto num_vertices = vertices.size();
    if (num_vertices <= 2) return Polygon{std::move(vertices)};

    std::sort(vertices.begin(),vertices.end());
    vertices.erase(std::unique(vertices.begin(),vertices.end()), vertices.end());
    if (vertices.size() <= 2) return Polygon{std::move(vertices)};
    
    return amChain_(std::move(vertices));
}
// check if hull is valid
bool ConvexHull::isValidHull(const Polygon& pgon) noexcept {
    const auto num_vertices = pgon.size();
    if (num_vertices < 3) return num_vertices > 0;

    for (std::size_t i = 0; i < num_vertices; ++i) {
        const auto& a = pgon[i];
        const auto& b = pgon[(i+1) % num_vertices];
        const auto& c = pgon[(i+2) % num_vertices];
        if (orientation_(a,b,c) < -EPS_) return false;
    }
    return true;
}    
// combines all vertices into one hull to pass to computeHulls
Polygon ConvexHull::mergeHulls(const Polygon& hull1, const Polygon& hull2) {
    if (hull1.empty()) return hull2;
    if (hull2.empty()) return hull1;

    std::vector<PointR2> sum;
    sum.reserve(hull1.size() + hull2.size()); // is preallocation always good? 

    const auto& hull1_vertices = hull1.vertices();
    const auto& hull2_vertices = hull2.vertices();

    sum.insert(sum.end(),hull1_vertices.begin(),hull1_vertices.end());
    sum.insert(sum.end(),hull2_vertices.begin(),hull2_vertices.end());

    return computeHulls(std::move(sum));
}
Polygon ConvexHull::amChain_(std::vector<PointR2> vertices) {
    std::vector<PointR2> hull;
    hull.reserve(2*vertices.size());
    /*
    LOWER - checks orientation_ and removes right turns/collinear points, adds next vertex
    */
    for (const auto& v : vertices) {
        while (hull.size() >= 2) {
            if (orientation_(hull[hull.size()-2],hull[hull.size()-1],v) > EPS_) break;
            hull.pop_back();
        }
        hull.push_back(v);
    }
    /*
    UPPER - (loops pointers in reverse order) checks orientation_ completes loop, removing possible duplicate endpoint 
    */
    const auto lower_size = hull.size();
    for (auto ri = std::next(vertices.rbegin()); ri != vertices.rend(); ++ri) {
        while (hull.size() > lower_size) {
            if (orientation_(hull[hull.size()-2],hull[hull.size()-1],*ri) > EPS_) break;
            hull.pop_back();
        }
        hull.push_back(*ri);
    }
    if (hull.size() > 1) hull.pop_back();
    hull.shrink_to_fit();
    return Polygon{std::move(hull)};
}
//orientation_ computing algorithm
double ConvexHull::orientation_(const PointR2& a, const PointR2& b, const PointR2& c) noexcept {
    return (b.x()-a.x()) * (c.y()-a.y()) - (b.y()-a.y()) * (c.x()-a.x());
}
// Utils for dynamic cacheing of \textit{in situ} hull calculation
namespace ConvexHullUtils {
    class IncrementalHull {
    public: 
        void addPoint(const PointR2& vertex) {
            vertices_.push_back(vertex);
            cached_hull_.reset();
        }
        void addPoints(const std::vector<PointR2>& vertices){
            vertices_.reserve(vertices_.size() + vertices.size());
            vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
            cached_hull_.reset();
        }
        const Polygon& getHull() const {
            if (!cached_hull_) {
                cached_hull_ = ConvexHull::computeHulls(vertices_);
            }
            return *cached_hull_;
        }
        void clear() noexcept {
            vertices_.clear();
            cached_hull_.reset();
        }
        std::size_t size() const noexcept {
            return vertices_.size();
        }
        bool empty() const noexcept {
            return vertices_.empty();
        }
    private:
        std::vector<PointR2> vertices_;
        mutable std::optional<Polygon> cached_hull_;
    };
}