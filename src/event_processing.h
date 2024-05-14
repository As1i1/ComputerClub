//
// Created by Dubinin Dmitrii on 5/12/24.
//
#pragma once

#include "event/event.h"

#include <cstddef>
#include <vector>
#include <chrono>
#include <utility>

namespace processing {
    std::pair<std::vector<std::pair<std::uint64_t, std::chrono::minutes>>, std::vector<event>>
    event_processing(std::vector<event>, club_info);
}