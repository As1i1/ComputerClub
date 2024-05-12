//
// Created by Dubinin Dmitrii on 5/12/24.
//

#include "parser.h"

#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <list>
#include <algorithm>

namespace {
    class parser {
    public:
        explicit parser(std::istream &input) : input(input) {}

        std::pair<club_info, std::list<event>> parse() {
            club_info info = parse_club_info();
            std::list<event> events = parse_events(info.place_count);
            return {info, events};
        }

    private:
        club_info parse_club_info() {
            club_info info{};
            try {
                info.place_count = std::stoi(get_next_line());
            } catch (const std::exception &e) {
                throw parsing::parse_exception("Cannot parse number in line 1");
            }
            std::vector<std::string> working_hours = split(get_next_line(), ' ');
            if (working_hours.size() != 2) {
                throw parsing::parse_exception("Expected working hours in line 2");
            }
            info.start_time = parse_time(working_hours[0]);
            info.end_time = parse_time(working_hours[1]);
            try {
                info.cost = std::stoi(get_next_line());
            } catch (const std::exception &e) {
                throw parsing::parse_exception("Cannot parse number in line 3");
            }
            line_no = 4;
            return info;
        }

        std::list<event> parse_events(std::size_t place_count) {
            std::list<event> events;
            std::chrono::minutes previous_time{0};
            for (;!input.eof();++line_no) {
                events.emplace_back(get_next_event(previous_time, place_count));
                previous_time = events.back().time;
            }
            return events;
        }

        event get_next_event(std::chrono::minutes previous_time, std::size_t place_count) {
            event new_event;

            std::string cur_line = get_next_line();
            std::vector<std::string> splitted = split(cur_line, ' ');
            if (splitted.size() < 3 || splitted.size() > 4) {
                throw parsing::parse_exception("Unsupported event in line " + std::to_string(line_no));
            }

            new_event.time = parse_time(splitted[0]);
            if (new_event.time < previous_time) {
                throw parsing::parse_exception("Wrong event order in line " + std::to_string(line_no));
            }

            int type_int_value;
            try {
                type_int_value = std::stoi(splitted[1]);
            } catch (std::exception &e) {
                throw parsing::parse_exception("Unknown event id in line " + std::to_string(line_no));
            }
            new_event.type = get_event_type(type_int_value);
            if (new_event.type == event_type::CLIENT_LOCK_TABLE && splitted.size() != 4) {
                throw parsing::parse_exception(
                        "Expected 4 tokens for event with id: 2, buf found 3, in line " + std::to_string(line_no));
            } else if (new_event.type != event_type::CLIENT_LOCK_TABLE && splitted.size() == 4) {
                throw parsing::parse_exception("Expected 3 tokens, but found 4, in line " + std::to_string(line_no));
            }

            if (!check_client_name(splitted[2])) {
                throw parsing::parse_exception("Unsupported client name in line " + std::to_string(line_no));
            }
            new_event.client_name = splitted[2];

            if (new_event.type == event_type::CLIENT_LOCK_TABLE) {
                try {
                    new_event.place_no = std::stoul(splitted[3]);
                } catch (const std::exception &e) {
                    throw parsing::parse_exception("Unsupported place no, in line " + std::to_string(line_no));
                }
                if (new_event.place_no > place_count) {
                    throw parsing::parse_exception("Unexpected place_no, in line " + std::to_string(line_no));
                }
            }

            return new_event;
        }

        std::string get_next_line() {
            std::string line;
            std::getline(input, line);
            return line;
        }

        std::chrono::minutes parse_time(const std::string &s) {
            std::vector<std::string> splitted = split(s, ':');
            if (splitted.size() != 2) {
                throw parsing::parse_exception("Unsupported time format in line " + std::to_string(line_no));
            }
            std::size_t h;
            std::size_t m;
            try {
                h = std::stoull(splitted[0]);
                m = std::stoull(splitted[1]);
            } catch (std::exception &e) {
                throw parsing::parse_exception("Unsupported time format in line " + std::to_string(line_no));
            }
            if (h >= 24 || m >= 60) {
                throw parsing::parse_exception("Unsupported time format in line " + std::to_string(line_no));
            }
            return std::chrono::minutes{h * 60 + m};
        }

        event_type get_event_type(int type) const {
            switch (type) {
                case static_cast<int>(event_type::CLIENT_ARRIVE):
                    return event_type::CLIENT_ARRIVE;
                case static_cast<int>(event_type::CLIENT_LOCK_TABLE):
                    return event_type::CLIENT_LOCK_TABLE;
                case static_cast<int>(event_type::CLIENT_LEFT):
                    return event_type::CLIENT_LEFT;
                case static_cast<int>(event_type::CLIENT_WAITING):
                    return event_type::CLIENT_WAITING;
                default:
                    throw parsing::parse_exception("Unknown event type in line " + std::to_string(line_no));
            }
        }

        static bool check_client_name(const std::string &client_name) {
            auto pred = [](char c) {
                return c == '_' || c == '-' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z');
            };
            return std::ranges::all_of(client_name, pred);
        }

        static std::vector<std::string> split(const std::string &s, char delim) {
            std::istringstream iss(s);
            std::vector<std::string> result;
            std::string next;
            while (std::getline(iss, next, delim)) {
                result.emplace_back(next);
            }
            return result;
        }

        std::istream &input;
        std::size_t line_no = 1;
    };
}

std::pair<club_info, std::list<event>> parsing::parse(std::istream &input) {
    return parser(input).parse();
}