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
            itemData["additionalCapabilities"] = person->getAdditionalCapabilities();
            itemData["numInDeck"] = person->getNumInDeck();

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

        for (const auto& person : jsonData) {
            std::string name = person["name"];
            int teamNumber = person["teamNumber"];
            int side = person["side"];
            int maxActions = person["maxActions"];
            int HP = person["HP"];
            Pawn::SpaceInventory space;
            space.hands = person["space"]["hands"];
            space.extras = person["space"]["extras"];
            int price = person["price"];
            std::string additionalCapabilities = person["additionalCapabilities"];
            int numInDeck = person["numInDeck"];

            Pawn* pawn = new Pawn(name, teamNumber, side, maxActions, HP, space, price, 
                additionalCapabilities, numInDeck);
            pawnList.push_back(pawn);
        }
    }
    else {
        std::string fullPath = filename + ".json";
        throw std::runtime_error("PawnsManager ERROR: Could not open file -> " + fullPath);
    }

    return pawnList;
}
