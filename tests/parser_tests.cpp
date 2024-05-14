//
// Created by Dubinin Dmitrii on 5/12/24.
//
#include <gtest/gtest.h>
#include <fstream>
#include "../src/parser/parser.h"

TEST(header, correct) {
    std::stringstream s("123\n10:00 12:00\n100");
    auto [h, v] = parsing::parse(s);
    ASSERT_EQ(h.place_count, 123);
    ASSERT_EQ(h.cost, 100);
    ASSERT_EQ(h.start_time.count(), 10 * 60);
    ASSERT_EQ(h.end_time.count(), 12 * 60);
    ASSERT_TRUE(v.empty());
}

static void check_error(char line_no, std::size_t line_no_i, const std::string& s) {
    std::cout << s;
    std::stringstream iss(s);
    ASSERT_THROW(parsing::parse(iss), parsing::parse_exception);
    try {
        iss = std::stringstream("-1\n10:00 12:00\n100");
        parsing::parse(iss);
    } catch (parsing::parse_exception &e) {
        std::string msg = e.what();
        ASSERT_TRUE(msg.find(line_no) != line_no_i);
    }
}

TEST(header, wrong_place_count) {
    check_error('1', 1, "-1\n10:00 12:00\n100");
    check_error('1', 1, "sadbad\n10:00 12:00\n100");
}

TEST(header, wrong_time_start) {
    check_error('2', 2, "1\n-2:00 12:00\n100");
    check_error('2', 2, "1\nasfdasfd 12:00\n100");
    check_error('2', 2, "1\nasfda:sfd 12:00\n100");
    check_error('2', 2, "1\n12:00 -1:00\n100");
    check_error('2', 2, "1\n12:00 12:-1\n100");
    check_error('2', 2, "1\n12:-7 12:00\n100");
    check_error('2', 2, "1\n12:00 ewqqwe\n100");
    check_error('2', 2, "1\n12:00 ewq:qwe\n100");
    check_error('2', 2, "1\n12:0 13:00\n100");
    check_error('2', 2, "1\n9:00 19:00\n100");
    check_error('2', 2, "1\n08:00 9:00\n100");
    check_error('2', 2, "1\n09:00 19:0\n100");
    check_error('2', 2, "1\n19:00 10:00\n100");
}

TEST(header, wrong_cost) {
    check_error('3', 3, "1\n10:00 19:00\n-100");
    check_error('3', 3, "1\n10:00 19:00\nfsadfdasd");
}

TEST(events, correct) {
    std::string s = "123\n10:00 12:00\n100";
    for (std::size_t i = 0; i < 4; ++i) {
        s += std::format("\n11:{} 1 client{}", (i + 1) * 10, i);
    }
    std::stringstream iss(s);
    auto [h, v] = parsing::parse(iss);
    ASSERT_EQ(h.place_count, 123);
    ASSERT_EQ(h.cost, 100);
    ASSERT_EQ(h.start_time.count(), 10 * 60);
    ASSERT_EQ(h.end_time.count(), 12 * 60);
    ASSERT_EQ(v.size(), 4);
}

static std::string get_string(const std::string& last) {
    std::string s = "123\n10:00 12:00\n100";
    for (std::size_t i = 0; i < 4; ++i) {
        s += std::format("\n11:{} 1 client{}", (i + 1) * 10, i);
    }
    s += last;
    return s;
}

TEST(events, wrong_time) {
    check_error('8', 8, get_string("\n-1:00 1 client_wrong"));
}

TEST(events, wrong_client_name) {
    check_error('8', 8, get_string("\n11:59 1 client_WRONG"));
}

TEST(events, wrong_event_order) {
    check_error('8', 8, get_string("\n10:01 1 client_wrong"));
}

TEST(events, unknow_event) {
    check_error('8', 8, get_string("\n11:59 10 client_wrong"));
    check_error('8', 8, get_string("\n11:59 -1 client_wrong"));
    check_error('8', 8, get_string("\n11:59 11 client_wrong"));
}


TEST(events, correct_file) {
    std::ifstream input("tests/test_cases/case_1");
    if (!input.is_open()) {
        input = std::ifstream("test_cases/case_1");
    }
    auto [h, v] = parsing::parse(input);
    ASSERT_EQ(h.place_count, 3);
    ASSERT_EQ(h.cost, 10);
    ASSERT_EQ(h.start_time.count(), 9 * 60);
    ASSERT_EQ(h.end_time.count(), 19 * 60);
    ASSERT_EQ(v.size(), 14);
}