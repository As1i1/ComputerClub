//
// Created by Dubinin Dmitrii on 5/12/24.
//
#pragma once

#include "../event/event.h"
#include <exception>
#include <utility>
#include <vector>
#include <string>
#include <istream>

namespace parsing {
    std::pair<club_info, std::vector<event>> parse(std::istream&);

    class parse_exception : public std::exception {
    public:
        explicit parse_exception(std::string msg) : msg(std::move(msg)) {}

        const char *what() const noexcept override {
            return msg.data();
        }

    private:
        const std::string msg;
    };
}