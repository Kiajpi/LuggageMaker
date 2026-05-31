#include "luggagemaker/core/airline_repository.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using namespace luggagemaker::core;

std::map<std::string, std::map<std::string, std::vector<BagPreset>>> AirlineRepository::loadAirlines(const std::string& filePath) {
    std::map<std::string, std::map<std::string, std::vector<BagPreset>>> airlines;
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku taryf: " << filePath << std::endl;
        return airlines;
    }

    nlohmann::json j;
    try {
        file >> j;
        for (auto& [airlineName, packagesJson] : j.items()) {
            std::map<std::string, std::vector<BagPreset>> packages;

            for (auto& [packageName, bagsArray] : packagesJson.items()) {
                std::vector<BagPreset> presetsList;

                if (bagsArray.is_array()) {
                    for (auto& bagJson : bagsArray) {
                        std::string originalType = bagJson.value("type", "");
                        int weight = bagJson.value("weight_g", 0);

                        BagType bType = BagType::Cabin;
                        if (originalType.find("checked") != std::string::npos) {
                            bType = BagType::Checked;
                        }

                        presetsList.push_back(BagPreset{ originalType, bType, weight });
                    }
                }
                packages[packageName] = presetsList;
            }
            airlines[airlineName] = packages;
        }
    } 
    catch (const std::exception& e) {
        std::cerr << "Blad parsowania taryf: " << e.what() << std::endl;
    }

    return airlines;
}