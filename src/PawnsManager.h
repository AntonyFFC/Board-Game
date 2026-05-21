#pragma once
#include "json.hpp"
#include <fstream>
#include "Pawn.h"
using json = nlohmann::json;

class PawnsManager {
public:
    static bool savePawnsToJson(const std::vector<Pawn*>& equipment, const std::string& filename);
    static std::vector<Pawn*> loadPawnsFromJson(const std::string& filename);
};