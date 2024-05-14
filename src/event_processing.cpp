//
// Created by Dubinin Dmitrii on 5/12/24.
//

#include "event_processing.h"
#include <queue>
#include <map>
#include <set>
#include <iostream>
#include <utility>

enum class client_state {
    IN = -1,
    WAITING = -2,
};

class computer_club {
public:
    explicit computer_club(std::vector<event> events, club_info info) :
            events(std::move(events)), info(info),
            places(info.place_count, ntime),
            place_amount(info.place_count, {0, std::chrono::minutes{0}}) {
        for (std::size_t i = 0; i < info.place_count; ++i) {
            free_places.insert(i);
        }
    }

    std::pair<std::vector<std::pair<uint64_t, std::chrono::minutes>>, std::vector<event>> event_processing() {
        std::vector<event> processed_events;

        for (auto&& e: events) {
            if (e.time > info.end_time && !clients_in_club.empty()) {
                clear_computer_club(processed_events);
            }
            processed_events.emplace_back(e);
            if (e.time < info.start_time || e.time > info.end_time) {
                add_error(processed_events, "NotOpenYet", e);
                continue;
            }
            switch (e.type) {
                case event_type::CLIENT_ARRIVE:
                    if (clients_in_club.contains(e.client_name)) {
                        add_error(processed_events, "YouShallNotPass", e);
                    } else {
                        clients_in_club.emplace(e.client_name, client_state::IN);
                    }
                    break;
                case event_type::CLIENT_LOCK_TABLE:
                    if (!clients_in_club.contains(e.client_name)) {
                        add_error(processed_events, "ClientUnknown", e);
                    } else if (places[e.place_no - 1] != ntime) {
                        add_error(processed_events, "PlaceIsBusy", e);
                    } else {
                        int pos = static_cast<int>(clients_in_club[e.client_name]);
                        clients_in_club[e.client_name] = client_state{static_cast<int>(e.place_no - 1)};
                        places[e.place_no - 1] = e.time;
                        free_places.erase(e.place_no - 1);
                        if (pos >= 0) {
                            free_place(processed_events, pos, e.time);
                        }
                    }
                    break;
                case event_type::CLIENT_WAITING:
                    if (!clients_in_club.contains(e.client_name)) {
                        add_error(processed_events, "ClientUnknown", e); // TODO
                        continue;
                    }
                    if (!free_places.empty()) {
                        add_error(processed_events, "ICanWaitNoLonger!", e);
                    } else if (clients_queue.size() - skip_clients.size() >= info.place_count) {
                        processed_events.emplace_back(event{
                                .time = e.time,
                                .type = event_type::OUT_CLIENT_LEFT,
                                .client_name = e.client_name
                        });
                        if (clients_in_club[e.client_name] == client_state::WAITING) {
                            skip_clients.insert(e.client_name);
                        }
                        clients_in_club.erase(e.client_name);
                    } else {
                        clients_in_club[e.client_name] = client_state::WAITING;
                        clients_queue.push(e.client_name);
                    }
                    break;
                case event_type::CLIENT_LEFT:
                    if (!clients_in_club.contains(e.client_name)) {
                        add_error(processed_events, "ClientUnknown", e);
                    } else {
                        switch (clients_in_club[e.client_name]) {
                            case client_state::IN:
                                // nothing
                                break;
                            case client_state::WAITING:
                                skip_clients.insert(e.client_name);
                                break;
                            default:
                                int pos = static_cast<int>(clients_in_club[e.client_name]);
                                free_place(processed_events, pos, e.time);
                                break;
                        }
                        clients_in_club.erase(e.client_name);
                    }
                    break;
                default:
                    // skip other events
                    break;
            }
        }

        clear_computer_club(processed_events);

        return {place_amount, processed_events};
    }

private:
    void free_place(std::vector<event>& result, int place, std::chrono::minutes end_time) {
        place_amount[place].first += info.cost * (((end_time - places[place]).count() + 59) / 60);
        place_amount[place].second += end_time - places[place];
        places[place] = ntime;
        while (!clients_queue.empty()) {
            auto client = clients_queue.front();
            clients_queue.pop();
            auto it = skip_clients.find(client);
            if (it != skip_clients.end()) {
                skip_clients.erase(it);
            } else {
                places[place] = end_time;
                result.emplace_back(event{
                        .time = end_time,
                        .type = event_type::OUT_CLIENT_LOCK_TABLE,
                        .client_name = client,
                        .place_no = static_cast<std::size_t>(place) + 1
                });
                clients_in_club[client] = static_cast<client_state>(place);
                return;
            }
        }
        free_places.insert(place);
    }

    void clear_computer_club(std::vector<event>& result) {
        for (auto &&[client, state]: clients_in_club) {
            result.emplace_back(event{
                    .time = info.end_time,
                    .type = event_type::OUT_CLIENT_LEFT,
                    .client_name = client,
            });
            int pos = static_cast<int>(state);
            if (pos >= 0) {
                place_amount[pos].first += info.cost * (((info.end_time - places[pos]).count() + 59) / 60);
                place_amount[pos].second += info.end_time - places[pos];
                places[pos] = ntime;
                free_places.insert(pos);
            }
        }
        clients_in_club.clear();
    }

    static void add_error(std::vector<event>& result, std::string msg, const event& e) {
        result.emplace_back(event{
                .time = e.time,
                .type = event_type::OUT_ERROR,
                .msg = std::move(msg)
        });
    }

    constexpr static std::chrono::minutes ntime{-1}; // mark place unused

    club_info info;
    std::vector<event> events;

    std::multiset<std::string> skip_clients;
    std::queue<std::string> clients_queue;
    std::map<std::string, client_state> clients_in_club;

    std::set<std::size_t> free_places;
    std::vector<std::chrono::minutes> places;
    std::vector<std::pair<uint64_t, std::chrono::minutes>> place_amount;
};


std::pair<std::vector<std::pair<uint64_t, std::chrono::minutes>>, std::vector<event>>
processing::event_processing(std::vector<event> events, club_info info) {
    return computer_club(std::move(events), info).event_processing();
}