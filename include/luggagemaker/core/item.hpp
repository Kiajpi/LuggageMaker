#pragma once
#include <string>
#include <map>
#include <nlohmann/json.hpp>

namespace luggagemaker::core {

struct AdvancedWeather
{
    double idealTemperature = 20.0;
    double sensitivityLow = 0.0;
    double sensitivityHigh = 0.0;
    std::map<std::string, int> combinations;
};

struct Item
{
    int id;
    std::string category;
    std::string name_pl;
    std::string name_en;
    int weight;
    bool is_important;
    std::string quantity_rule = "max_one";
    bool day_scaling = false;
    int base_priority = 50;
    AdvancedWeather weather;
    std::map<std::string, int> activity_triggers;
    std::string allowed = "any";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AdvancedWeather, idealTemperature, sensitivityLow, sensitivityHigh, combinations)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Item, id, category, name_pl, name_en, weight, is_important, quantity_rule, day_scaling, base_priority, weather, activity_triggers, allowed)

}