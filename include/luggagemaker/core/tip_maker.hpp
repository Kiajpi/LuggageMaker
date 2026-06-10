//
// Created by Kacper Laba on 10/06/2026.
//

#ifndef LUGGAGEMAKER_TIP_MAKER_H
#define LUGGAGEMAKER_TIP_MAKER_H
#include <string>
#include <nlohmann/json.hpp>

namespace luggagemaker::core {
    struct Tip_
    {
        std::string city;
        double lat;
        double lon;
        std::string tip;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Tip_,city,lat,lon,tip);
}
#endif //LUGGAGEMAKER_TIP_MAKER_H