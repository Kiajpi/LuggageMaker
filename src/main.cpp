#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string flag = argv[1];
        if (flag == "-h" || flag == "--help")
        {
            std::cout << "====================================================\n";
            std::cout << "         LuggageMaker - Instrukcja Pomocy           \n";
            std::cout << "====================================================\n";
            std::cout << "Uzycie: ./LuggageMaker [opcje]\n\n";
            std::cout << "Opcje:\n";
            std::cout << "  -h, --help    Wyswietla ten tekst pomocy\n\n";
            std::cout << "Opis:\n";
            std::cout << "  Aplikacja ulatwia optymalne pakowanie bagazu na\n";
            std::cout << "  podstawie prognozy pogody API oraz taryf linii.\n";
            std::cout << "====================================================\n";
            return 0;
        }
    }

    std::cout << "[LuggageMaker] Uruchamianie bazy danych i testu sieci...\n";

    cpr::Response r = cpr::Get(cpr::Url{"https://httpbin.org/get"});

    if (r.status_code == 200)
    {
        std::cout << "[SUKCES] Polaczenie sieciowe z httpbin.org dziala!\n";

        auto data = nlohmann::json::parse(r.text);
        std::cout << "[INFO] Tworze obiekt testowy JSON. Twoje IP: " << data["origin"] << "\n";
    }
    else
    {
        std::cerr << "[BLAD] Nie udalo sie polaczyc z API testowym. Kod bledu cpr: " << r.status_code << "\n";
    }

    std::cout << "[LuggageMaker] Gotowy do kroku 2 (Parsowanie items.json)!\n";
    return 0;
}