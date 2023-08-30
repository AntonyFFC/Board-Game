#include "PawnsManager.h"

bool PawnsManager::savePawnsToJson(const std::vector<Pawn*>& equipment, const std::string& filename)
{
    std::ofstream outputFile(filename + ".json");
    if (outputFile.is_open()) {
        nlohmann::json equipmentData;
        for (auto& person : equipment)
        {
            nlohmann::json itemData;
            itemData["name"] = person->getName();
            itemData["teamNumber"] = person->getTeamNumber();
            itemData["side"] = person->getSide();
            itemData["maxActions"] = person->getMaxActions();
            itemData["HP"] = person->getHP(); 
            nlohmann::json space;
            space["hands"] = person->getSpace().hands;
            space["extras"] = person->getSpace().extras;
            itemData["space"] = space;
            itemData["price"] = person->getPrice();
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

std::vector<Pawn*> PawnsManager::loadPawnsFromJson(const std::string& filename)
{
    // remember to delete the pointers to the objects in the returned vector
    std::vector<Pawn*> pawnList;

    std::ifstream inputFile(filename + ".json");
    if (inputFile.is_open()) {
        nlohmann::json jsonData;
        inputFile >> jsonData;
        inputFile.close();

        for (const auto& item : jsonData) {
            std::string name = item["name"];
            int teamNumber = item["teamNumber"];
            int side = item["side"];
            int maxActions = item["maxActions"];
            int HP = item["HP"];
            Pawn::SpaceInventory space;
            space.hands = item["space"]["hands"];
            space.extras = item["space"]["extras"];
            int price = item["price"];

            Pawn* pawn = new Pawn(name, teamNumber, side, maxActions, HP, space, price);
            pawnList.push_back(pawn);
        }
    }
    else {
        // Handle error
    }

    return pawnList;
}
