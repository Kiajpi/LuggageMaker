# LuggageMaker

Aplikacja wspomagająca optymalne pakowanie bagażu w zależności od taryfy lotniczej i pogody.

## Funkcje
- Automatyczne pobieranie pogody (Open-Meteo API).
- Algorytm optymalizacji bagażu pod limity wagowe.
- Wizualizacja zapełnienia bagażu w czasie rzeczywistym.

## Użycie
Po uruchomieniu aplikacji w oknie głównym należy wypełnić formularz:
1. Podaj cel podróży.
2. Wybierz linię lotniczą i taryfę (bagaż).
3. Zaznacz preferowane aktywności.
4. Kliknij "Generuj" - aplikacja wyświetli listę ubrań i grafikę walizek.

## Wymagania
- Biblioteki: Dear ImGui, cpr, nlohmann-json.
