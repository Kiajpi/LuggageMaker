#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <luggagemaker/core/item.hpp>
#include <luggagemaker/core/item_repository.hpp>
#include <luggagemaker/api/weather_client.hpp>

using json = nlohmann::json;
using namespace luggagemaker::core;

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string flag = argv[1];
        if (flag == "-h" || flag == "--help") return 0;
    }

    std::cout << "Test listy ubrań z json'a" << std::endl;
    std::string path_file = "assets/items.json";

    ItemRepository item_repository;
    if (item_repository.loadFromJson(path_file))
    {
        for (const auto& item : item_repository.getItems()) {
            std::cout << item.name_pl << std::endl;
            std::cout << item.weight << std::endl;
            std::cout << item.weather.idealTemperature << std::endl;
            std::cout << item.allowed << std::endl;
            std::cout << std::endl;
        }
    }

    auto pogoda = WeatherClient::fetchData(41.9028, 12.4964, "2026-06-05", "2026-06-12");
    if (pogoda.works == false) {
        std::cerr << "Blad pobieranie pogody";
    }
    else {
        for (auto& temp : pogoda.max_temperature) {
            std::cout << temp << ", ";
        }
        std::cout << std::endl;
        for (auto& temp : pogoda.min_temperature) {
            std::cout << temp << ", ";
        }
        std::cout << std::endl;
        for (const auto& temp : pogoda.rain_probability) {
            std::cout << temp << ", ";
        }
    }

    return 0;
}