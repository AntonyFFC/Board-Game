#include "EquipmentManager.h"

bool EquipmentManager::saveEquipmentToJson(const std::vector<Equipment*>& equipment, const std::string& filename)
{
    std::ofstream outputFile(filename+".json");
    if (outputFile.is_open()) {
        nlohmann::json equipmentData;
        for (auto& item : equipment)
        {
            nlohmann::json itemData;
            itemData["name"] = item->getName();
            nlohmann::json range;
                range["minRange"] = item->getRange().minRange;
                range["maxRange"] = item->getRange().maxRange;
            itemData["range"] = range;
            nlohmann::json space;
                space["numSpaces"] = item->getSpaceOccupied().numSpaces;
                space["spaceType"] = item->getSpaceOccupied().spaceType;
            itemData["spaceOccupied"] = space;
            itemData["attackValue"] = item->getAttackValue();
            itemData["attackActions"] = item->getAttackActions();
            itemData["type"] = item->getType();
            itemData["price"] = item->getPrice();
            itemData["additionalCapabilities"] = item->getAdditionalCapabilities();
            itemData["numInDeck"] = item->getNumInDeck();

            nlohmann::json actionBonusJson = nlohmann::json::array();
            for (const auto& bonus : item->getActionBonus()) {
                nlohmann::json bonusJson;
                bonusJson["condition"] = bonus.first;
                bonusJson["bonus"] = bonus.second;
                actionBonusJson.push_back(bonusJson);
            }
            itemData["actionBonus"] = actionBonusJson;

            nlohmann::json attackBonusJson = nlohmann::json::array();
            for (const auto& bonus : item->getAttackBonus()) {
                nlohmann::json bonusJson;
                bonusJson["condition"] = bonus.first;
                bonusJson["bonus"] = bonus.second;
                attackBonusJson.push_back(bonusJson);
            }
            itemData["attackBonus"] = attackBonusJson;

            nlohmann::json rangeBonusJson = nlohmann::json::array();
            for (const auto& bonus : item->getRangeBonus()) {
                nlohmann::json bonusJson;
                bonusJson["condition"] = bonus.first;
                bonusJson["bonus"] = bonus.second;
                rangeBonusJson.push_back(bonusJson);
            }
            itemData["rangeBonus"] = rangeBonusJson;

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

std::vector<Equipment*> EquipmentManager::loadEquipmentFromJson(const std::string& filename) {
    // remember to delete the pointers to the objects in the returned vector
    std::vector<Equipment*> equipmentList;

    std::ifstream inputFile(filename+".json");
    if (inputFile.is_open()) {
        nlohmann::json jsonData;
        inputFile >> jsonData;
        inputFile.close();

        for (const auto& item : jsonData) {
            std::string name = item["name"];
            Equipment::Range range;
                int minRange = item["range"]["minRange"];
                int maxRange = item["range"]["maxRange"];
                range.minRange = minRange;
                range.maxRange = maxRange;
            Equipment::SpaceOccupied spaceOccupied;
                int numSpaces = item["spaceOccupied"]["numSpaces"];
                std::string spaceType = item["spaceOccupied"]["spaceType"];
                spaceOccupied.numSpaces = numSpaces;
                spaceOccupied.spaceType = spaceType;
            int attackValue = item["attackValue"];
            int attackActions = item["attackActions"];
            std::string type = item["type"];
            int price = item["price"];
            std::string additionalCapabilities = item["additionalCapabilities"];
            int numInDeck = item["numInDeck"];

            std::vector<std::pair<std::string, int>> actionBonus = {};
            if (item.contains("actionBonus") && item["actionBonus"].is_array()) {
                for (const auto& cond : item["actionBonus"]) {
                    actionBonus.emplace_back(cond["condition"], cond["bonus"]);
                }
            }
            std::vector<std::pair<std::string, int>> attackBonus = {};
            if (item.contains("attackBonus") && item["attackBonus"].is_array()) {
                for (const auto& cond : item["attackBonus"]) {
                    attackBonus.emplace_back(cond["condition"], cond["bonus"]);
                }
            }
            std::vector<std::pair<std::string, int>> rangeBonus = {};
            if (item.contains("rangeBonus") && item["rangeBonus"].is_array()) {
                for (const auto& cond : item["rangeBonus"]) {
                    rangeBonus.emplace_back(cond["condition"], cond["bonus"]);
                }
            }

            Equipment* equipment = new Equipment(name, range, spaceOccupied, attackValue, attackActions,
                type, price, additionalCapabilities, numInDeck, actionBonus, attackBonus, rangeBonus);
            equipmentList.push_back(equipment);
        }
    }
    else {
        // error
    }

    return equipmentList;
}

std::vector<Equipment*> EquipmentManager::loadWeaponsFromJson(const std::string& filename)
{
    std::vector<Equipment*> weaponsList;
    std::vector<Equipment*> equipmentList = loadEquipmentFromJson(filename);
    for (Equipment* item : equipmentList)
    {
        if (item->getType() == "Weapon")
        {
            weaponsList.push_back(item);
        }
    }
    return weaponsList;
}

std::vector<Equipment*> EquipmentManager::loadArmourFromJson(const std::string& filename)
{
    std::vector<Equipment*> armourList;
    std::vector<Equipment*> equipmentList = loadEquipmentFromJson(filename);
    for (Equipment* item : equipmentList)
    {
        if (item->getType() == "Armour")
        {
            armourList.push_back(item);
        }
    }
    return armourList;
}

std::vector<Equipment*> EquipmentManager::loadAccesoriesFromJson(const std::string& filename)
{
    std::vector<Equipment*> accesoryList;
    std::vector<Equipment*> equipmentList = loadEquipmentFromJson(filename);
    for (Equipment* item : equipmentList)
    {
        if (item->getType() == "Accesory")
        {
            accesoryList.push_back(item);
        }
    }
    return accesoryList;
}
