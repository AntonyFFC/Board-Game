#pragma once
#include "json.hpp"
#include <fstream>
#include "Equipment.h"
using json = nlohmann::json;

class EquipmentManager {
public:
    static bool saveEquipmentToJson(const std::vector<Equipment>& equipment, const std::string& filename);
    static std::vector<Equipment> loadEquipmentFromJson(const std::string& filename);
};