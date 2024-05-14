//
// Created by Dubinin Dmitrii on 5/12/24.
//

#include "../src/event_processing.h"
#include "../src/parser/parser.h"

#include "gtest/gtest.h"

#include <fstream>
#include <algorithm>

namespace {
    bool check_events(const event &e1, const event &e2) {
        return e1.time == e2.time && e1.type == e2.type && e1.client_name == e2.client_name &&
               e1.place_no == e2.place_no &&
               e1.msg == e2.msg;
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::istringstream iss(s);
        std::vector<std::string> result;
        std::string next;
        while (std::getline(iss, next, delim)) {
            result.emplace_back(next);
        }
        return result;
    }

    std::chrono::minutes parse_time(const std::string &s) {
        std::vector<std::string> splitted = split(s, ':');
        return std::chrono::minutes{std::stoi(splitted[0]) * 60 + std::stoi(splitted[1])};
    }

    std::pair<std::vector<std::pair<uint64_t, std::chrono::minutes>>, std::vector<event>>
    parse(const std::string &path) {
        std::ifstream is(path);
        if (!is.is_open()) {
            is = std::ifstream("tests/" + path);
        }
        std::vector<event> result_events;
        std::string cur_line;
        std::getline(is, cur_line);
        for (; !cur_line.empty(); std::getline(is, cur_line)) {
            std::vector<std::string> splitted = split(cur_line, ' ');
            event e;
            e.time = parse_time(splitted[0]);
            e.type = static_cast<event_type>(std::stoi(splitted[1]));
            if (e.type == event_type::OUT_ERROR) {
                e.msg = splitted[2];
            } else {
                e.client_name = splitted[2];
                if (e.type == event_type::CLIENT_LOCK_TABLE || e.type == event_type::OUT_CLIENT_LOCK_TABLE) {
                    e.place_no = std::stoi(splitted[3]);
                }
            }
            result_events.emplace_back(e);
        }

        std::vector<std::pair<std::uint64_t, std::chrono::minutes>> result_amount;
        for (; !is.eof();) {
            std::getline(is, cur_line);
            std::vector<std::string> splitted = split(cur_line, ' ');
            result_amount.emplace_back(std::stoi(splitted[1]), parse_time(splitted[2]));
        }
        return {result_amount, result_events};
    }

    void test_correct(const std::string &path, const std::string &path_to_ans) {
        std::ifstream input(path);
        if (!input.is_open()) {
            input = std::ifstream("tests/" + path);
        }
        auto [info, events_1] = parsing::parse(input);
        auto [amount, res_events] = processing::event_processing(events_1, info);
        auto [answer_amount, answer_events] = parse(path_to_ans);
        ASSERT_EQ(res_events.size(), answer_events.size());
        for (std::size_t i = 0; i < res_events.size(); ++i) {
            std::cout << i << "\n";
            ASSERT_TRUE(check_events(res_events[i], answer_events[i]));
        }
        ASSERT_EQ(amount, answer_amount);
    }
}

TEST(correctness, sample) {
    test_correct("test_cases/case_1", "test_cases/case_1_answer");
}

TEST(correctness, Not_Open_Yet) {
    test_correct("test_cases/case_2", "test_cases/case_2_answer");
}

TEST(correctness, Client_Unknown) {
    test_correct("test_cases/case_3", "test_cases/case_3_answer");
}

TEST(correctness, You_Shall_Not_Pass) {
    test_correct("test_cases/case_4", "test_cases/case_4_answer");
}

TEST(correctness, Client_Leave) {
    test_correct("test_cases/case_5", "test_cases/case_5_answer");
}

TEST(correctness, Client_Never_Leave) {
    test_correct("test_cases/case_6", "test_cases/case_6_answer");
}