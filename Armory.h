#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Equipment.h"
#include "EquipmentManager.h"
#include "Globals.h"
class Armory
{
public:
	Armory(sf::RenderWindow* window);
	~Armory();
	void start();
	void exit();
private:
	void draw();
	std::string filename;
	sf::RenderWindow* window;
	std::vector<Equipment*> equipmentList;
};
