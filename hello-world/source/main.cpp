#include <m3ds/M3DS.hpp>

int main() {
    // Initialises systems, sets bottom screen as console (configurable)
    M3DS::Init _ {};

    // Supports formatting through std::format
    M3DS::Debug::log("Hello, {}!", "World");

    // Create a root node and begin main loop (so program doesn't exit immediately)
    // Press START to exit in default mainLoop.
    M3DS::Root root {};
    root.mainLoop();
}
