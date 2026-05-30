#include <luggagemaker/core/item_repository.hpp>
using namespace luggagemaker::core;
using json = nlohmann::json;

bool ItemRepository::loadFromJson(const std::string &path_file)
{
    std::ifstream file(path_file);

    try
    {
        json j;
        file >> j;

        items = j["items"].get<std::vector<Item>>();
    }
    catch (json::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

const std::vector<Item> &ItemRepository::getItems() const
{
    return items;
}
