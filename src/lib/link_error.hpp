#ifndef LINK_ERROR_HPP
#define LINK_ERROR_HPP

namespace galaxy {
    namespace pluto {
        class link_error : public std::runtime_error {
        public:
            link_error(const std::string& what_str) : runtime_error(what_str) {}
        };
}
}

#endif /* LINK_ERROR_HPP */

