#pragma once
#include "Geometry.h"

/*
Polygon class - representing as ordered sequence of vertices, where orientation is defined by (counter)clockwise in RHCS
 */

class Polygon {
    private:
        std::vector<PointR2> vertices_;
        static constexpr double EPS_ = 1e-14;

    public:
    // Constructors
    Polygon() {}
    Polygon(const std::vector<PointR2>& vertices) : vertices_(vertices) {}
    // Accessors for .vertices() 
    std::vector<PointR2>& vertices() {
        return vertices_;
    }
    const std::vector<PointR2>& vertices() const {
        return vertices_;
    }
    // BASIC OPERATIONS
    // check size
    size_t size() const {
        return vertices_.size();
    }
    // check empty
    bool empty() const {
        return vertices_.empty();
    }
    // access components
    PointR2& operator[](size_t i) {
        return vertices_[i];
    }
    const PointR2& operator[](size_t i) const {
        return vertices_[i];
    }
    // add vertex to end of array
    void addVertex(const PointR2& vec) {
        vertices_.push_back(vec);
    }
    // insert vertex at specific index idx
    void insertVertex(size_t idx, const PointR2& vec) {
        vertices_.insert(vertices_.begin() + idx, vec);
    }
    // remove vertex
    void removeVertex(size_t idx) {
        if (idx < vertices_.size()) {
            vertices_.erase(vertices_.begin() + idx);
        }
    }
    // clear vertices
    void clear() {
        vertices_.clear();
    }
    // GEOMETRIC OPERATIONS
    // polygon area with Shoelace formula
    double signedArea() const{
        if (vertices_.size() < 3) return 0.0;
        double area = 0.0;
        for (size_t i = 0; i < vertices_.size(); ++i) {
            size_t next_vertex = (i+1) % vertices_.size();
            area += vertices_[i].cross(vertices_[next_vertex]);
        }
        return area / 2.0; 
    }
    // signed area
    double area() const {
        if (vertices_.size() < 3) return 0.0;
        
        return std::abs(signedArea());
    }
    // check if surface is oriented via CCW point ordering
    bool isCCW() const{
        return signedArea() > 0;
    }
    // calculate centeroid of polygon as a PointR2 class object
    PointR2 centroid() const {
        if (vertices_.empty()) return PointR2(0,0);
        PointR2 center(0,0);
        for (const auto& vertex: vertices_) {
            center += vertex;
        }
        return center / static_cast<double>(vertices_.size());
    }
    // force CCW orientation
    void forceCCW() {
        if (!isCCW()){
            std::reverse(vertices_.begin(), vertices_.end());
        }
    }
    // check if convex
    bool isConvex() const {
        if (vertices_.size() < 3) return false;
        bool hasPositive = false; 
        bool hasNegative = false;
        const auto num_vertices = vertices_.size();
        for (size_t i = 0; i < num_vertices; ++i ) {
            const PointR2 a = vertices_[i];
            const PointR2 b = vertices_[(i+1) % num_vertices];
            const PointR2 c = vertices_[(i+2) % num_vertices];
            double cross = PointR2::orientation(a,b,c);
            if (cross > EPS_ ) hasPositive = true;
            if (cross < -EPS_) hasNegative = true;
            if (hasPositive && hasNegative) return false;
        }
        return true;
    }
    // check if point is in polygon
    bool hasPoint(const PointR2& vertex) const {
        if (vertices_.size() < 3) return false;
        bool inside = false;
        const auto num_vertices = vertices_.size();
        for (size_t i = 0, j = num_vertices - 1; i < num_vertices; j = i++){
            const PointR2& vi = vertices_[i];
            const PointR2& vj = vertices_[j];
            if (((vi.y() > vertex.y()) != (vj.y() > vertex.y())) && 
                (vertex.x() < (vj.x()-vi.x()) * (vertex.y() - vi.y()) / (vj.y() - vi.y()) + vi.x())) {
                    inside = !inside;
            }
        }
        return inside;
    }
    // return polygon edges as pairs of vectors 
    std::vector<std::pair<PointR2,PointR2>> getEdges() const {
        std::vector<std::pair<PointR2, PointR2>> edges;
        if (vertices_.size() < 2) return edges;
        edges.reserve(vertices_.size()); // I miss Fortran :(
        for (size_t i = 0; i < vertices_.size(); ++i) {
            const size_t next_vertex = (i+1) % vertices_.size();
            edges.emplace_back(vertices_[i], vertices_[next_vertex]);
        }
        return edges;
    }
    // find smallest enclosing bounding box
    struct BoundingBox {
        PointR2 min,max;
        bool valid;
        BoundingBox(): valid(false) {}
        BoundingBox(const PointR2& min, const PointR2& max) : min(min), max(max), valid(true) {}
    };
    BoundingBox getBoundingBox() const {
        if (vertices_.empty()) return BoundingBox();
        PointR2 minpoint = vertices_[0];
        PointR2 maxpoint = vertices_[0];
        for (const auto& vec : vertices_) {
            minpoint.x() = std::min(minpoint.x(), vec.x());
            minpoint.y() = std::min(minpoint.y(), vec.y());
            maxpoint.x() = std::max(maxpoint.x(), vec.x());
            maxpoint.y() = std::max(maxpoint.y(), vec.y());
        }
        return BoundingBox(minpoint,maxpoint);
    }
    // TRANSFORMATIONS
    // translations
    void translate(const PointR2& shift) {
        for (auto& vertex : vertices_) {
            vertex += shift;
        }
    }
    // scaling
    void scale(double scalar, const PointR2& center = PointR2(0.0,0.0)) {
        for (auto& vertex : vertices_) {
            vertex = center + (vertex - center) * scalar;
        }
    }
    // rotating
    void rotate(double angle, const PointR2& center = PointR2(0.0,0.0)) {
        double cos_phi = std::cos(angle);
        double sin_phi = std::sin(angle);
        for (auto& vertex : vertices_){
            PointR2 vec = vertex - center;
            vertex = center + PointR2(
            vec.x() * cos_phi - vec.y() * sin_phi,
            vec.x() * sin_phi + vec.y() * cos_phi
            );
        }
    }
    // Just wrap these because im lazy
    void push_back(const PointR2& vertex) {
        vertices_.push_back(vertex);
    }
    void reserve(size_t num_vertices) {
        vertices_.reserve(num_vertices);
    }

};

// SEPARATE UTILITY FUNCTIONS
// create common polygons
namespace PolygonUtils {
    // Perfect comparision, 
    // ________________________TO_DO__________________________Umeyama's algorithm (or Procrustes alignment) could be used to handle all SE(2) group
    const bool COMPARE_POLYGONS(const Polygon& Pgon1, const Polygon& Pgon2) {
        Polygon pgon1 = Pgon1;
        Polygon pgon2 = Pgon2;
        std::vector<PointR2> vertices1 = pgon1.vertices();
        std::vector<PointR2> vertices2 = pgon2.vertices();
        /*
        force Lex order
        */
        std::sort(vertices1.begin(), vertices1.end(),
            [](const PointR2& vertex1, const PointR2& vertex2) {
                return std::tie(vertex1.x(), vertex1.y())
                    < std::tie(vertex2.x(), vertex2.y());
            }
        );
        std::sort(vertices2.begin(), vertices2.end(),
            [](const PointR2& vertex1, const PointR2& vertex2) {
                return std::tie(vertex1.x(), vertex1.y())
                    < std::tie(vertex2.x(), vertex2.y());
            }
        );

        return vertices1 == vertices2;
    }
    inline Polygon createRectangle() {
        std::vector<PointR2> vertices = {
            {0.25, 0.25},
            {0.25, -0.25},
            {-0.25, -0.25},
            {-0.25, 0.25}

        };
        return Polygon(vertices);
    }
    inline Polygon createTriangle() {
        std::vector<PointR2> vertices = {
            {0.25, -0.25},
            {0.75, -0.25},
            {0.5, 0.25}
        };
        return Polygon(vertices);
    }
    inline Polygon createDecompTestShape() {
        std::vector<PointR2> vertices = {
            { 0.5,   -0.25 },          
            { 0.5,    0.25 }, 
            { 0.0,    0.00 }, 
            {-0.5,    0.25 },  
            {-0.5,   -0.25 }   
        };
        return Polygon(vertices);
    }
    inline Polygon createDecompTestResult_R() {
        std::vector<PointR2> vertices = {
            {0.0,    0.00},
            {0.5,   -0.25},
            {0.5,    0.25}
        };
        return Polygon(vertices);
    }
    inline Polygon createDecompTestResult_L() {
        std::vector<PointR2> vertices = {
            {-0.5,  -0.25},
            {-0.5,   0.25},
            { 0.0,   0.00}
            
        };
        return Polygon(vertices);
    }
    inline Polygon createDecompTestResult_C() {
        std::vector<PointR2> vertices = {
            {-0.5,  -0.25},
            { 0.0,   0.00},
            { 0.5,   -0.25}
        };
        return Polygon(vertices);
    }
    inline Polygon createMinkowskiTest() {
        std::vector<PointR2> vertices  =  {
            {0,0},
            {1,0},
            {0,1}
        };
        return Polygon(vertices);
    }
    inline Polygon createMinkowskiResult() { // Result is a convex hull
        std::vector<PointR2> vertices  =  {
            {0,0},
            {2,0},
            {0,2}
        };
        return Polygon(vertices);
    }
    inline Polygon createPolygon(const PointR2& center, double radius, int sides) {
        std::vector<PointR2> vertices;
        vertices.reserve(sides); 
        for (int i = 0; i< sides; ++i) {
            const double angle = 2.0 * M_PI * i / sides;
            vertices.emplace_back(
                center.x() + radius * std::cos(angle),
                center.y() + radius * std::sin(angle)
            );
        }
        return Polygon(vertices);
    }
    
}
