#include <gtest/gtest.h>
#include <verimatlang/interpreter.hpp>

// include the tested cpp, so we can test symbols from anonymous namespaces
#include <interpreter.cpp>
#include <io.cpp>

#include <memory>
#include <iostream>

using testparam_t = std::tuple<std::size_t, std::int64_t, std::vector<bool>, std::vector<bool> >;
class TapePrevBitTest: public ::testing::TestWithParam< testparam_t > {};

TEST_P(TapePrevBitTest, PrevBit) {
    // test tape state after moving the pointer to the left
    auto const& [count, exp_pos, exp_negbits, exp_nonnegbits] = GetParam();

    tape mem;
    for (std::size_t i = 0; i != count; ++i) {
        mem.prev_bit();
    }
    auto const& pos = mem.pos();
    auto const& neg = mem.negbits();
    auto const& nonneg = mem.nonnegbits();
    EXPECT_EQ(pos, exp_pos);
    EXPECT_EQ(neg, exp_negbits);
    EXPECT_EQ(nonneg, exp_nonnegbits);
}

INSTANTIATE_TEST_SUITE_P(PrevBit, TapePrevBitTest,
        ::testing::Values(
            testparam_t{1, -1, {false}, {false}},
            testparam_t{4, -4, {false, false, false, false}, {false}}
        )
);

class TapeNextBitTest: public ::testing::TestWithParam< testparam_t > {};

TEST_P(TapeNextBitTest, NextBit) {
    // test tape state after moving the pointer to the right
    auto const& [count, exp_pos, exp_negbits, exp_nonnegbits] = GetParam();

    tape mem;
    for (std::size_t i = 0; i != count; ++i) {
        mem.next_bit();
    }
    auto const& pos = mem.pos();
    auto const& neg = mem.negbits();
    auto const& nonneg = mem.nonnegbits();
    EXPECT_EQ(pos, exp_pos);
    EXPECT_EQ(neg, exp_negbits);
    EXPECT_EQ(nonneg, exp_nonnegbits);
}

INSTANTIATE_TEST_SUITE_P(NextBit, TapeNextBitTest,
        ::testing::Values(
            testparam_t{1, 1, {}, {false, false}},
            testparam_t{4, 4, {}, {false, false, false, false, false}}
        )
);

TEST(ReadBitTest, ReadBit) {
    // test reading in default ptr position
    tape mem;
    EXPECT_EQ(mem.read_bit(), false);
}

TEST(WriteBitTest, WriteBit) {
    // test writing and rewriting
    tape mem;
    mem.write_bit(true);
    EXPECT_EQ(mem.read_bit(), true);
    mem.write_bit(false);
    EXPECT_EQ(mem.read_bit(), false);
}

TEST(TapeComplexTest, Complex) {
    // test complex operations on a tape instance
    tape mem;
    mem.prev_bit();
    mem.write_bit(true);
    mem.prev_bit();
    mem.next_bit();
    mem.next_bit();
    mem.next_bit();
    mem.next_bit();
    mem.next_bit();
    mem.write_bit(true);
    mem.next_bit();
    EXPECT_EQ(mem.pos(), 4);
    std::vector<bool> exp_negbits{true, false};
    EXPECT_EQ(mem.negbits(), exp_negbits);
    std::vector<bool> exp_nonnegbits{false, false, false, true, false};
    EXPECT_EQ(mem.nonnegbits(), exp_nonnegbits);
}

TEST(HelloWorldTest, Interpret) {
    struct input_bitstream: public verimatlang::input_bitstream {
        bool read() { return false; }
    };
    struct output_bitstream: public verimatlang::output_bitstream {
        void write(bool bit) { buffer->push_back(bit ? '1' : '0'); }
        void flush() {}
        std::shared_ptr<std::string> buffer{std::make_shared<std::string>()};
    };
    std::string hello_world_program = "\
;;;+;+;;+;+;    H\n\
+;+;+;+;;+;;+;  e\n\
;;+;;+;+;;+;    l\n\
;;+;;+;+;;+;    l\n\
+;;;;+;+;;+;    o\n\
;;+;;+;+;+;;    ,\n\
;;;;;+;+;;      \n\
+;;;+;+;;;+;    w\n\
+;;;;+;+;;+;    o\n\
;+;+;;+;;;+;    r\n\
;;+;;+;+;;+;    l\n\
;;+;+;;+;;+;    d\n\
+;+;;;;+;+;;    !\n\
;+;+;+;\n\
";

    output_bitstream obs;
    auto buf_ptr = obs.buffer;
    verimatlang::interpret(verimatlang::parse(hello_world_program), std::move(input_bitstream()), std::move(obs));
    EXPECT_EQ(*buf_ptr, "000100101010011000110110001101101111011000110100000001001110111011110110010011100011011000100110100001000101");
}
