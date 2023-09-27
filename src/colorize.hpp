#ifndef BEAUTIFIER_COLORIZE_HPP

#include <iostream>
#include <cinttypes>
#include <algorithm>
#include <set>

namespace Colorizer {
    struct RGB_CONTAINER {
    public:
        const int32_t HEX;
        uint16_t R{256};
        uint16_t G{256};
        uint16_t B{256};

        explicit RGB_CONTAINER(int32_t hexValueArg)
                : HEX(hexValueArg),
                  R(hexValueArg >= 0 ? (((hexValueArg >> 16) & 0xFF)) : 256),
                  G(hexValueArg >= 0 ? (((hexValueArg >> 8) & 0xFF)) : 256),
                  B(hexValueArg >= 0 ? (((hexValueArg) & 0xFF)) : 256) {}
    };

    namespace _internals {
        /*
         Just does a simple static_cast on a given enumerator by TResult

        Could Also be written as:
            `template <typename E, typename = std::enable_if_t<std::is_enum<E>::value>>`
        */
        template<typename TResult, typename TInput>
        requires std::is_enum<TInput>::value
        inline constexpr TResult EnumValue(TInput enumArg) {
            return static_cast<TResult>(enumArg);
        }
    }


    // Referenced form https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
    enum colorState : uint16_t {
        colorInitialized = 1,
        colorUninitialized = 0,
    };

    enum colorScheme : uint16_t {
        Background = 48,
        Foreground = 38
    };

    enum colorMode : uint16_t {
        BIT_4 = 4,
        BIT_8 = 5,
        BIT_24 = 2
    };

    enum emphasis : uint16_t {
        BOLD = 1,
        FAINT = 2,
        ITALIC = 3,
        UNDERLINE = 4,
        BLINK_SLOW = 5,
        BLINK_RAPID = 6,
        REVERSE = 7,
        CONCEAL_OR_HIDE = 8,
        CROSSEDOUT_OR_STRIKETHROUGH = 9,
        FRAKTUR_GOTHIC = 20,
        DOUBLE_UNDERLINE_OR_NOT_BOLD = 21
    };


    enum BIT_4_COLORS : uint16_t {
        COLOR_NONE = 999,
        // 3BIT FOREGROUND COLORS
        FOREGROUND_Black = 30,
        FOREGROUND_Red = 31,
        FOREGROUND_Green = 32,
        FOREGROUND_Yellow = 33,
        FOREGROUND_Blue = 34,
        FOREGROUND_Magenta = 35,
        FOREGROUND_Cyan = 36,
        FOREGROUND_White = 37,

        // 3BIT BACKGROUND COLORS
        BACKGROUND_Black = 40,
        BACKGROUND_Red = 41,
        BACKGROUND_Green = 42,
        BACKGROUND_Yellow = 43,
        BACKGROUND_Blue = 44,
        BACKGROUND_Magenta = 45,
        BACKGROUND_Cyan = 46,
        BACKGROUND_White = 47,

        // All 3BIT Colors are till here,
        // the rest is 4BIT

        // 3BIT FOREGROUND COLORS
        FOREGROUND_Gray = 90,
        FOREGROUND_LIGHT_Red = 91,
        FOREGROUND_LIGHT_Green = 92,
        FOREGROUND_LIGHT_Yellow = 93,
        FOREGROUND_LIGHT_Blue = 94,
        FOREGROUND_LIGHT_Magenta = 95,
        FOREGROUND_LIGHT_Cyan = 96,
        FOREGROUND_LIGHT_White = 97,

        // 3BIT BACKGROUND COLORS
        BACKGROUND_Gray = 100,
        BACKGROUND_LIGHT_Red = 101,
        BACKGROUND_LIGHT_Green = 102,
        BACKGROUND_LIGHT_Yellow = 103,
        BACKGROUND_LIGHT_Blue = 104,
        BACKGROUND_LIGHT_Magenta = 105,
        BACKGROUND_LIGHT_Cyan = 106,
        BACKGROUND_LIGHT_White = 107,
    };


    const static char ANSI_DELIMITER{';'};
    const static char ANSI_CURSOR_MANIPULATOR_DELIMITER{'?'};
    const static char ANSI_ESCAPE_SEQUENCE{'\033'};
    const static std::basic_string<char>
            ANSI_ESCAPE_SEQUENCE_STRING{
            Colorizer::ANSI_ESCAPE_SEQUENCE
            + std::basic_string<char>("[")};
    const static uint16_t ANSI_MAX_COLOR_SIZE{255};
    const static std::basic_string<char>
            ANSI_RESET{
            Colorizer::ANSI_ESCAPE_SEQUENCE
            + std::basic_string<char>("[0m")};

    const static std::basic_string<char>
            ANSI_CURSOR_MANIPULATOR_HIDE{
            Colorizer::ANSI_ESCAPE_SEQUENCE
            + std::basic_string<char>("[")
            + Colorizer::ANSI_CURSOR_MANIPULATOR_DELIMITER
            + "25l"};

    const static std::basic_string<char>
            ANSI_CURSOR_MANIPULATOR_SHOW{
            Colorizer::ANSI_ESCAPE_SEQUENCE
            + std::basic_string<char>("[")
            + Colorizer::ANSI_CURSOR_MANIPULATOR_DELIMITER
            + "25h"};

    namespace Manipulators {
        inline constexpr std::basic_string<char> RGBToANSI(const RGB_CONTAINER &hexColorArg) {
            return std::to_string(hexColorArg.R)
                   + ';' + std::to_string(hexColorArg.G)
                   + ';' + std::to_string(hexColorArg.B);
        }

        inline void hidecursor(void) {
            std::cout << Colorizer::ANSI_CURSOR_MANIPULATOR_HIDE << std::flush;
        }

        inline void showcursor(void) {
            std::cout << Colorizer::ANSI_CURSOR_MANIPULATOR_SHOW << std::flush;
        }
    }
}

class Colorize {
private:
    /**
     * Validates a ColorCode
     \Takes
            ColorCode, in 8 bit mode 2 ^ 8 --> 0-255

     \Returns
            a ColorState which determines the state of the given ColorCode

    */
    template<Colorizer::colorMode colorModeArg, typename T>
    static constexpr bool isValidColor(T colorCodeArg) {
        switch (colorModeArg) {
            case Colorizer::BIT_4:
                return (
                        (colorCodeArg >= 30 && colorCodeArg <= 37 || colorCodeArg >= 90 && colorCodeArg <= 97)
                        ||
                        (colorCodeArg >= 40 && colorCodeArg <= 47 || colorCodeArg >= 100 && colorCodeArg <= 107)
                );
            case Colorizer::BIT_8:
                return colorCodeArg >= 0 && colorCodeArg <= Colorizer::ANSI_MAX_COLOR_SIZE;
            case Colorizer::BIT_24: {
                auto colorCodeHolder = (Colorizer::RGB_CONTAINER) colorCodeArg;
                return (colorCodeHolder.R >= 0 && colorCodeHolder.R <= 255) &&
                       (colorCodeHolder.G >= 0 && colorCodeHolder.G <= 255) &&
                       (colorCodeHolder.B >= 0 && colorCodeHolder.B <= 255);
            }
            default:
                return false;
        }
    }

    /**
     * Gives the start EscapeSequence for a given ColorScheme
     \Takes
            ColorSchemes: Background, Foreground

     \Returns
            A string containing the start EscapeSequence for a given ColorScheme && ColorMode

    */
    template<Colorizer::colorMode colorModeArg>
    static constexpr std::basic_string<char>
    colorSchemeStyle(const int32_t &foregroundColorArg = -1,
                     const int32_t &backgroundColorArg = -1,
                     const std::set<Colorizer::emphasis> &stylesArg = {}) {
        bool backgroundColorState = Colorize::isValidColor<colorModeArg>(backgroundColorArg);
        bool foregroundColorState = Colorize::isValidColor<colorModeArg>(foregroundColorArg);

        std::basic_string<char> backgroundStartTemp{
                backgroundColorState && colorModeArg != Colorizer::BIT_4 ? Colorizer::ANSI_ESCAPE_SEQUENCE_STRING
                                                                         : ""};
        std::basic_string<char> foregroundStartTemp{
                foregroundColorState || !stylesArg.empty() ? Colorizer::ANSI_ESCAPE_SEQUENCE_STRING : ""};

        if (colorModeArg == Colorizer::colorMode::BIT_4) {
            if (backgroundColorState) { foregroundStartTemp += std::to_string(backgroundColorArg) + ';'; }
            if (foregroundColorState) { foregroundStartTemp += std::to_string(foregroundColorArg) + ';'; }

        } else {
            if (backgroundColorState) {
                backgroundStartTemp += std::to_string(Colorizer::colorScheme::Background)
                                       + Colorizer::ANSI_DELIMITER
                                       + std::to_string(colorModeArg)
                                       + Colorizer::ANSI_DELIMITER
                                       + (colorModeArg == Colorizer::colorMode::BIT_8
                                          ? std::to_string(backgroundColorArg)
                                          : Colorizer::Manipulators::RGBToANSI(
                                Colorizer::RGB_CONTAINER(backgroundColorArg)))
                                       + 'm';
            }
            if (foregroundColorState) {
                foregroundStartTemp += std::to_string(Colorizer::colorScheme::Foreground)
                                       + Colorizer::ANSI_DELIMITER
                                       + std::to_string(colorModeArg)
                                       + Colorizer::ANSI_DELIMITER
                                       + (colorModeArg == Colorizer::colorMode::BIT_8
                                          ? std::to_string(foregroundColorArg)
                                          : Colorizer::Manipulators::RGBToANSI(
                                Colorizer::RGB_CONTAINER(foregroundColorArg)))
                                       + Colorizer::ANSI_DELIMITER;
            }
        }

        for (const Colorizer::emphasis &style: stylesArg) {
            foregroundStartTemp
                    += std::to_string(Colorizer::_internals::EnumValue<uint16_t>(style)) + ';';
        }

        if (!foregroundStartTemp.empty()) {
            foregroundStartTemp.pop_back();
            foregroundStartTemp += 'm';
        }
        return backgroundStartTemp + foregroundStartTemp;
    }

public:
    // 4-BIT - [Used by Value]
    template<int16_t foregroundArg = -1, int16_t backgroundArg = -1>
    static std::basic_string<char>
    rich4V(const std::basic_string<char> &rawMessageArg,
           const std::set<Colorizer::emphasis> &stylesArg = {}) {
        return colorSchemeStyle<Colorizer::colorMode::BIT_4>
                       (foregroundArg,
                        backgroundArg,
                        stylesArg)
               + rawMessageArg + Colorizer::ANSI_RESET;
    }

    // 4-BIT - [Used by Reference]
    static std::basic_string<char>
    rich4R(const std::basic_string<char> &rawMessageArg,
           const int16_t &foregroundArg = -1,
           const int16_t &backgroundArg = -1,
           const std::set<Colorizer::emphasis> &stylesArg = {}) {
        return colorSchemeStyle<Colorizer::colorMode::BIT_4>
                       (foregroundArg,
                        backgroundArg,
                        stylesArg)
               + rawMessageArg + Colorizer::ANSI_RESET;
    }


    // 8-BIT - [Used by Value]
    template<int16_t foregroundArg = -1, int16_t backgroundArg = -1>
    static std::basic_string<char>
    rich8V(const std::basic_string<char> &rawMessageArg,
           const std::set<Colorizer::emphasis> &stylesArg = {}) {
        return colorSchemeStyle<Colorizer::colorMode::BIT_8>
                       (foregroundArg,
                        backgroundArg,
                        stylesArg)
               + rawMessageArg + Colorizer::ANSI_RESET;
    }

    // 8-BIT - [Used by Reference]
    static std::basic_string<char>
    rich8R(const std::basic_string<char> &rawMessageArg,
           const int16_t &foregroundArg = -1,
           const int16_t &backgroundArg = -1,
           const std::set<Colorizer::emphasis> &stylesArg = {}) {
        return colorSchemeStyle<Colorizer::colorMode::BIT_8>
                       (foregroundArg,
                        backgroundArg,
                        stylesArg)
               + rawMessageArg + Colorizer::ANSI_RESET;
    }

    // 24-BIT (TRUE-COLOR) - [Used by Value]
    template<int32_t foregroundArg = -1, int32_t backgroundArg = -1>
    static std::basic_string<char>
    rich24V(const std::basic_string<char> &rawMessageArg,
            const std::set<Colorizer::emphasis> &stylesArg = {}) {
        return colorSchemeStyle<Colorizer::colorMode::BIT_24>
                       (foregroundArg,
                        backgroundArg,
                        stylesArg)
               + rawMessageArg + Colorizer::ANSI_RESET;
    }

    // 24-BIT (TRUE-COLOR) - [Used by Reference]
    static std::basic_string<char>
    rich24R(const std::basic_string<char> &rawMessageArg,
            const int32_t &foregroundArg = -1,
            const int32_t &backgroundArg = -1,
            const std::set<Colorizer::emphasis> &stylesArg = {}) {
        return colorSchemeStyle<Colorizer::colorMode::BIT_24>
                       (foregroundArg,
                        backgroundArg,
                        stylesArg)
               + rawMessageArg + Colorizer::ANSI_RESET;
    }
};

// blocks for test ████████████████████████████
#define BEAUTIFIER_COLORIZE_HPP
#endif //BEAUTIFIER_COLORIZE_HPP
