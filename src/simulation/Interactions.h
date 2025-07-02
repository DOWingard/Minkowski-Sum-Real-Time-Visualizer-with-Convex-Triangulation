#pragma once
#include "../algorithms/MinkowskiAddition.h"
#include <glm/gtx/norm.hpp>

/*
 * Interactions class to handle controlling the simulator with the mouse
 * and real time updating
 */

class Interactions {
    // initialize interaction objects
    std::vector<Polygon> polygons_; 
    int selectedPolygon_ = -1;
    std::pair<int,int> selectedVertex_{-1,-1};
    PointR2 lastMousePos_;
    static constexpr double threshold_ = 0.25; // controls search range 
    
    // grab closest object within threshold and do stuff
    // 
    bool inPolygon(const PointR2& point, const Polygon& pgon) const {
        const auto& vertices = pgon.vertices();
        int crossings = 0;
        for (int i = 0, n = vertices.size(); i < n; ++i) {
            const auto& v1 = vertices[i];
            const auto& v2 = vertices[(i+1) % n];
            if (((v1.y() > point.y()) != (v2.y() > point.y())) &&
                (point.x() < (v2.x()-v1.x()) * (point.y()-v1.y()) / (v2.y()-v1.y()) + v1.x()))
                ++crossings;
        }
        return crossings % 2 == 1;
    }

    void reSum() {
        if (polygons_.empty()) return;
        Polygon sum = polygons_[0];
        for (size_t i = 1; i < polygons_.size(); ++i) {
            sum = MinkowskiSum::computeSum(sum,polygons_[i]);
        }
        Sum_ = std::move(sum);
    }
    Polygon Sum_;

public:
    void addPolygon(const Polygon& pgon) {
        polygons_.push_back(pgon);
        reSum();
    }
    bool mouseClick(double x, double y) {
        PointR2 pos{x,y};
        // selectedPolygon_ = -1;    // moving whole polygons == bad for minkowski sum
        // for (int i = 0; i < int(polygons_.size()); ++i) {
        //     if (inPolygon(pos, polygons_[i])) {
        //         selectedPolygon_ = i;
        //         lastMousePos_ = pos;
        //         return true;
        //     }
        // }
        selectedVertex_ = {-1,-1};
        double thresh2_ = threshold_ * threshold_;
        for (int i = 0; i < int(polygons_.size()); ++i) {
            auto& vertices = polygons_[i].vertices();
            for (int j = 0; j < int(vertices.size()); ++j) {
                double d2 = glm::distance2(
                    glm::vec2(vertices[j].x(), vertices[j].y()),
                    glm::vec2(pos.x(), pos.y())
                );
                if (d2 < thresh2_) {
                    thresh2_ = d2;
                    selectedVertex_ = {i,j};
                }
            }
        }
        if (selectedVertex_.first >= 0) {
            lastMousePos_ = pos;
            return true;
        }
        return false;
    }
    /*
    & locks vertices within window or else the can get dragged offscreen and 
    if further than threshold_ they're ungrabbable

    attempted to maximize speed and efficiency since this would me calculated on all mouse dragging
    * only 1 sqrt used
    * 
    */ 
    void mouseDrag(double x,double y) {
        // if (selectedPolygon_ == -1) return;
        // PointR2 pos{x,y};
        // PointR2 pos_change = pos - lastMousePos_;
        // lastMousePos_ = pos;

        // auto& pgon = polygons_[selectedPolygon_];
        // auto& vertices = pgon.vertices();
        // for (auto& vertex : vertices) {
        //     vertex += pos_change;
        // }
        auto [i, j] = selectedVertex_;
        if (i < 0) return;

        PointR2 pos{x,y};
        PointR2 dp = pos - lastMousePos_;
        lastMousePos_ = pos;

        auto& vertices = polygons_[i].vertices();
        PointR2 newPos = vertices[j] + dp;

        //create setter functions to clamp coords 
        // clamp to within [100,1100] x [100,1100] so no vertices get "lost" beyond threshold^2 out of window
        // map([100,1100])-> Range{-1,1} ==> approx [-0.85, 0.85]
        double scaled_x = std::clamp(newPos.x(),-0.85,0.85);
        double scaled_y = std::clamp(newPos.y(),-0.85,0.60); // top of screen gave me problems w/e hardcode solution hehe


        /*
        Dynamic boundary generation, especially for reflex points is ideal to avoid intersections
        Attempted, much more sophisticated problem than the rest of this code
        !!!!!!!!!!!!!!!!!!!!!!!!! TO DO IN FUTURE (OR NOT)
        */
        vertices[j] = PointR2{scaled_x,scaled_y};
        reSum();
    }

    void mouseRelease() {
        //selectedPolygon_ = -1;
        selectedVertex_ = {-1,-1};
    }
    const std::vector<Polygon>& polygons() const {
        return polygons_;
    }
    const Polygon& Sum() const {
        return Sum_;
    }
    
};