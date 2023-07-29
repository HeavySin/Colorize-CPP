#include "colorize.hpp"

int main(void) {
    // blocks for test ████████████████████████████
    Beautifier::Manipulators::HideCursor();

    for (int16_t colorCode{0}; colorCode <= 16; ++colorCode) {
        std::cout << Colorize::rich(
                "████████████████ [" + std::string(colorCode < 10 ? "0" : "")  + std::to_string(colorCode) + "] ████████████████\n"
                , colorCode, -1) << std::flush;
    }
    std::cout << std::endl;


    std::string description{"A library for colorizing texts in terminal!"};
    for (int16_t colorCode{0}, charPtr{0}; charPtr <= description.length(); ++colorCode, ++charPtr) {

        std::cout << Colorize::rich(description.substr(charPtr, 1), colorCode+=4) << std::flush;
    }
    std::cout << std::endl << std::endl;

    std::cout << Colorize::rich("BOLD",  {Beautifier::emphasis::BOLD}) << std::endl;
    std::cout << Colorize::rich("FAINT",  {Beautifier::emphasis::FAINT}) << std::endl;
    std::cout << Colorize::rich("ITALIC",  {Beautifier::emphasis::ITALIC}) << std::endl;
    std::cout << Colorize::rich("UNDERLINE",  {Beautifier::emphasis::UNDERLINE}) << std::endl;
    std::cout << Colorize::rich("SLOW BLINK",  {Beautifier::emphasis::BLINK_SLOW}) << std::endl;
    std::cout << Colorize::rich("RAPID BLINK",  {Beautifier::emphasis::BLINK_RAPID}) << std::endl;
    std::cout << Colorize::rich("REVERSE",  {Beautifier::emphasis::REVERSE}) << std::endl;
    std::cout << Colorize::rich("CONCEAL OR HIDE",  {Beautifier::emphasis::CONCEAL_OR_HIDE}) << " <- (CONCEAL OR HIDE)" << std::endl;
    std::cout << Colorize::rich("CROSSEDOUT OR STRIKETHROUGH",  {Beautifier::emphasis::CROSSEDOUT_OR_STRIKETHROUGH}) << std::endl;
    std::cout << Colorize::rich("DOUBLE UNDERLINE OR NOT BOLD",  {Beautifier::emphasis::DOUBLE_UNDERLINE_OR_NOT_BOLD}) << std::endl;

    std::cout << std::endl;

    std::string titleBar2{"------------------------------------------------"};
    for (int16_t colorCode{0}, charPtr{0}; charPtr <= titleBar2.length(); ++colorCode, ++charPtr) {
        std::cout << Colorize::rich(titleBar2.substr(charPtr, 1), colorCode+=5, -1, {Beautifier::emphasis::BOLD}) << std::flush;
    }
    std::cout << std::endl << std::endl;

    std::cout << Colorize::rich24<0xeb106f, 0x263640>("Hello World", {Beautifier::emphasis::BOLD, Beautifier::emphasis::UNDERLINE}) << std::endl;
    std::cout << std::endl;
    std::cout << Colorize::rich<199, 253>("Hello World", {Beautifier::emphasis::BOLD}) << std::endl;

    Beautifier::Manipulators::ShowCursor();
    return 0;
}
