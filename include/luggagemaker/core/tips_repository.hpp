#include <iostream>
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "tip_maker.hpp"

using namespace luggagemaker::core;

class TipRepository
{
    public:
        TipRepository() = default;
        ~TipRepository() = default;

        bool loadFromJson(const std::string& filePath);
        [[nodiscard]] const std::vector<Tip_>& getItems() const;

    private:
        std::vector<Tip_> tips_list;
};