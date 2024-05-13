//
// Created by Dubinin Dmitrii on 5/12/24.
//
#pragma once
#include "event/event.h"

#include <list>

namespace processing {
    std::pair<std::vector<std::pair<uint64_t, std::chrono::minutes>>, std::list<event>> event_processing(std::list<event>, club_info);
}