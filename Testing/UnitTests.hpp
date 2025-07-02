#pragma once
#include <iostream>
#include <algorithm>
/*
Simple unit test assertion and arbitrary container check routine
*/



#define ASSERT_THROW_SIMPLE(expected, actual) \
    if (expected != actual) \
    std::cerr <<__FUNCTION__<< " failed: " << #expected \
    << " != " << #actual << "\n" ;

template<typename Container>
void ASSERT_THROW_GENERAL(const Container& expected, const Container& actual, const char* NAME) {
    if (!std::equal(expected.begin(),expected.end(),actual.begin(),actual.end())) {
        std::cerr << NAME << " failed: results not equal\n";
    }
}
