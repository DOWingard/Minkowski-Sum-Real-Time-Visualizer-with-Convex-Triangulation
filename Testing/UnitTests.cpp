#pragma once
#include "UnitTests.hpp"
#include "../src/algorithms/MinkowskiAddition.h"
#include <vector>
#include <list>

/*
Builds unit tests to run in test_X.cpp scripts
ASSERT_THROW_GENERAL
*/

void test_convexDecomposition() {
    //Right -> Center -> Left
    Polygon test_input = PolygonUtils::createDecompTestShape();
    Polygon expectedL  = PolygonUtils::createDecompTestResult_L();
    Polygon expectedR  = PolygonUtils::createDecompTestResult_R();
    Polygon expectedC  = PolygonUtils::createDecompTestResult_C();
    std::vector<Polygon> decomposed = ConvexDecomposition::decompose(test_input);
    std::vector<std::vector<PointR2>> vertices;
    std::vector<double> order;
    if (!decomposed.size() == 3) {
        std::cout << " decomposed.size != 3\n";
        return;
    }
    for (auto& pgon : decomposed) {
        vertices.push_back(pgon.vertices());
    }
    std::vector<bool> results ;
    
    ///////////////////////////////////////////////////*
    std::sort(decomposed.begin(), decomposed.end(), //// Lamda functions are pretty cool
        [](const Polygon& A, const Polygon& B) {   /////
            return A.centroid().x() < B.centroid().x();
        }
    );
    
    {  
    for (int i = 0; i < 3; ++i) {
    
    if (i==0) {
    bool polygons_are_identical = PolygonUtils::COMPARE_POLYGONS(decomposed[i],expectedL);
    results.push_back(polygons_are_identical);
    } else if (i==1) {
        bool polygons_are_identical = PolygonUtils::COMPARE_POLYGONS(decomposed[i],expectedC);
        results.push_back(polygons_are_identical);
    } else {
        bool polygons_are_identical = PolygonUtils::COMPARE_POLYGONS(decomposed[i],expectedR);
        results.push_back(polygons_are_identical);
    }; 
    
    }
    }
    std::cout << std::boolalpha;
    for (int i = 0; i < 3; ++i) {
        std::cout << "polygon_" << i << "_isMatch = " << results[i] << "\n";
    }
}


void test_MinkowskiSum() {
    Polygon pgon1  = PolygonUtils::createMinkowskiTest();
    Polygon pgon2  = PolygonUtils::createMinkowskiTest();
    Polygon expected = PolygonUtils::createMinkowskiResult();
    Polygon actual = MinkowskiSum::computeSum(pgon1,pgon2);
    bool polygons_are_identical = PolygonUtils::COMPARE_POLYGONS(actual,expected);

    ASSERT_THROW_SIMPLE(polygons_are_identical,true);

    if (polygons_are_identical) {
        std::cout << "test_MinkowskiSum" << " PASSED\n";
    }
    
    
}

