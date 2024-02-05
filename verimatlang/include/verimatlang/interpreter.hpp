#pragma once

#include "io.hpp"
#include "parser.hpp"

namespace verimatlang {

void interpret(program_t const& program, input_bitstream&& in, output_bitstream&& out);

}
