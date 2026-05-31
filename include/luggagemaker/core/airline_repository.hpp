#pragma once
#include <string>
#include <vector>
#include <map>
#include <luggagemaker/core/luggage_packer.hpp>

namespace luggagemaker {
    namespace core {

        struct BagPreset {
            std::string name;
            BagType type;
            int max_weight;
        };

        class AirlineRepository {
        public:
            static std::map<std::string, std::map<std::string, std::vector<BagPreset>>> loadAirlines(const std::string& filePath);
        };

    }
}