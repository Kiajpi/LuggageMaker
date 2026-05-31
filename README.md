# LuggageMaker

**Version:** 0.2.0  
**Last Updated:** 2026-05-31

LuggageMaker is an intelligent packing assistant designed to optimize luggage weight and content based on airline constraints, destination weather, and specific travel activities.

This project was created as an academic assignment to demonstrate proficiency in C++, API integration, and GUI development using Dear ImGui.

---

## Features

* **Smart Packing Algorithm**: Calculates optimal items based on baggage limits.
* **Weather Integration**: Automatically fetches weather data for the destination (Open-Meteo API) to suggest appropriate clothing.
* **Modern UI**: A responsive dashboard built with Dear ImGui.
* **Activity Support**: Customizable packing lists for different trip types.

---

## Dependencies

* Dear ImGui – Graphical User Interface
* cpr – HTTP requests and API integration
* nlohmann/json – JSON parsing
* CMake 3.15 or higher
* C++17 compatible compiler

---

## How to Build

### Clone the Repository

```bash
git clone <repository_url>
cd LuggageMaker
```

### Build the Project

```bash
mkdir build
cd build
cmake ..
make
```

### Run

```bash
./LuggageMaker
```

---

## Usage

1. Launch the application.
2. Enter your travel destination and travel dates.
3. Select your airline and baggage allowance.
4. Choose your planned activities (e.g., SPA, Beach, Skiing).
5. Click **Generate**.
6. Review your optimized packing list and suitcase visualization.

---

## AI Disclosure

This project was developed with the assistance of AI (LLM) tools.

AI was used for:

* Designing Icons in the UI/UX architecture using Dear ImGui.
* Drafting project documentation.
* Generating Item list consisting of 100 items.
* Generating Luggage Policies from different airlines.

All generated code was reviewed, modified, tested, and integrated by the project author.

---

### Notes

Please ensure all dependencies are installed before building the project.

The application requires an active internet connection to retrieve weather data from the Open-Meteo API.

---

## Project Structure
```text
.
├── assets/                   # JSON assets used in the project
│   ├── airlines.json            # Airlines bag policies
│   └── items.json               # List of items that can be placed in luggages
├── include/                  # .hpp files
│   └── lugagemaker/
│       ├── api/
│       │   └── weather_client.hpp    # Fetching weather data
│       ├── core/
│       │   ├── airline_repository.hpp    # Luggage limits from airlines.json 
│       │   ├── item.hpp                  # Item class
│       │   ├── item_repository.hpp       # Items data from items.json
│       │   └── luggage_packer.hpp        # Algorithm for optimised packing luggage
│       └── ui/
│           └── ui.hpp                 # Ui
├── src/                      # .cpp files
│   ├── api/
│   │   └── weather_client.cpp        # Converting data from weather API
│   ├── core/
│   │   ├── airline_repository.cpp      # Converting data from airlines.json
│   │   ├── item_repository.cpp         # Creating items data from items.json
│   │   └── luggage_packer.cpp          # Running algorithm to succesfully place all the items in designated luggages
│   ├── ui/
│   │   └── ui.cpp                    # Ui used to generate Deskop view of the app
│   └── main.cpp
├── CMakeLists.txt            # Project Configuration
├── .gitignore                
└── README.md                 # Read me
```

## Changelog

### v 0.2.0 (2026-05-31)
- Prototype version
- Implemented smart packing algorithm.
- Created first graphical interface with Dear ImGui.
- Added airline baggage constraints support.
- Added activity-based packing recommendations.

### v 0.1.0 (2026-05-30)
- Prototype version.
- Basic luggage optimization implemented.
- Item Data implemented.
- Weather Data fetching implemented.
