
#include "GUI.h"
#include "Equipment.h"
#include "EquipmentManager.h"
#include <iostream>

int main()
{
    /*std::vector<Equipment> equipmentList;
    Equipment e1("Sword", 1, {1, "hands"}, 1, 1, "Weapon", 5, "");
    equipmentList.push_back(e1);
    Equipment e2("Flail", 1, { 1, "hands" }, 1, 2, "Weapon", 3, "2 attack with shield");
    equipmentList.push_back(e2);
    Equipment e3("Fork", 1, { 1, "hands" }, 1, 2, "Weapon", 3, "2 range with peasant");
    equipmentList.push_back(e3);
    std::string filename = "equipment.json";
    EquipmentManager::saveEquipmentToJson(equipmentList, filename);*/

    /*std::string filename = "equipment.json";
    std::vector<Equipment> loadedEquipment = EquipmentManager::loadEquipmentFromJson(filename);
    for (auto& data : loadedEquipment)
    {
        std::cout << data.getName() << data.getPrice() << "\n";
    }*/

    Gui interface1;
    interface1.start();
    return 0;
}
