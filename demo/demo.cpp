#include "colorize.hpp"

int main(void) {
    // blocks for test ████████████████████████████
    Beautifier::Manipulators::HideCursor();

    for (int16_t colorCode{0}; colorCode <= 16; ++colorCode) {
        std::cout << Colorize::rich8R
            ("████████████████ ["
                + std::string(colorCode < 10 ? "0" : "")
                + std::to_string(colorCode)
                + "] ████████████████\n"
            , colorCode, -1)
        << std::flush;
    }
    std::cout << std::endl;


    std::string description{"A library for colorizing texts in terminal!"};
    for (int16_t colorCode{0}, charPtr{0}; charPtr <= description.length(); ++colorCode, ++charPtr) {
        std::cout << Colorize::rich8R(description.substr(charPtr, 1), colorCode+=4) << std::flush;
    }
    std::cout << std::endl << std::endl;

    std::cout << Colorize::rich8V("BOLD",  {Beautifier::emphasis::BOLD}) << std::endl;
    std::cout << Colorize::rich8V("FAINT",  {Beautifier::emphasis::FAINT}) << std::endl;
    std::cout << Colorize::rich8V("ITALIC",  {Beautifier::emphasis::ITALIC}) << std::endl;
    std::cout << Colorize::rich8V("UNDERLINE",  {Beautifier::emphasis::UNDERLINE}) << std::endl;
    std::cout << Colorize::rich8V("SLOW BLINK",  {Beautifier::emphasis::BLINK_SLOW}) << std::endl;
    std::cout << Colorize::rich8V("RAPID BLINK",  {Beautifier::emphasis::BLINK_RAPID}) << std::endl;
    std::cout << Colorize::rich8V("REVERSE",  {Beautifier::emphasis::REVERSE}) << std::endl;
    std::cout << Colorize::rich8V("CONCEAL OR HIDE",  {Beautifier::emphasis::CONCEAL_OR_HIDE}) << " <- (CONCEAL OR HIDE)" << std::endl;
    std::cout << Colorize::rich8V("CROSSEDOUT OR STRIKETHROUGH",  {Beautifier::emphasis::CROSSEDOUT_OR_STRIKETHROUGH}) << std::endl;
    std::cout << Colorize::rich8V("DOUBLE UNDERLINE OR NOT BOLD",  {Beautifier::emphasis::DOUBLE_UNDERLINE_OR_NOT_BOLD}) << std::endl;

    std::cout << std::endl;

    std::string titleBar2{"------------------------------------------------"};
    for (int16_t colorCode{0}, charPtr{0}; charPtr <= titleBar2.length(); ++colorCode, ++charPtr) {
        std::cout << Colorize::rich8R(titleBar2.substr(charPtr, 1), colorCode+=5, -1, {Beautifier::emphasis::BOLD}) << std::flush;
    }
    std::cout << std::endl << std::endl;

    // 8 & 24 BIT Coloring
    std::cout << Colorize::rich24V<0xeb106f, 0x263640>("Hello World", {Beautifier::emphasis::BOLD, Beautifier::emphasis::UNDERLINE}) << std::endl;
    std::cout << Colorize::rich8V<199, 253>("Hello World") << std::endl;

    std::cout << std::endl;

    // 4-BIT Coloring
    std::cout << Colorize::rich4V<Beautifier::BIT_4_COLORS::COLOR_NONE, Beautifier::BIT_4_COLORS::BACKGROUND_Magenta>("Hello World", {Beautifier::emphasis::BOLD}) << std::endl;
    std::cout << Colorize::rich4V<Beautifier::BIT_4_COLORS::FOREGROUND_LIGHT_Blue, Beautifier::BIT_4_COLORS::COLOR_NONE>("Hello World", {Beautifier::emphasis::BOLD}) << std::endl;
    std::cout << Colorize::rich4V<Beautifier::BIT_4_COLORS::FOREGROUND_LIGHT_Blue, Beautifier::BIT_4_COLORS::BACKGROUND_Magenta>("Hello World", {Beautifier::emphasis::BOLD, Beautifier::emphasis::BOLD}) << std::endl;
    std::cout << Colorize::rich4V<Beautifier::BIT_4_COLORS::FOREGROUND_Black, Beautifier::BIT_4_COLORS::BACKGROUND_Cyan>("Hello World") << std::endl;

    Beautifier::Manipulators::ShowCursor();
    return 0;
}
