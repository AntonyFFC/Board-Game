#pragma once

#include "Pawn.h"

#include <vector>

struct GameLaunchData {
    std::vector<Pawn*> pawns[2];
    int walls[2] = { 0, 0 };
};
