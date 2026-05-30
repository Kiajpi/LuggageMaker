#include <iostream>
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "item.hpp"

using namespace luggagemaker::core;

class ItemRepository
{
    public:
        ItemRepository() = default;
        ~ItemRepository() = default;

        bool loadFromJson(const std::string& filePath);
        [[nodiscard]] const std::vector<Item>& getItems() const;

    private:
        std::vector <Item> items;

};

