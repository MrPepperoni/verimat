#include <verimatlang/parser.hpp>
#include <verimatlang/io.hpp>
#include <verimatlang/interpreter.hpp>

#include <argparse/argparse.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <iterator>
#include <climits>

namespace {

class stdin_input_bitstream: public verimatlang::input_bitstream {
public:
    virtual bool read() {
        if (bit_pos_ >= CHAR_BIT) {
            bit_pos_ = 0;
            curr_char_ = std::cin.get();
            if (curr_char_ == std::istream::traits_type::eof()) {
                curr_char_ = 0;
            }
        }
        bool const bit{(curr_char_ & (1 << bit_pos_)) >> bit_pos_ == 1};
        ++bit_pos_;
        return bit;
    }
private:
    std::istream::int_type curr_char_{0};
    std::size_t bit_pos_{CHAR_BIT};
};

class stdout_output_bitstream: public verimatlang::output_bitstream {
public:
    virtual void write(bool bit) {
        if (bit) {
            curr_char_ |= (1 << bit_pos_);
        } else {
            curr_char_ &= ~(1 << bit_pos_);
        }
        ++bit_pos_;
        if (bit_pos_ >= CHAR_BIT) {
            flush();
        }
    }
    virtual void flush() {
        if (bit_pos_ == 0) {
            return;
        }
        std::cout.put(curr_char_);
        bit_pos_ = 0;
        curr_char_ = 0;
    }
private:
    std::ostream::char_type curr_char_{0};
    std::size_t bit_pos_{0};
};

} // anonymous namespace

int main(int argc, char* argv[]) {
    argparse::ArgumentParser parser{"verimat"};
    parser.add_argument("program")
        .help("program file to execute");

    try {
        parser.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    std::ifstream program_stream{parser.get<std::string>("program")};
    std::string program{std::istreambuf_iterator<char>{program_stream}, {}};

    try {
        auto const& parsed_program{verimatlang::parse(program)};
        verimatlang::interpret(parsed_program, std::move(stdin_input_bitstream()), std::move(stdout_output_bitstream()));
    }
    catch (const verimatlang::syntax_error& err) {
        std::cerr << err.what() << std::endl;
        std::size_t line_number = 0;
        std::size_t char_number = 0;
        std::size_t num_tabs = 0;
        for (std::size_t i = 0; i != err.position(); ++i) {
            if (program[i] == '\n') {
                ++line_number;
                char_number = 0;
                num_tabs = 0;
            } else {
                if (program[i] == '\t') {
                    ++num_tabs;
                }
                ++char_number;
            }
        }
        std::cerr << "syntax error at line " << line_number + 1 << " char " << char_number + 1 << std::endl;
        if (char_number > 80) {
            char_number = 76;
            std::cerr << "... ";
        }
        std::string_view view{program.c_str() + err.position() - char_number, char_number + 1};
        std::cerr << view << std::endl;
        for (std::size_t i = 0; i != num_tabs; ++i) {
            std::cerr << '\t';
        }
        for (std::size_t i = 0; i != char_number - num_tabs; ++i) {
            std::cerr << ' ';
        }
        std::cerr << '^' << std::endl;
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program << std::endl;
    }

    return 0;
}
