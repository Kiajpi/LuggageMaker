#include <luggagemaker/core/tips_repository.hpp>
using namespace luggagemaker::core;
using json = nlohmann::json;

bool TipRepository::loadFromJson(const std::string &filePath)
{
    std::ifstream file(filePath);

    try
    {
        json j;
        file >> j;

        tips_list = j.get<std::vector<Tip_>>();
    }
    catch (json::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

const std::vector<Tip_> &TipRepository::getItems() const
{
    return tips_list;
}
