#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "item.hpp"
#include <luggagemaker/api/weather_client.hpp>

using namespace luggagemaker::core;

enum class BagType {
    Cabin,
    Checked
};

struct Bag {
    int max_weight;
    BagType type;
    std::vector <Item> items;
};

class LuggagePacker
{
    public:
        explicit LuggagePacker(std::vector<Bag> bags, int max_weight_) : bags(bags), max_wg(max_weight_) {}
        void pack(const std::vector<Item>& items, const WeatherResult& weather, const std::vector<std::string>& planned_activities, std::string gender);
        [[nodiscard]] const std::vector<Bag>& get_bags() const { return bags; }
        int get_max_weight() const { return max_wg; }
    private:
        std::vector<Bag> bags;
        int max_wg;
};
