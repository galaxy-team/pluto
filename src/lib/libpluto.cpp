#include "libpluto.hpp"

#include <cstdint>

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <iostream>

#include <libasteroid.hpp>
#include <link_error.hpp>

std::vector<std::uint16_t> galaxy::pluto::link(std::vector<galaxy::asteroid> asteroids)
{
    // Step 1: Work out the offsets for each asteroid
    std::vector<int> offsets;
    offsets.push_back(0);
    for (int i = 1; i < asteroids.size(); ++i) {
        offsets.push_back(offsets[i-1] + asteroids[i-1].object_code.size());
    }

    // Step 2: Shift their used_labels
    for (int i = 0; i < asteroids.size(); ++i) {
        for (std::uint16_t addr : asteroids[i].used_labels) {
            asteroids[i].object_code[addr] += offsets[i];
        }
    }

    // Step 3: Create a global (w.r.t. the asteroids, not w.r.t. this program) exported_labels map.
    std::unordered_map<std::string, std::uint16_t> exported_labels;
    for (int i = 0; i < asteroids.size(); ++i) {
        for (std::pair<std::string, std::uint16_t> p : asteroids[i].exported_labels) {
            // Step 3a: Check that we haven't exported the same label from multiple asteroids.
            if (exported_labels.count(p.first)) {
                throw galaxy::pluto::link_error("exported label exported from multiple object files");
            }
            else {
                exported_labels[p.first] = p.second + offsets[i];
            }
        }
    }

    // Step 4: Check that all imported labels have been exported somewhere.
    // Step 5: Shift their imported_labels
    for (int i = 0; i < asteroids.size(); ++i) {
        for (std::pair<std::uint16_t, std::string> p : asteroids[i].imported_labels) {
            try {
                asteroids[i].object_code[p.first] = exported_labels.at(p.second);
            } catch (std::out_of_range& e) {
                throw galaxy::pluto::link_error("imported label not found.");
            }
        }
    }
    // Step 6: Concatenate together final binaries.
    std::vector<std::uint16_t> binary;
    for (int i = 0; i < asteroids.size(); ++i) {
        std::copy(asteroids[i].object_code.begin(), asteroids[i].object_code.end(), std::back_inserter(binary));
    }

    // Example:
    // Two asteroids:
    // asteroid0: (was ".EXPORT start \ start: SET PC, start")
        // object_code: 0x7f81 0x0000
        // imported_labels: {}
        // exported_labels: {"start": 0x0000}
        // used_labels: {0x0000}
    // asteroid1: (was ".IMPORT start \ func: SET A, 5 \ SET PC, start"
        // object_code: 0x7c01 0x0005 0x7f81 0x0000
        // imported_labels: {3: "start"}
        // exported_labels: {}
        // used_labels: {} (no labels local to the file used)
    // Step 1: Work out the offsets for each asteroid
        // By random chance, asteroid1 will be offset 0, asteroid0 will be offset 4 (the length of asteroid1)
    // Step 2: Shift their used_labels
        // asteroid0.object_code: 0x7f81 0x0002
        // asteroid1.object_code: 0x7c01 0x0005 0x7f81 0x0000
    // Step 3: Create a global exported_labels map.
        // {"start": 0x0004} (offset by 4)
    // Steps 3a & 4: All good
    // Step 5: Shift their imported_labels
        // asteroid0.object_code: 0x7f81 0x0002
        // asteroid1.object_code: 0x7c01 0x0005 0x7f81 0x0004
    // Step 6: Concatenate together final binaries
        // 0x7c01 0x0005 0x7f81 0x0004 0x7f81 0x0002

    return binary;
}
