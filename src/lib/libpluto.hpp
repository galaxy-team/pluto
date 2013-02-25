#include <cstdint>

#include <stdexcept>
#include <string>

#include <libasteroid.hpp>

namespace galaxy {
    namespace pluto {
        /**
         * vector<u16> link(asteroid)
         * 
         * links the object code in the provided asteroids,
         * producing a vector of unsigned 16-bit integers
         * representing a DCPU-16 binary.
         */
        std::vector<std::uint16_t> link(std::vector<galaxy::asteroid>);

        class link_error : public std::runtime_error {
        public:
            link_error(const std::string& what_str) : runtime_error(what_str) {}
        };
    }
}
