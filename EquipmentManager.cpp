#include "EquipmentManager.h"

bool EquipmentManager::saveEquipmentToJson(const std::vector<Equipment>& equipment, const std::string& filename)
{
    

    std::ofstream outputFile("equipment.json");
    if (outputFile.is_open()) {
        nlohmann::json equipmentData;
        for (auto& item : equipment)
        {
            nlohmann::json itemData;
            itemData["name"] = item.getName();
            itemData["range"] = item.getRange();
            nlohmann::json space;
                space["numSpaces"] = item.getSpaceOccupied().numSpaces;
                space["spaceType"] = item.getSpaceOccupied().spaceType;
            itemData["spaceOccupied"] = space;
            itemData["attackValue"] = item.getAttackValue();
            itemData["attackActions"] = item.getAttackActions();
            itemData["type"] = item.getType();
            itemData["price"] = item.getPrice();
            itemData["additionalCapabilities"] = item.getAdditionalCapabilities();

            equipmentData.push_back(itemData);
        }

        outputFile << equipmentData.dump(4);
        outputFile.close();
        return true;
    }
    else {
        return false;
    }
}

std::vector<Equipment> EquipmentManager::loadEquipmentFromJson(const std::string& filename) {
    std::vector<Equipment> equipmentList;

    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        nlohmann::json jsonData;
        inputFile >> jsonData;
        inputFile.close();

        for (const auto& item : jsonData) {
            std::string name = item["name"];
            int range = item["range"];
            int numSpaces = item["spaceOccupied"]["numSpaces"];
            std::string spaceType = item["spaceOccupied"]["spaceType"];
            Equipment::SpaceOccupied spaceOccupied;
            spaceOccupied.numSpaces = numSpaces;
            spaceOccupied.spaceType = spaceType;
            int attackValue = item["attackValue"];
            int attackActions = item["attackActions"];
            std::string type = item["type"];
            int price = item["price"];
            std::string additionalCapabilities = item["additionalCapabilities"];

            Equipment equipment(name, range, spaceOccupied, attackValue, attackActions,
                type, price, additionalCapabilities);
            equipmentList.push_back(equipment);
        }
    }
    else {
        // error
    }

    return equipmentList;
}