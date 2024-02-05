#include "verimatlang/interpreter.hpp"

namespace {

class tape {
public:
    void prev_bit() {
        --pos_;
        if (pos_ < 0) {
            negbits_.resize(std::max<std::size_t>(negbits_.size(), -pos_));
        }
    }
    void next_bit() {
        ++pos_;
        if (pos_ > 0) {
            nonnegbits_.resize(std::max<std::size_t>(nonnegbits_.size(), pos_ + 1));
        }
    }
    bool read_bit() const {
        if (pos_ >= 0) {
            return nonnegbits_[pos_];
        }
        return negbits_[-pos_ - 1];
    }
    void write_bit(bool bit) {
        if (pos_ >= 0) {
            nonnegbits_[pos_] = bit;
        } else {
            negbits_[-pos_ - 1] = bit;
        }
    }

    std::int64_t pos() const { return pos_; }
    std::vector<bool> nonnegbits() const { return nonnegbits_; }
    std::vector<bool> negbits() const { return negbits_; }
private:
    std::int64_t pos_{0};
    std::vector<bool> nonnegbits_{false};
    std::vector<bool> negbits_;
};

} // anonymous namespace

namespace verimatlang {

void interpret(program_t const& program, input_bitstream&& in, output_bitstream&& out) {
    tape mem;
    auto ip = program.begin();
    while(ip != program.end()) {
        auto const& [token, pos] = *(ip);
        switch (token) {
            case token::negate_bit:
                mem.write_bit(!mem.read_bit());
                break;
            case token::read:
                mem.write_bit(in.read());
                break;
            case token::write:
                out.write(mem.read_bit());
                break;
            case token::prev_bit:
                mem.prev_bit();
                break;
            case token::next_bit:
                mem.next_bit();
                break;
            case token::jump_if_zero:
                if (!mem.read_bit()) {
                    ip = program.begin() + pos;
                    continue;
                }
                break;
            case token::jump_back_if_one:
                if (mem.read_bit()) {
                    ip = program.begin() + pos;
                    continue;
                }
                break;
            default:
                break;
        }
        ++ip;
    }
    out.flush();
}

}
