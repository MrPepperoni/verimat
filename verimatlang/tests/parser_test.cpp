#include <gtest/gtest.h>
#include <verimatlang/parser.hpp>

// include the tested cpp, so we can test symbols from anonymous namespaces
#include <parser.cpp>

class TokenizeTest: public ::testing::TestWithParam<std::tuple<char, verimatlang::token> > {};

TEST_P(TokenizeTest, Tokenization) {
    auto const& [input, expected] = GetParam();
    auto const& tok = tokenize(input);

    EXPECT_EQ(expected, tok);
}

INSTANTIATE_TEST_SUITE_P(Tokens, TokenizeTest,
        ::testing::Values(
            std::make_tuple('+', verimatlang::token::negate_bit),
            std::make_tuple(',', verimatlang::token::read),
            std::make_tuple(';', verimatlang::token::write),
            std::make_tuple('<', verimatlang::token::prev_bit),
            std::make_tuple('>', verimatlang::token::next_bit),
            std::make_tuple('[', verimatlang::token::jump_if_zero),
            std::make_tuple(']', verimatlang::token::jump_back_if_one),
            std::make_tuple('a', verimatlang::token::noop)
        )
);

class ParserTest: public ::testing::TestWithParam<std::tuple<std::string, verimatlang::program_t> > {};

TEST_P(ParserTest, TokenParsing) {
    auto const& [input, expected] = GetParam();

    auto const& parsed = verimatlang::parse(input);

    EXPECT_EQ(expected, parsed);
}

INSTANTIATE_TEST_SUITE_P(Tokens, ParserTest,
        ::testing::Values(
            std::make_tuple(std::string{"+"}, verimatlang::program_t{{verimatlang::token::negate_bit, 0}}),
            std::make_tuple(std::string{","}, verimatlang::program_t{{verimatlang::token::read, 0}}),
            std::make_tuple(std::string{";"}, verimatlang::program_t{{verimatlang::token::write, 0}}),
            std::make_tuple(std::string{"<"}, verimatlang::program_t{{verimatlang::token::prev_bit, 0}}),
            std::make_tuple(std::string{">"}, verimatlang::program_t{{verimatlang::token::next_bit, 0}}),
            std::make_tuple(std::string{"[]"}, verimatlang::program_t{{verimatlang::token::jump_if_zero, 1}, {verimatlang::token::jump_back_if_one, 0}}),
            std::make_tuple(std::string{"a"}, verimatlang::program_t{{verimatlang::token::noop, 0}})
        )
);

class ParserTestExc: public ::testing::TestWithParam<std::string> {};

TEST_P(ParserTestExc, SyntaxError) {
    EXPECT_THROW({
            verimatlang::parse(GetParam());
    }, verimatlang::syntax_error);
}

INSTANTIATE_TEST_SUITE_P(SyntaxError, ParserTestExc,
        ::testing::Values(
            std::string{"["},
            std::string{"]"},
            std::string{"[[]][][[][]"}
        )
);
