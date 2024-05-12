//
// Created by Dubinin Dmitrii on 5/12/24.
//
#include <gtest/gtest.h>
#include <fstream>
#include "../src/parser/parser.h"

TEST(parse_test, simple) {
    std::stringstream s("123\n10:00 12:00\n100");
    auto [h, v] = parsing::parse(s);
    ASSERT_EQ(h.place_count, 123);
    ASSERT_EQ(h.cost, 100);
    ASSERT_EQ(h.start_time.count(), 10 * 60);
    ASSERT_EQ(h.end_time.count(), 12 * 60);
    ASSERT_TRUE(v.empty());
}

TEST(parse_test, example) {
    std::ifstream input("test_cases/case_1"); // TODO set working directory to folder tests
    auto [h, v] = parsing::parse(input);
    ASSERT_EQ(h.place_count, 3);
    ASSERT_EQ(h.cost, 10);
    ASSERT_EQ(h.start_time.count(), 9 * 60);
    ASSERT_EQ(h.end_time.count(), 19 * 60);
    ASSERT_EQ(v.size(), 14);
}