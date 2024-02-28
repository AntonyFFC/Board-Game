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

            Equipment* equipment = new Equipment(name, range, spaceOccupied, attackValue, attackActions,
                type, price, additionalCapabilities);
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
