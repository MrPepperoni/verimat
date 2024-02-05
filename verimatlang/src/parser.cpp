#include "verimatlang/parser.hpp"

namespace {

constexpr verimatlang::token tokenize(char const c) {
    using namespace verimatlang;
    switch (c) {
        case '+': return token::negate_bit;
        case ',': return token::read;
        case ';': return token::write;
        case '<': return token::prev_bit;
        case '>': return token::next_bit;
        case '[': return token::jump_if_zero;
        case ']': return token::jump_back_if_one;
        default: return token::noop;
    }
}

}

namespace verimatlang {

program_t parse(std::string const& program) {
    program_t tokenized_program;
    std::stack<std::size_t> fwd_jumps;
    for (auto const& c: program) {
        auto const tok{tokenize(c)};
        // could skip noop to save a bit of memory
        // but then we'd have to track the current position for error feedback
        std::size_t loc{0};
        if (tok == token::jump_if_zero) {
            fwd_jumps.push(tokenized_program.size());
        }
        else if (tok == token::jump_back_if_one) {
            if (fwd_jumps.empty()) {
                throw syntax_error(
                    "`]` without matching `[` token at "
                        + std::to_string(tokenized_program.size()),
                    tokenized_program.size()
                );
            }
            loc = fwd_jumps.top();
            std::get<1>(tokenized_program[loc]) = tokenized_program.size();
            fwd_jumps.pop();
        }
        tokenized_program.push_back(std::make_tuple(tok, loc));
    }
    if (!fwd_jumps.empty()) {
        throw syntax_error(
            "unterminated `[` token at "
                + std::to_string(fwd_jumps.top()),
            fwd_jumps.top()
        );
    }
    return tokenized_program;
}

}
