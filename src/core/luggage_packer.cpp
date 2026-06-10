#include <luggagemaker/core/luggage_packer.hpp>
#include <algorithm>
#include <cmath>

using namespace luggagemaker::core;

struct RatedItem
{
    Item item;
    double priority;

    bool operator>(const RatedItem& other) const
    {
        if (item.is_important != other.item.is_important) return item.is_important > other.item.is_important;
        return priority > other.priority;
    }
};

void LuggagePacker::pack(const std::vector<Item> &items, const WeatherResult &weather, const std::vector<std::string>& planned_activities, std::string gender)
{
    std::vector<RatedItem> rated_items;
    const size_t num_days = weather.works ? weather.max_temperature.size() : 1;

    for (const auto& item : items)
    {
        double max_priority = item.base_priority;

        if (item.gender != "unisex" && item.gender != gender) continue;

        if (!item.is_important && weather.works) {
            for (size_t d = 0; d < num_days; ++d)
            {
                double day_priority = item.base_priority;
                const double t_max = weather.max_temperature[d];
                const double t_min = weather.min_temperature[d];
                const double rain_prob = weather.rain_probability[d];

                if (t_max > item.weather.idealTemperature) day_priority += (t_max - item.weather.idealTemperature) * (item.weather.sensitivityHigh) ;
                if (t_min < item.weather.idealTemperature) day_priority += (item.weather.idealTemperature - t_min) * (item.weather.sensitivityLow);

                if (rain_prob > 30.0)
                {
                    if (t_min <= 3.0 && item.weather.combinations.contains("snow")) day_priority += (item.weather.combinations.at("snow")/2);
                    else if (item.weather.combinations.contains("rain"))  day_priority += (item.weather.combinations.at("rain")/2);
                }
                max_priority = std::max(max_priority, day_priority);
            }
        }

        double activity_bonus = 0.0;

        for (const auto& activity : planned_activities)
        {
            if (item.activity_triggers.count(activity) > 0)  activity_bonus += (item.activity_triggers.at(activity)/2);
        }

        const double final_priority = activity_bonus + max_priority;
        const int limit = max_wg > 10000 ? static_cast<int>(num_days*4) : static_cast<int>(num_days*3/2);

        const int m = (item.quantity_rule == "max_one") ? 1 : limit;

        double dynamic_priority = final_priority;

        for (int i = 0; i < m; ++i)
        {
            if (i>0)
            {
                if (max_wg > 5000) dynamic_priority = dynamic_priority*0.85;
                else dynamic_priority = dynamic_priority*0.7;
            }
            rated_items.push_back({item, dynamic_priority});
        }
    }

    std::ranges::sort(rated_items,std::greater<RatedItem>());

    for (const auto&[item, priority] : rated_items)
    {
        std::vector<Bag*> bags_order;

        if (item.allowed == "cabin_only") {for (auto& bag : bags) if (bag.type == BagType::Cabin) bags_order.push_back(&bag);}
        else if (item.allowed == "checked_only") {for (auto& bag : bags) if (bag.type == BagType::Checked) bags_order.push_back(&bag);}
        else if (item.allowed == "any")
        {
            for (auto& bag : bags) if (bag.type == BagType::Checked) bags_order.push_back(&bag);
            for (auto& bag : bags) if (bag.type == BagType::Cabin) bags_order.push_back(&bag);
        }

        for (auto* bag_ptr : bags_order)
        {
            auto& bag = *bag_ptr;

            int curr_weight = 0;
            for (const auto& packed_item : bag.items) curr_weight += packed_item.weight;

            if (curr_weight + item.weight <= bag.max_weight)
            {
                bag.items.push_back(item);
                break;
            }
        }
    }
}
