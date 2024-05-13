//
// Created by Dubinin Dmitrii on 5/11/24.
//
#include "parser/parser.h"
#include "event/event.h"
#include "event_processing.h"
#include <iostream>
#include <fstream>

static std::string format_number(long i) {
    std::stringstream s;
    s << std::setfill('0') << std::setw(2) << i;
    return s.str();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "expected 2 args";
        return 1;
    }
    std::ifstream file{argv[1]};
    if (!file.is_open()) {
        std::cerr << "Cannot open file " << argv[1];
        return 1;
    }

    club_info info{};
    std::list<event> events;
    try {
        std::tie(info, events) = parsing::parse(file);
    } catch (const parsing::parse_exception &e) {
        std::cerr << e.what();
        return 1;
    }

    std::vector<std::pair<std::uint64_t, std::chrono::minutes>> places_amount;
    std::tie(places_amount, events) = processing::event_processing(events, info);
    std::cout << std::format("{}:{}", format_number(info.start_time.count() / 60),
                             format_number(info.start_time.count() % 60)) << std::endl;
    for (auto &&e: events) {
        std::string hours = format_number(e.time.count() / 60);
        std::string minutes = format_number(e.time.count() % 60);
        switch (e.type) {
            case event_type::OUT_CLIENT_LOCK_TABLE:
            case event_type::CLIENT_LOCK_TABLE:
                std::cout << std::format("{}:{} {} {} {}", hours, minutes,
                                         static_cast<int>(e.type), e.client_name, e.place_no) << std::endl;
                break;
            case event_type::CLIENT_ARRIVE:
            case event_type::CLIENT_WAITING:
            case event_type::CLIENT_LEFT:
            case event_type::OUT_CLIENT_LEFT:
                std::cout << std::format("{}:{} {} {}", hours, minutes,
                                         static_cast<int>(e.type), e.client_name) << std::endl;
                break;
            case event_type::OUT_ERROR:
                std::cout << std::format("{}:{} {} {}", hours, minutes,
                                         static_cast<int>(e.type), e.msg) << std::endl;
                break;
        }
    }
    std::cout << std::format("{}:{}\n", format_number(info.end_time.count() / 60),
                             format_number(info.end_time.count() % 60));

    for (std::size_t i = 0; i < info.place_count; ++i) {
        std::string hours = format_number(places_amount[i].second.count() / 60);
        std::string minutes = format_number(places_amount[i].second.count() % 60);
        std::cout << i + 1 << " " << places_amount[i].first << " " << std::format("{}:{}", hours, minutes) << std::endl;
    }
}
