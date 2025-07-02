#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

/*
classes defined for elements of R^2, polygon enclosing regions in R^2, Convex Hull set 
*/

// points in R2 with vector operations

class PointR2 : public std::vector<double> {
    private:
        static constexpr double EPSILON_ = 1e-14;

    public:
    
    PointR2(double x = 0.0, double y = 0.0) : std::vector<double>{x,y} {}
        // component fxns
        double& x() {
            return (*this)[0];
        }
        double& y() {
            return (*this)[1];
        }
        // read only references
        const double& x() const {
            return (*this)[0];
        }
        const double& y() const {
            return (*this)[1];
        }

        // VECTOR OPERATIONS

        // addition/subtraction
        PointR2 operator+(const PointR2& vec) const {
            return {
                x() + vec.x(), y() + vec.y()
            };
        }

        PointR2 operator-(const PointR2& vec) const {
            return {
                x() - vec.x(), y() - vec.y()
            };
        }
        // compound addition/subtraction
        PointR2 operator+=(const PointR2& vec) {
            x() += vec.x(); y() += vec.y(); 
            return *this;
        }
        PointR2 operator-=(const PointR2& vec) {
            x() -= vec.x(); y() -= vec.y(); 
            return *this;
        }
        // multiplication
        PointR2 operator*(double scalar) const {
            return {
                scalar * x(), scalar * y()
            };
        }
        // division
        PointR2 operator/(double scalar) const {
            return {
                x() / scalar, y() / scalar
            };
        }
        // inner product
        double dot(const PointR2& vec) const {
            return x() * vec.x() + y() * vec.y();
        }
        // outer product -> scalar
        double cross(const PointR2& vec) const {
            return x() * vec.y() - y() * vec.x();
        }
        // L2-norm
        double norm() const {
            return std::sqrt(dot(*this));
        }
        // norm^2
        double norm2() const {
            return dot(*this);
        }
        // normalize vector
        PointR2 normalize() const {
            double nrm = norm();
            if (nrm < EPSILON_) {
                return {0.0, 0.0};
            }
            return {
                x() / nrm, y() / nrm
            };
        }
        // metric functions
        double ds(const PointR2& vec) const {
            return (*this -vec).norm();
            }
        double ds2(const PointR2& vec) const {
            return (*this -vec).norm2();
        }
        // polar angle
        double angle() const {
            return std::atan2(y(),x());
        }
        // Boolean operators
        bool operator!=(const PointR2& vec) const {
            return !(*this == vec);
        }
        bool operator==(const PointR2& vec) const {
            const double eps = EPSILON_;
            return std::abs(x() - vec.x()) <= eps && std::abs(y() - vec.y()) <= eps;
        }
        // ordering x > y
        bool operator<(const PointR2& vec) const{
            const double eps = EPSILON_;
            if (std::abs(x()-vec.x()) > eps) 
            return x() < vec.x();
            return y() < vec.y();
        }
        // print functionality
        friend std::ostream& operator<<(std::ostream& os,const PointR2& vec){
            os << "(" << vec.x() << "," << vec.y() << ")";
            return os;
        }
        // given a,b,c /in R^2, returns >0 is c left of the line ab
        static double orientation(const PointR2& a, const PointR2& b, const PointR2& c) {
            return (b-a).cross(c-a);
        }
        
        // collinearity
        static bool collinear(const PointR2& a, const PointR2& b, const PointR2& c) {
            return std::abs(orientation(a,b,c)) < EPSILON_;
        }
        // angle check 
        static bool checkAngle(const PointR2& a, const PointR2& b, const PointR2& pivot) {
            const PointR2 va = a - pivot;
            const PointR2 vb = b - pivot;
            const double cross_prod = va.cross(vb);
            
            if (std::abs(cross_prod) < EPSILON_) {
                return va.norm2() < vb.norm2();
            }
            return cross_prod > 0;
        }

        static void sortAngle(std::vector<PointR2>& points, const PointR2& pivot) {
            std::sort(points.begin(), points.end(), 
                [&pivot](const PointR2& a, const PointR2& b) {
                    return checkAngle(a, b, pivot);
                }
            );
        }
        // Grab Sign
        inline static int sign(double number) {
            return (number < 0) ? -1 : 1;
        }
};





