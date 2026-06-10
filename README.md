# LuggageMaker

**Version:** 0.4.0  
**Last Updated:** 2026-06-10

LuggageMaker is an intelligent packing assistant designed to optimize luggage weight and content based on airline constraints, destination weather, and specific travel activities.

It uses generative algorithm, API integrations and user input to compute the most optimised way to pack your luggage for your trip.

All of it is wrapped in nice and clean UI with extremely easy to use functionality.

---

## Features

* **Smart Packing Algorithm**: Calculates optimal items based on baggage limits.
* **Weather Integration**: Automatically fetches weather data for the destination (Open-Meteo API) to suggest appropriate clothing.
* **Modern UI**: A responsive dashboard built with Dear ImGui.
* **Activity Support**: Customizable packing lists for different trip types.
* **Travel Tips**: Costomizable travel tips specifically for your destination.

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
2. Enter your travel destination (preferably City, County) and travel dates.
3. Select your airline and baggage allowance.
4. Choose your planned activities (e.g., SPA, Beach, Skiing).
5. Click **Generate**, the button at the end of the page.
6. Review your optimized packing list and suitcase visualization.

---

## AI Disclosure

This project was developed with the assistance of AI tools.

AI was used for:

* Designing Icons in the UI/UX architecture using Dear ImGui.
* Drafting project documentation.
* Generating Databases for Luggage Policies, Items, and Travel Tips. 
* Providing proper UI/UX documentation and helping with fixing bugs.
* Proposing proper project structure to ensure visibility and credibility.

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
│   ├── items.json               # List of items that can be placed in luggages
│   └── tips.json                # List of different tips matching to specific destination via GPS-coordinates.
├── include/                  # .hpp files
│   └── lugagemaker/
│       ├── api/
│       │   └── weather_client.hpp    # Fetching weather data
│       ├── core/
│       │   ├── airline_repository.hpp    # Luggage limits from airlines.json 
│       │   ├── item.hpp                  # Item class
│       │   ├── item_repository.hpp       # Items data from items.json
│       │   ├── luggage_packer.hpp        # Algorithm for optimised packing luggage
│       │   ├── tip_maker.hpp             # Class for tip structure
│       │   └── tips_repository.hpp       # Database of tips     
│       └── ui/
│           └── ui.hpp   # Ui
├── src/                 # .cpp files
│   ├── api/
│   │   └── weather_client.cpp        # Converting data from weather API
│   ├── core/
│   │   ├── airline_repository.cpp      # Converting data from airlines.json
│   │   ├── item_repository.cpp         # Creating items data from items.json
│   │   ├── luggage_packer.cpp          # Running algorithm to succesfully place all the items in designated luggages
│   │   └── tips_repository.cpp         # Parsing json data from tips.json
│   ├── ui/
│   │   └── ui.cpp                    # Ui used to generate Deskop view of the app
│   └── main.cpp              # Main Project
├── CMakeLists.txt            # Project Configuration
├── .gitignore                
└── README.md                 # Read me
```

## Changelog

### v 0.4.0 (2026-06-10)
- Added travel tips database
- Added travel tips field for over 50 popular destinations.
- Fixed geolocalization and parsing of coordinates.
- Fixed the weather client issues.
- Modified the help pop-up content and UI.
- Modified the Item Database

### v 0.3.3 (2026-06-10)
- Modified luggage optimization algorithm for more diverse list of items.
- Updated the Item Database

### v 0.3.2 (2026-06-10)
- Added the help button and help pop-up 
- Small tweaks in UI

### v 0.3.1 (2026-06-10)
- Updated the Item Database
- Changes in UI

### v 0.3.0 (2026-06-10)
- Implemented the gender modifications.
- Changed UI interface to be looking better.
- Modified luggage optimization algorithm.

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
