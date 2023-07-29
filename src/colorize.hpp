#ifndef BEAUTIFIER_COLORIZE_HPP

#include <iostream>
#include <cinttypes>
#include <algorithm>
#include <list>

namespace Beautifier {
    const static uint16_t ANSI_MAX_HEX_COLOR_LENGTH{6};

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
        inline constexpr std::basic_string<char> ExtractHexColorFromString(const std::basic_string<char> &hexColorArg) {
            std::basic_string<char> hexColorTemp{hexColorArg.starts_with('#') ? hexColorArg.substr(1) : hexColorArg};
            return hexColorTemp;
        }

        namespace Validators {
            /**
             * Validates a HexColor
             \Takes
                    HexColor : int32 , 0x00-00-00 -> 0xFF-FF-FF
                    <br>
                    (last 2 bytes are excluded since we dont have them in terminal coloring scheme)

             \Returns
                    a boolean which determines the validity of the given HexColor

            */
            bool isValidHexColor(const int32_t &hexColorArg) {
                // Check if each color component (R, G, B) is within the valid range (0 to 255)
                int red = (hexColorArg >> 16) & 0xFF;
                int green = (hexColorArg >> 8) & 0xFF;
                int blue = hexColorArg & 0xFF;

                return (red >= 0 && red <= 255) &&
                       (green >= 0 && green <= 255) &&
                       (blue >= 0 && blue <= 255);
            }

            bool isValidHexColor(const RGB_CONTAINER &hexColorArg) {
                // Check if each color component (R, G, B) is within the valid range (0 to 255)
                return (hexColorArg.R >= 0 && hexColorArg.R <= 255) &&
                       (hexColorArg.G >= 0 && hexColorArg.G <= 255) &&
                       (hexColorArg.B >= 0 && hexColorArg.B <= 255);
            }

            bool isValidHexColorString(const std::basic_string<char> &hexColorArg) {
                /* Hex color format: RR-GG-BB-AA which gives the length of 8 bytes at most
                if (hexColorArg.starts_with('#')) {
                    if (hexColorArg.length() > 9) {
                        return false;
                    } else {
                        hexColorTemp = hexColorTemp.substr(1);
                    }
                } else {
                    if (hexColorArg.length() > 8) {
                        return false;
                    }
                }
                */

                // Hex color format: RR-GG-BB which gives the length of 6 bytes at most
                if (hexColorArg.length() > Beautifier::ANSI_MAX_HEX_COLOR_LENGTH) {
                    return false;
                }

                // Check if all the characters are hex
                return std::all_of(hexColorArg.begin(), hexColorArg.end(), ::isxdigit);
            }
        }

        template<typename E>
        inline constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
            return static_cast<typename std::underlying_type<E>::type>(e);
        }

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

    enum class emphasis : uint16_t {
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


    const static char ANSI_DELIMITER{';'};
    const static char ANSI_CURSOR_MANIPULATOR_DELIMITER{'?'};
    const static char ANSI_ESCAPE_SEQUENCE{'\033'};
    const static std::basic_string<char> ANSI_ESCAPE_SEQUENCE_STRING{
            Beautifier::ANSI_ESCAPE_SEQUENCE + std::basic_string<char>("[")};
    const static uint16_t ANSI_MAX_COLOR_SIZE{255};
    const static std::basic_string<char> ANSI_RESET{
            Beautifier::ANSI_ESCAPE_SEQUENCE + std::basic_string<char>("[0m")};

    const static std::basic_string<char> ANSI_CURSOR_MANIPULATOR_HIDE{
            Beautifier::ANSI_ESCAPE_SEQUENCE + std::basic_string<char>("[")
            + Beautifier::ANSI_CURSOR_MANIPULATOR_DELIMITER
            + "25l"};

    const static std::basic_string<char> ANSI_CURSOR_MANIPULATOR_SHOW{
            Beautifier::ANSI_ESCAPE_SEQUENCE + std::basic_string<char>("[")
            + Beautifier::ANSI_CURSOR_MANIPULATOR_DELIMITER
            + "25h"};

    namespace Manipulators {
        inline constexpr std::basic_string<char> RGBToANSI(const RGB_CONTAINER &hexColorArg) {
            return std::to_string(hexColorArg.R) + ';' + std::to_string(hexColorArg.G) + ';' +
                   std::to_string(hexColorArg.B);
        }

        inline void HideCursor(void) {
            std::cout << Beautifier::ANSI_CURSOR_MANIPULATOR_HIDE << std::flush;
        }

        inline void ShowCursor(void) {
            std::cout << Beautifier::ANSI_CURSOR_MANIPULATOR_SHOW << std::flush;
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
    static inline constexpr bool isValidColor(const int32_t colorCodeArg) {
        if (colorCodeArg >= 0 && colorCodeArg <= Beautifier::ANSI_MAX_COLOR_SIZE) {
            return true;
        } else {
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
    template<Beautifier::colorMode colorModeArg>
    static constexpr std::basic_string<char>
    colorSchemeStyle(const Beautifier::colorScheme &colorSchemeArg, const int32_t &colorCodeArg,
                     const std::list<Beautifier::emphasis> &stylesArg = {}) {
        std::basic_string<char> colorSchemeStyleTemp{Beautifier::ANSI_ESCAPE_SEQUENCE_STRING};

        switch (colorModeArg) {
            case Beautifier::colorMode::BIT_4:
                return colorSchemeStyleTemp;
            case Beautifier::colorMode::BIT_8: {
                bool colorCodeState = Colorize::isValidColor(colorCodeArg);
                if (colorCodeState) {
                    colorSchemeStyleTemp += std::to_string(colorSchemeArg)
                                            + Beautifier::ANSI_DELIMITER
                                            + std::to_string(colorModeArg)
                                            + Beautifier::ANSI_DELIMITER
                                            + std::to_string(colorCodeArg);

                    if (colorSchemeArg == Beautifier::Foreground) {
                        colorSchemeStyleTemp += ';';
                    }
                } else {
                    if (colorSchemeArg == Beautifier::Background || stylesArg.empty()) {
                        return "";
                    }
                }

                if (!stylesArg.empty()) {
                    for (const Beautifier::emphasis &style: stylesArg) {
                        colorSchemeStyleTemp +=
                                std::to_string(Beautifier::_internals::EnumValue<uint16_t>(style)) + ';';
                    }
                }

                if (!colorSchemeStyleTemp.empty() && colorSchemeArg == Beautifier::Foreground) {
                    colorSchemeStyleTemp.pop_back();
                }

                colorSchemeStyleTemp += 'm';

                return colorSchemeStyleTemp;
            }
            case Beautifier::colorMode::BIT_24: {
                Beautifier::RGB_CONTAINER hexColorCodes(colorCodeArg);
                bool hexColorCodeState = Beautifier::_internals::Validators::isValidHexColor(hexColorCodes);

                if (hexColorCodeState) {
                    colorSchemeStyleTemp += std::to_string(colorSchemeArg)
                                            + Beautifier::ANSI_DELIMITER
                                            + std::to_string(colorModeArg)
                                            + Beautifier::ANSI_DELIMITER
                                            + Beautifier::Manipulators::RGBToANSI(hexColorCodes);
                    if (colorSchemeArg == Beautifier::Foreground) {
                        colorSchemeStyleTemp += ';';
                    }
                } else {
                    if (colorSchemeArg == Beautifier::Background || stylesArg.empty()) {
                        return "";
                    }
                }

                for (const Beautifier::emphasis &style: stylesArg) {
                    colorSchemeStyleTemp += std::to_string(Beautifier::_internals::EnumValue<uint16_t>(style)) + ';';
                }

                if (!colorSchemeStyleTemp.empty() && colorSchemeArg == Beautifier::Foreground) {
                    colorSchemeStyleTemp.pop_back();
                }

                colorSchemeStyleTemp += 'm';

                return colorSchemeStyleTemp;
            }
            default:
                return colorSchemeStyleTemp;
        }
    }

public:
    // 8-BIT
    template<int16_t foregroundArg = 256, int16_t backgroundArg = 256>
    static std::basic_string<char>
    rich(const std::basic_string<char> &rawMessageArg, const std::list<Beautifier::emphasis> &stylesArg = {}) {
        std::basic_string<char> backgroundStart{
                colorSchemeStyle<Beautifier::colorMode::BIT_8>(
                        Beautifier::colorScheme::Background,
                        backgroundArg)};
        std::basic_string<char> foregroundStart{
                colorSchemeStyle<Beautifier::colorMode::BIT_8>(
                        Beautifier::colorScheme::Foreground,
                        foregroundArg,
                        stylesArg)};

        std::basic_string<char> coloredTextStart = backgroundStart + foregroundStart;;

        return coloredTextStart + rawMessageArg + Beautifier::ANSI_RESET;
    }

    // 8-BIT
    static std::basic_string<char> rich(const std::basic_string<char> &rawMessageArg,
                                        const int16_t &foregroundArg = 256, const int16_t &backgroundArg = 256,
                                        const std::list<Beautifier::emphasis> &stylesArg = {}) {
        std::basic_string<char> backgroundStart{
                colorSchemeStyle<Beautifier::colorMode::BIT_8>(
                        Beautifier::colorScheme::Background,
                        backgroundArg)};
        std::basic_string<char> foregroundStart{
                colorSchemeStyle<Beautifier::colorMode::BIT_8>(
                        Beautifier::colorScheme::Foreground,
                        foregroundArg,
                        stylesArg)};

        std::basic_string<char> coloredTextStart = backgroundStart + foregroundStart;;

        return coloredTextStart + rawMessageArg + Beautifier::ANSI_RESET;
    }

    // 24-BIT (TRUE-COLOR)
    template<int32_t foregroundArg = -1, int32_t backgroundArg = -1>
    static std::basic_string<char>
    rich24(const std::basic_string<char> &rawMessageArg, const std::list<Beautifier::emphasis> &stylesArg = {}) {
        std::basic_string<char> backgroundStart{
                colorSchemeStyle<Beautifier::colorMode::BIT_24>(
                        Beautifier::colorScheme::Background,
                        backgroundArg)};
        std::basic_string<char> foregroundStart{
                colorSchemeStyle<Beautifier::colorMode::BIT_24>(
                        Beautifier::colorScheme::Foreground,
                        foregroundArg,
                        stylesArg)};

        std::basic_string<char> coloredTextStart;


        coloredTextStart += (backgroundStart + foregroundStart);

        return coloredTextStart + rawMessageArg + Beautifier::ANSI_RESET;
    }


    /**
     * Can be used via referenced color like:
    <br>
    ```CPP
            int x = 0xbf2269; // HexColor
            rich24("Hello World", x, -1);
    ```
     */
    static std::basic_string<char> rich24(const std::basic_string<char> &rawMessageArg,
                                          int32_t foregroundArg = -1, int32_t backgroundArg = -1,
                                          const std::list<Beautifier::emphasis> &stylesArg = {}) {
        std::basic_string<char> backgroundStart{
                colorSchemeStyle<Beautifier::colorMode::BIT_24>(
                        Beautifier::colorScheme::Background,
                        backgroundArg,
                        stylesArg)};
        std::basic_string<char> foregroundStart{
                colorSchemeStyle<Beautifier::colorMode::BIT_24>(
                        Beautifier::colorScheme::Foreground,
                        foregroundArg,
                        stylesArg)};

        std::basic_string<char> coloredTextStart = backgroundStart + foregroundStart;;

        return coloredTextStart + rawMessageArg + Beautifier::ANSI_RESET;
    }
};

// blocks for test ████████████████████████████
#define BEAUTIFIER_COLORIZE_HPP
#endif //BEAUTIFIER_COLORIZE_HPP
