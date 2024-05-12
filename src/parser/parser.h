//
// Created by Dubinin Dmitrii on 5/12/24.
//
#pragma once

#include "../event/event.h"
#include <exception>
#include <utility>
#include <list>

namespace parsing {
    std::pair<club_info, std::list<event>> parse(std::istream &input);

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