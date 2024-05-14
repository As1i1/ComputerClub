//
// Created by Dubinin Dmitrii on 5/12/24.
//
#pragma once

#include "event/event.h"

#include <vector>

namespace processing {
    std::pair<std::vector<std::pair<uint64_t, std::chrono::minutes>>, std::vector<event>>
    event_processing(std::vector<event>, club_info);
}