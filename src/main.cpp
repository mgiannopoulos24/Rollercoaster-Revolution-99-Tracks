#include "core/Config.h"
#include "core/Game.h"
#include "core/Random.h"
#include "db/DatabaseManager.h"
#include "game/MenuState.h"
#include "version.h"

#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "Rollercoaster Revolution 99 v" << RR99_VERSION_STRING << std::endl;

    // Seed random
    rr99::Random::seed(42);

    // Initialize config
    rr99::Config::instance().load();

    // Initialize database
    rr99::DatabaseManager::instance().initialize("rr99_save.db");
    rr99::DatabaseManager::instance().createTables();

    // Create and run game
    SDL_Init(SDL_INIT_VIDEO);
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    auto game = std::make_unique<rr99::Game>();
    if (!game->initialize("Rollercoaster Revolution 99", 450, dm.h, false)) {
        std::cerr << "Failed to initialize game." << std::endl;
        return 1;
    }

    game->pushState(std::make_unique<rr99::MenuState>(*game));

    game->run();

    // Cleanup
    rr99::DatabaseManager::instance().close();

    return 0;
}
