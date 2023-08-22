
#include "GUI.h"
#include "Equipment.h"
#include "EquipmentManager.h"
#include <iostream>
#include "Menu.h"

int main()
{
    /*std::vector<Equipment*> equipmentList;
    Equipment* e1 = new Equipment("helmet", { 0,0 }, {1, "extras"}, 3, 0, "Armour", 2, "Helmet");
    equipmentList.push_back(e1);
    Equipment* e2 = new Equipment("great helmet", { 0,0 }, { 1, "extras" }, 4, 1, "Armour", 1, "Helmet");
    equipmentList.push_back(e2);
    Equipment* e3 = new Equipment("light shield", { 0,0 }, { 1, "hands" }, 5, 1, "Armour", 3,
        "Shield -2 movement 0 with knight");
    equipmentList.push_back(e3);
    Equipment* e4 = new Equipment("shield", { 0,0 }, { 1, "hands" }, 5, 2, "Armour", 2,
        "Shield -2 movement, -1 with knight");
    equipmentList.push_back(e4);
    Equipment* e5 = new Equipment("heavy shield", { 0,0 }, { 1, "hands" }, 6, 2, "Armour", 2, "Shield");
    equipmentList.push_back(e5);
    Equipment* e6 = new Equipment("leather armour", { 0,0 }, { 1, "extras" }, 2, 1, "Armour", 1, "Covering");
    equipmentList.push_back(e6);
    Equipment* e7 = new Equipment("plate armour", { 0,0 }, { 1, "extras" }, 3, 1, "Armour", 2, "Covering");
    equipmentList.push_back(e7);
    Equipment* e8 = new Equipment("flail", { 0,1 }, { 1, "hands" }, 1, 2, "Weapon", 3, "-1 attack, -2 with shield");
    equipmentList.push_back(e8);
    Equipment* e9 = new Equipment("bracers", { 0,0 }, { 1, "extras" }, 0, 0, "Accesory", 1, "misses less by 1");
    equipmentList.push_back(e9);
    Equipment* e10 = new Equipment("war horse", { 0,0 }, { 1, "extras" }, 0, 2, "Accesory", 3, "adds 2 movement");
    equipmentList.push_back(e10);
    Equipment* e11 = new Equipment("horse", { 0,0 }, { 1, "extras" }, 0, 1, "Accesory", 1, "adds 1 movement");
    equipmentList.push_back(e11);
    Equipment* e12 = new Equipment("crossbow", { 4,6 }, { 1, "hands" }, 2, 3, "Weapon", 4, "Ranged 2");
    equipmentList.push_back(e12);
    Equipment* e13 = new Equipment("heavy crossbow", { 4,6 }, { 1, "hands" }, 2, 4, "Weapon", 2, "Ranged 2");
    equipmentList.push_back(e13);
    Equipment* e14 = new Equipment("bow", { 4,7 }, { 2, "hands" }, 1, 2, "Weapon", 4, "Ranged 2");
    equipmentList.push_back(e14);
    Equipment* e15 = new Equipment("longbow", { 5,7 }, { 2, "hands" }, 1, 2, "Weapon", 2, "Ranged 2");
    equipmentList.push_back(e15);
    Equipment* e16 = new Equipment("sword", { 0,1 }, { 1, "hands" }, 1, 1, "Weapon", 5, "");
    equipmentList.push_back(e16);
    Equipment* e17 = new Equipment("two handed sword", { 0,2 }, { 2, "hands" }, 1, 1, "Weapon", 4, "");
    equipmentList.push_back(e17);
    Equipment* e18 = new Equipment("two handed axe", { 0,2 }, { 2, "hands" }, 3, 4, "Weapon", 2,
        "3 attack, 4 with townsman");
    equipmentList.push_back(e18);
    Equipment* e19 = new Equipment("warhammer", { 0,1 }, { 1, "hands" }, 2, 4, "Weapon", 2,
        "destroys walls using 1 action, 1 range, 2 with townsman");
    equipmentList.push_back(e19);
    Equipment* e20 = new Equipment("large slingshot", { 2,4 }, { 2, "hands" }, 2, 3, "Weapon", 2,
        "Ranged 3 ,-3 actions, -2 with peasant");
    equipmentList.push_back(e20);
    Equipment* e21 = new Equipment("slingshot", { 2,4 }, { 2, "hands" }, 1, 3, "Weapon", 1,
        "Ranged 3 ,1 attack, 2 with peasant");
    equipmentList.push_back(e21);
    Equipment* e22 = new Equipment("gauntlets", { 0,0 }, { 1, "extras" }, 1, 0, "Accesory", 3, "+1 to every attack");
    equipmentList.push_back(e22);
    Equipment* e23 = new Equipment("long dagger", { 0,1 }, { 1, "extras" }, 5, 1, "Weapon", 2,
        "Single use, 1 range, 2 with prince");
    equipmentList.push_back(e23);
    Equipment* e24 = new Equipment("dagger", { 0,1 }, { 1, "extras" }, 4, 1, "Weapon", 2,
        "Single use, 4 attack, 5 with prince");
    equipmentList.push_back(e24);
    Equipment* e25 = new Equipment("spear", { 0,2 }, { 1, "hands" }, 2, 3, "Weapon", 4,
        "2 attack, 3 on a horse");
    equipmentList.push_back(e25);
    Equipment* e26 = new Equipment("axe", { 0,1 }, { 1, "hands" }, 2, 3, "Weapon", 3, "2 attack, 3 with townsman");
    equipmentList.push_back(e26);
    Equipment* e27 = new Equipment("fork", { 0,1 }, { 1, "hands" }, 1, 2, "Weapon", 3, "1 range, 2 with peasant");
    equipmentList.push_back(e27);
    std::string filename = "equipment";
    EquipmentManager::saveEquipmentToJson(equipmentList, filename);
    delete e1;
    delete e2;
    delete e3;
    delete e4;
    delete e5;
    delete e6;
    delete e7;
    delete e8;
    delete e9;
    delete e10;
    delete e11;
    delete e12;
    delete e13;
    delete e14;
    delete e15;
    delete e16;
    delete e17;
    delete e18;
    delete e19;
    delete e20;
    delete e21;
    delete e22;
    delete e23;
    delete e24;
    delete e25;
    delete e26;
    delete e27;*/


    Menu menu1;
    menu1.start();
    return 0;
}
