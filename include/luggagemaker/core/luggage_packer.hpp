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
    private:
        std::vector<Bag> bags;
    public:
        explicit LuggagePacker(std::vector<Bag> bags) : bags(std::move(bags)) {}
        void pack(const std::vector<Item>& items, const WeatherResult& weather, const std::vector<std::string>& planned_activities);
        const std::vector<Bag>& get_bags() const { return bags; }
};
