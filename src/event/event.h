//
// Created by Dubinin Dmitrii on 5/12/24.
//
#pragma once
#include <chrono>

enum class event_type {
    // in events
    CLIENT_ARRIVE = 1,
    CLIENT_LOCK_TABLE = 2,
    CLIENT_WAITING = 3,
    CLIENT_LEFT = 4,

    // out events
    OUT_CLIENT_LEFT = 11,
    OUT_CLIENT_LOCK_TABLE = 12,
    OUT_ERROR = 13
};

constexpr std::size_t place_npos = -1;

struct event{
    std::chrono::minutes time;
    event_type type;
    std::string client_name;
    std::size_t place_no = place_npos;
    std::string msg;
};

struct club_info {
    std::size_t place_count;
    std::chrono::minutes start_time;
    std::chrono::minutes end_time;
    std::uint64_t cost;
};