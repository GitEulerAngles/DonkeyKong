#include "map.h"
#include <string>
#include <fstream>

void map::setupColliders(physicsWorld* world) {
    std::ifstream file;
    std::string line;
    file.open("colliders.txt");
    while (std::getline(file, line)) {
        static int index = 0;
        int numberIndex = 0;
        float numbers[4];
        std::string newNumber = "";
        for (int i = 0; i < line.length(); i++) {
            if ((int)line[i] == 32) {
                numbers[numberIndex] = std::stoi(newNumber);
                numberIndex++;
                newNumber = "";
            }
            else
                newNumber += line[i];
        }
        numbers[numberIndex] = std::stoi(newNumber);

        staticObjects[index].collider.center = bas::vector2<float>((numbers[0]+numbers[2])*1.5, (numbers[1]+numbers[3])*1.5);
        staticObjects[index].collider.dimension = bas::vector2<float>((numbers[2]-numbers[0])*1.5, (numbers[3]-numbers[1])*1.5);
        staticObjects[index].collider.setAABB();
        world->addStaticObject(&staticObjects[index]);
        index++;
    }
}
