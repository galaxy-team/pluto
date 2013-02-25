#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <cstdint>

#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <libpluto.hpp>
#include <libasteroid.hpp>

template<typename T>
void add_to_vector(std::vector<T>& v) {
}

template<typename A, typename ...T>
void add_to_vector(std::vector<A>& v, A t, T ...args)
{
    v.push_back(t);
    add_to_vector(v, ((uint16_t)args)...);
}

TEST_CASE("libpluto/link", "Test linking together two simple pre-assembled object files.") {
    galaxy::asteroid a1;
    galaxy::asteroid a2;

    a1.object_code.push_back(0x7f81);
    a1.object_code.push_back(0x0000);
    a1.exported_labels["start"] = 0x0000;
    a1.used_labels.insert(0x0001);
    
    a2.object_code.push_back(0x7c01);
    a2.object_code.push_back(0x0005);
    a2.object_code.push_back(0x7f81);
    a2.object_code.push_back(0x0000);
    a2.imported_labels[0x0003] = "start";

    std::vector<galaxy::asteroid> v{a2, a1};
    std::vector<std::uint16_t> bin = galaxy::pluto::link(v);
    std::vector<std::uint16_t> correct_bin;
    add_to_vector(correct_bin, (uint16_t)0x7c01, 0x0005, 0x7f81, 0x0004, 0x7f81, 0x0004);
    REQUIRE(std::equal(bin.begin(), bin.end(), correct_bin.begin()));
}

