#include "luggagemaker/ui/ui.hpp"
#include "luggagemaker/core/airline_repository.hpp"
#include "luggagemaker/core/item_repository.hpp"
#include "luggagemaker/api/weather_client.hpp"
#include "luggagemaker/core/luggage_packer.hpp"
#include <imgui.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <map>
#include <algorithm>

using namespace luggagemaker::core;

enum class AppScreen { Setup, Results };
static AppScreen currentScreen = AppScreen::Setup;
static float animAlpha = 0.0f;

static std::map<std::string, std::map<std::string, std::vector<BagPreset>>> airlinePresets;
static ItemRepository itemRepository;
static bool repositoriesLoaded = false;

static char cityInput[128] = "Reykjavik";
static char departureDate[16] = "2026-07-01";
static char returnDate[16] = "2026-07-08";
static std::string selectedAirline = "";
static std::string selectedPackage = "";

static bool actSpa = false;
static bool actBeach = false;
static bool actSki = false;

static std::vector<Bag> packedBagsResults;

void ApplyModernDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 16.0f;
    style.FrameRounding = 10.0f;
    style.ChildRounding = 14.0f;
    style.ItemSpacing = ImVec2(16, 16);
    style.FramePadding = ImVec2(16, 12);
    style.WindowPadding = ImVec2(25, 25);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]         = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);
    colors[ImGuiCol_ChildBg]          = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_FrameBg]          = ImVec4(0.17f, 0.17f, 0.19f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_Button]           = ImVec4(0.03f, 0.51f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonHovered]    = ImVec4(0.1f, 0.6f, 1.00f, 1.00f);
    colors[ImGuiCol_Header]           = ImVec4(0.17f, 0.17f, 0.19f, 1.00f);
    colors[ImGuiCol_HeaderHovered]    = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_CheckMark]        = ImVec4(0.03f, 0.51f, 1.00f, 1.00f);
}

void DrawAppleStyleLuggage(const std::string& type_name, int current_weight, int max_weight, ImVec2 pos, float width) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float height = 110.0f;
    ImVec2 p_max = ImVec2(pos.x + width, pos.y + height);

    float fill_pct = (max_weight > 0) ? (float)current_weight / (float)max_weight : 0.0f;
    fill_pct = std::min(fill_pct, 1.0f);

    draw_list->AddRectFilled(pos, p_max, IM_COL32(28, 28, 30, 255), 14.0f);

    ImU32 accent_color = (fill_pct > 0.95f) ? IM_COL32(255, 69, 58, 255) : IM_COL32(52, 199, 89, 255);

    ImVec2 icon_center = ImVec2(pos.x + 45.0f, pos.y + height * 0.5f);

    if (max_weight <= 5000) {
        draw_list->AddRectFilled(ImVec2(icon_center.x - 18, icon_center.y - 15), ImVec2(icon_center.x + 18, icon_center.y + 25), IM_COL32(68, 68, 70, 255), 10.0f, ImDrawFlags_RoundCornersTop);
        draw_list->AddRectFilled(ImVec2(icon_center.x - 14, icon_center.y + 3), ImVec2(icon_center.x + 14, icon_center.y + 23), IM_COL32(98, 98, 102, 255), 4.0f);
        draw_list->AddCircle(ImVec2(icon_center.x, icon_center.y - 15), 5.0f, IM_COL32(142, 142, 147, 255), 12, 2.0f);
    }
    else if (max_weight <= 10000) {
        draw_list->AddRectFilled(ImVec2(icon_center.x - 18, icon_center.y - 20), ImVec2(icon_center.x + 18, icon_center.y + 20), IM_COL32(58, 122, 247, 255), 6.0f);
        draw_list->AddRect(ImVec2(icon_center.x - 8, icon_center.y - 28), ImVec2(icon_center.x + 8, icon_center.y - 20), IM_COL32(142, 142, 147, 255), 2.0f, 0, 2.0f);
        draw_list->AddCircleFilled(ImVec2(icon_center.x - 10, icon_center.y + 22), 3.0f, IM_COL32(255, 255, 255, 255));
        draw_list->AddCircleFilled(ImVec2(icon_center.x + 10, icon_center.y + 22), 3.0f, IM_COL32(255, 255, 255, 255));
    }
    else {
        draw_list->AddRectFilled(ImVec2(icon_center.x - 22, icon_center.y - 25), ImVec2(icon_center.x + 22, icon_center.y + 23), IM_COL32(44, 44, 46, 255), 8.0f);
        draw_list->AddLine(ImVec2(icon_center.x - 16, icon_center.y - 10), ImVec2(icon_center.x + 16, icon_center.y - 10), IM_COL32(98, 98, 102, 255), 2.0f);
        draw_list->AddLine(ImVec2(icon_center.x - 16, icon_center.y + 2), ImVec2(icon_center.x + 16, icon_center.y + 2), IM_COL32(98, 98, 102, 255), 2.0f);
        draw_list->AddLine(ImVec2(icon_center.x - 16, icon_center.y + 14), ImVec2(icon_center.x + 16, icon_center.y + 14), IM_COL32(98, 98, 102, 255), 2.0f);
        draw_list->AddRect(ImVec2(icon_center.x - 10, icon_center.y - 31), ImVec2(icon_center.x + 10, icon_center.y - 25), IM_COL32(142, 142, 147, 255), 2.0f, 0, 2.0f);
        draw_list->AddCircleFilled(ImVec2(icon_center.x - 12, icon_center.y + 25), 4.0f, IM_COL32(142, 142, 147, 255));
        draw_list->AddCircleFilled(ImVec2(icon_center.x + 12, icon_center.y + 25), 4.0f, IM_COL32(142, 142, 147, 255));
    }

    float text_start_x = pos.x + 90.0f;
    draw_list->AddText(ImVec2(text_start_x, pos.y + 16.0f), IM_COL32(255, 255, 255, 255), type_name.c_str());

    char pct_str[32];
    snprintf(pct_str, sizeof(pct_str), "%d%%", (int)(fill_pct * 100.0f));
    draw_list->AddText(ImVec2(p_max.x - 55.0f, pos.y + 16.0f), accent_color, pct_str);

    ImVec2 bar_min = ImVec2(text_start_x, pos.y + 48.0f);
    ImVec2 bar_max = ImVec2(p_max.x - 20.0f, pos.y + 56.0f);
    float bar_width = bar_max.x - bar_min.x;

    draw_list->AddRectFilled(bar_min, bar_max, IM_COL32(44, 44, 46, 255), 4.0f);
    if (fill_pct > 0.0f) {
        ImVec2 bar_fill_max = ImVec2(bar_min.x + (bar_width * fill_pct), bar_max.y);
        draw_list->AddRectFilled(bar_min, bar_fill_max, accent_color, 4.0f);
    }

    char weight_info[64];
    snprintf(weight_info, sizeof(weight_info), "Zapakowano: %d g z limitu %d g", current_weight, max_weight);
    draw_list->AddText(ImVec2(text_start_x, pos.y + 72.0f), IM_COL32(142, 142, 147, 255), weight_info);
}

void RenderPackingAppUI() {
    if (!repositoriesLoaded) {
        airlinePresets = AirlineRepository::loadAirlines("assets/airlines.json");
        itemRepository.loadFromJson("assets/items.json");
        if (!airlinePresets.empty()) {
            selectedAirline = airlinePresets.begin()->first;
            selectedPackage = airlinePresets[selectedAirline].begin()->first;
        }
        repositoriesLoaded = true;
    }

    animAlpha += ImGui::GetIO().DeltaTime * 4.0f;
    if (animAlpha > 1.0f) animAlpha = 1.0f;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("MainViewport", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, animAlpha);

    if (currentScreen == AppScreen::Setup) {
        ImGui::Spacing();
        ImGui::SetWindowFontScale(1.3f);
        ImGui::TextColored(ImVec4(1,1,1,1), "Gdzie się wybierasz?");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::TextDisabled("Ustaw cel podróży, aby dopasować algorytm pakowania.");
        ImGui::Spacing(); ImGui::Spacing();

        ImGui::BeginChild("CardLocation", ImVec2(0, 210), true);
        ImGui::SetCursorPos(ImVec2(20, 15)); ImGui::TextColored(ImVec4(0.03f, 0.51f, 1.00f, 1.00f), "PARAMETRY TERMINU");

        ImGui::SetCursorPos(ImVec2(20, 45));  ImGui::InputText("Kierunek docelowy", cityInput, IM_ARRAYSIZE(cityInput));
        ImGui::SetCursorPos(ImVec2(20, 95));  ImGui::InputText("Data wylotu", departureDate, IM_ARRAYSIZE(departureDate));
        ImGui::SetCursorPos(ImVec2(20, 145)); ImGui::InputText("Data powrotu", returnDate, IM_ARRAYSIZE(returnDate));
        ImGui::EndChild();

        ImGui::BeginChild("CardAirline", ImVec2(0, 160), true);
        ImGui::SetCursorPos(ImVec2(20, 15)); ImGui::TextColored(ImVec4(0.03f, 0.51f, 1.00f, 1.00f), "PRZEWOŹNIK I TARYFA WAGOWA");

        ImGui::SetCursorPos(ImVec2(20, 45));
        if (ImGui::BeginCombo("Linia", selectedAirline.c_str())) {
            for (const auto& [airlineName, _] : airlinePresets) {
                if (ImGui::Selectable(airlineName.c_str(), selectedAirline == airlineName)) {
                    selectedAirline = airlineName;
                    selectedPackage = airlinePresets.at(selectedAirline).begin()->first;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SetCursorPos(ImVec2(20, 95));
        if (ImGui::BeginCombo("Zestaw bagaży", selectedPackage.c_str())) {
            if (airlinePresets.contains(selectedAirline)) {
                for (const auto& [packageName, _] : airlinePresets.at(selectedAirline)) {
                    if (ImGui::Selectable(packageName.c_str(), selectedPackage == packageName)) {
                        selectedPackage = packageName;
                    }
                }
            }
            ImGui::EndCombo();
        }
        ImGui::EndChild();

        ImGui::BeginChild("CardActivities", ImVec2(0, 195), true); // Zwiększono wysokość karty
        ImGui::SetCursorPos(ImVec2(20, 15)); ImGui::TextColored(ImVec4(0.03f, 0.51f, 1.00f, 1.00f), "STYL I AKTYWNOŚCI URLOPOWE");

        ImGui::SetCursorPos(ImVec2(20, 50));  ImGui::Checkbox("Relaks w SPA / Basen kryty", &actSpa);
        ImGui::SetCursorPos(ImVec2(20, 95));  ImGui::Checkbox("Słońce, plażowanie i wysokie temperatury", &actBeach);
        ImGui::SetCursorPos(ImVec2(20, 140)); ImGui::Checkbox("Góry i sporty zimowe (Narty/Snowboard)", &actSki);
        ImGui::EndChild();

        ImGui::Spacing();
        if (ImGui::Button("GENERUJ INTELIGENTNĄ LISTĘ ->", ImVec2(-1, 55))) {
            double lat = 64.1265; double lon = -21.8174;
            auto geoRes = cpr::Get(cpr::Url{"https://geocoding-api.open-meteo.com/v1/search"},
                                   cpr::Parameters{{"name", cityInput}, {"count", "1"}});
            if (geoRes.status_code == 200) {
                auto geoJson = nlohmann::json::parse(geoRes.text);
                if (geoJson.contains("results") && !geoJson["results"].empty()) {
                    lat = geoJson["results"][0]["latitude"];
                    lon = geoJson["results"][0]["longitude"];
                }
            }

            auto pogoda = WeatherClient::fetchData(lat, lon, departureDate, returnDate);

            std::vector<Bag> available_bags;
            for (const auto& preset : airlinePresets.at(selectedAirline).at(selectedPackage)) {
                available_bags.push_back(Bag{ preset.max_weight, preset.type, {} });
            }

            std::vector<std::string> activities;
            if (actSpa)   activities.push_back("spa");
            if (actBeach) activities.push_back("beach");
            if (actSki)   activities.push_back("skiing");

            LuggagePacker packer(available_bags);
            packer.pack(itemRepository.getItems(), pogoda, activities);

            packedBagsResults = packer.get_bags();

            currentScreen = AppScreen::Results;
            animAlpha = 0.0f;
        }
    }

    else if (currentScreen == AppScreen::Results) {
        if (ImGui::Button("<- Wróć do edycji", ImVec2(180, 40))) {
            currentScreen = AppScreen::Setup;
            animAlpha = 0.0f;
        }
        ImGui::SameLine();
        ImGui::TextDisabled(" |  Twój spakowany wyjazd do: %s", cityInput);
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::BeginTable("DashboardSplit", 2, ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupColumn("Ubrania", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Walizki", ImGuiTableColumnFlags_WidthFixed, 420.0f);
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::BeginChild("IosPackingList", ImVec2(0, 0), false);

            for (size_t i = 0; i < packedBagsResults.size(); ++i) {
                const auto& bag = packedBagsResults[i];
                std::string type_label = (bag.type == BagType::Cabin) ? "Bagaż podręczny" : "Bagaż rejestrowany";

                ImGui::SetWindowFontScale(1.15f);
                ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.65f, 1.0f), "%s %zu", type_label.c_str(), i + 1);
                ImGui::SetWindowFontScale(1.0f);
                ImGui::Spacing();

                std::map<std::string, int> count_map;
                for (const auto& item : bag.items) {
                    count_map[item.name_pl]++;
                }

                if (count_map.empty()) {
                    ImGui::TextDisabled("  Pusta walizka - brak wymaganych rzeczy.");
                } else {
                    for (const auto& [name, count] : count_map) {
                        ImVec2 start_pos = ImGui::GetCursorScreenPos();
                        float row_w = ImGui::GetContentRegionAvail().x;

                        ImGui::GetWindowDrawList()->AddRectFilled(start_pos, ImVec2(start_pos.x + row_w, start_pos.y + 44.0f), IM_COL32(28, 28, 30, 255), 8.0f);

                        ImGui::SetCursorScreenPos(ImVec2(start_pos.x + 15.0f, start_pos.y + 12.0f));
                        ImGui::Text("%s", name.c_str());

                        ImVec2 badge_pos = ImVec2(start_pos.x + row_w - 55.0f, start_pos.y + 10.0f);
                        ImGui::GetWindowDrawList()->AddRectFilled(badge_pos, ImVec2(badge_pos.x + 40.0f, badge_pos.y + 24.0f), IM_COL32(44, 44, 46, 255), 12.0f);

                        char cnt_str[12]; snprintf(cnt_str, sizeof(cnt_str), "x%d", count);
                        ImGui::GetWindowDrawList()->AddText(ImVec2(badge_pos.x + 12.0f, badge_pos.y + 4.0f), IM_COL32(0, 122, 255, 255), cnt_str);

                        ImGui::SetCursorScreenPos(ImVec2(start_pos.x, start_pos.y + 50.0f));
                    }
                }
                ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
            }
            ImGui::EndChild();

            ImGui::TableSetColumnIndex(1);
            ImGui::BeginChild("IosSuitcasesVisualizer", ImVec2(0, 0), false);

            ImGui::SetWindowFontScale(1.3f);
            ImGui::Text("Twoje bagaże");
            ImGui::SetWindowFontScale(1.0f);
            ImGui::Spacing(); ImGui::Spacing();

            for (size_t i = 0; i < packedBagsResults.size(); ++i) {
                const auto& bag = packedBagsResults[i];

                std::string title = "Walizka Duża";
                if (bag.max_weight <= 5000) title = "Plecak";
                else if (bag.max_weight <= 10000) title = "Walizka Kabinowa";

                int current_weight = 0;
                for (const auto& item : bag.items) current_weight += item.weight;

                ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
                float available_width = ImGui::GetContentRegionAvail().x;

                DrawAppleStyleLuggage(title, current_weight, bag.max_weight, cursor_pos, available_width);

                ImGui::SetCursorScreenPos(ImVec2(cursor_pos.x, cursor_pos.y + 130.0f));
            }

            ImGui::EndChild();
        }
        ImGui::EndTable();
    }

    ImGui::PopStyleVar();
    ImGui::End();
}