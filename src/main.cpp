#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <luggagemaker/core/item.hpp>
#include <luggagemaker/core/item_repository.hpp>
#include <luggagemaker/api/weather_client.hpp>
#include <luggagemaker/core/luggage_packer.hpp>

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

    auto pogoda = WeatherClient::fetchData(64.1265, -21.8174, "2026-05-30", "2026-06-06");
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

    Bag plecak{ 3000, BagType::Cabin, {} };
    //Bag walizka_1{ 10000, BagType::Checked, {} };

    std::vector<Bag> available_bags = { plecak};

    LuggagePacker packer(available_bags);
    std::vector<std::string> activities = {"spa"};

    packer.pack(item_repository.getItems(), pogoda, activities);

    const auto& packed_bags = packer.get_bags();

    for (size_t i = 0; i < packed_bags.size(); ++i) {
        const auto& bag = packed_bags[i];
        std::string type_str = (bag.type == BagType::Cabin) ? "PODRĘCZNY (Cabin)" : "REJESTROWANY (Checked)";

        std::cout << "--------------------------------------------------\n";
        std::cout << "BAGAŻ #" << i + 1 << ": " << type_str << "\n";
        std::cout << "Limit: " << bag.max_weight << "g\n";
        std::cout << "Zawartość:\n";

        int total_weight = 0;
        std::map<std::string, int> item_counter;

        for (const auto& item : bag.items) {
            item_counter[item.name_pl]++;
            total_weight += item.weight;
        }

        if (item_counter.empty()) {
            std::cout << "  (pusty)\n";
        } else {
            for (const auto& pair : item_counter) {
                std::cout << "  - " << pair.first << " x" << pair.second << "\n";
            }
        }
        std::cout << "Waga całkowita: " << total_weight << "g\n";
    }
    std::cout << "--------------------------------------------------\n";


    return 0;
}