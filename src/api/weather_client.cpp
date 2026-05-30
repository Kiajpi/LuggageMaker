#include <luggagemaker/api/weather_client.hpp>

WeatherResult WeatherClient::fetchData(double lat, double lon, const std::string &startDate, const std::string &endDate)
{
    WeatherResult result;
    auto response = cpr::Get(
    cpr::Url{"https://api.open-meteo.com/v1/forecast"},
    cpr::Parameters{
        {"latitude", std::to_string(lat)},
        {"longitude", std::to_string(lon)},
        {"start_date", startDate},
        {"end_date", endDate},
        {"daily", "apparent_temperature_max,apparent_temperature_min,precipitation_probability_max"},
        {"timezone", "auto"}
    }
    );

    if (response.status_code !=  200) {
        result.works = false;
        return result;
    }

    try {
        auto data = nlohmann::json::parse(response.text);
        result.max_temperature= data["daily"]["apparent_temperature_max"].get<std::vector<double>>();
        result.min_temperature = data["daily"]["apparent_temperature_min"].get<std::vector<double>>();
        result.rain_probability = data["daily"]["precipitation_probability_max"].get<std::vector<double>>();
        result.works = true;
    }
    catch (nlohmann::json::exception& e) {
        result.works = false;
    }

    return result;

}
