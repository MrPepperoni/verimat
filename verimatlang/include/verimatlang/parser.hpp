#pragma once

#include <cassert>
#include <cstdint>
#include <stack>
#include <string>
#include <vector>
#include <stdexcept>
#include <tuple>

namespace verimatlang {

class syntax_error: public std::exception {
public:
    syntax_error(std::string const& what, std::size_t position)
        : msg_{what} , pos_{position} {}
    syntax_error(char const* what, std::size_t position)
        : msg_{what} , pos_{position} {}
    virtual ~syntax_error() noexcept {}
    virtual char const* what() const noexcept { return msg_.c_str(); }
    virtual std::size_t position() const noexcept { return pos_; }
protected:
    std::string msg_;
    std::size_t pos_;
};

enum class token {
    negate_bit,
    read,
    write,
    prev_bit,
    next_bit,
    jump_if_zero,
    jump_back_if_one,
    noop,
};

using program_t = std::vector<std::tuple<token, std::size_t> >;

program_t parse(std::string const& program);

}
