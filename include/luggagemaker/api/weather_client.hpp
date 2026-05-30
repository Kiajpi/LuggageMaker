#include <iostream>
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

struct WeatherResult {
    std::vector<double> max_temperature;
    std::vector<double> min_temperature;
    std::vector<double> rain_probability;
    bool works = false;
};

class WeatherClient
{
    public:
        static WeatherResult fetchData(double lat, double lon, const std::string& startDate, const std::string& endDate);

};

