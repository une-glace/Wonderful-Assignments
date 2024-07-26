#include "actor.h"

#include <sstream>
#include <unordered_set>


HumanActor::HumanActor() {
    this->GetName();
}

std::vector<Pet_T> HumanActor::ChooseStartingPet() {
    // TODO: 从用户输入中得到上场的宠物

    std::cout << "You can select three from five (1 for Green, 2 for Red, 3 for Blue, 4 for Yellow, 5 for Pink)." << std::endl;
    int choose_1, choose_2, choose_3;

    std::cout << "Select your 1st slime: ";
    while(std::cin >> choose_1 && (choose_1 < 1 || choose_1 > 5)) {
        std::cout << "Select your 1st slime: ";
    }
    allPetTs.push_back(static_cast<Pet_T>(choose_1));

    std::cout << "Select your 2nd slime: ";
    while(std::cin >> choose_2 && (choose_2 < 1 || choose_2 > 5 || choose_2 == choose_1)) {
        std::cout << "Select your 2nd slime: ";
    }
    allPetTs.push_back(static_cast<Pet_T>(choose_2));

    std::cout << "Select your 3rd slime: ";
    while(std::cin >> choose_3 && (choose_3 < 1 || choose_3 > 5 || choose_3 == choose_1 || choose_3 == choose_2)) {
        std::cout << "Select your 3rd slime: ";
    }
    allPetTs.push_back(static_cast<Pet_T>(choose_3));

    std::cout << "You have " << allPets.find(static_cast<Pet_T>(choose_1))->second.petName << ","
              << allPets.find(static_cast<Pet_T>(choose_2))->second.petName << " and "
              << allPets.find(static_cast<Pet_T>(choose_3))->second.petName << ","
              << " starting with " << allPets.find(static_cast<Pet_T>(choose_1))->second.petName << std::endl;

    petOnCourt = allPets.find(static_cast<Pet_T>(choose_1))->second;
    availPets[static_cast<Pet_T>(choose_2)] = allPets.find(static_cast<Pet_T>(choose_2))->second;
    availPets[static_cast<Pet_T>(choose_3)] = allPets.find(static_cast<Pet_T>(choose_3))->second;

    return allPetTs;
}

Action_T HumanActor::ChooseAction() {
    // TODO: 返回选取的动作

    int choose;
    if(availPets.empty()) {
        std::cout << "Select your action (0 for escape, 1 for skills): ";
        while(std::cin >> choose && (choose < 0 || choose > 1)) {
            std::cout << "Select your action (0 for escape, 1 for skills): ";
        }
    }
    else {
        std::cout << "Select your action (0 for escape, 1 for skills, 2 for change): ";
        while(std::cin >> choose && (choose < 0 || choose > 2)) {
            std::cout << "Select your action (0 for escape, 1 for skills, 2 for change): ";
        }
    }

    if (choose == 0){
        std::cout << "You escape" << std::endl;
        exit(0);
    }

    return static_cast<Action_T>(choose);
}

Pet_T HumanActor::ChoosePet(bool active) {
    // TODO: 返回即将交换上场的宠物
    // active 代表<主动交换宠物>or<场上宠物阵亡后被动交换宠物>

    int availNum = 0;
    Pet availPet1, availPet2;
    int num1, num2;
    for (int i = 0; i < 3; i++){
        if (availPets.find(allPetTs[i]) != availPets.end()){
            availNum++;
            if (availNum == 1) {
                availPet1 = availPets[allPetTs[i]];
                num1 = i + 1;
            }
            else {
                availPet2 = availPets[allPetTs[i]];
                num2 = i + 1;
            }
        }
    }

    if (active){
        std::cout << "Select your next slime (0 for back, ";
    }
    else{
        std::cout << "Select your next slime (";
    }

    std::cout <<num1 << " for " << availPet1.petName << "-HP" << availPet1.health;
    if (availNum == 1){
        std::cout << "): ";
    }
    else{
        std::cout << ", " << num2 << " for " << availPet2.petName << "-HP" << availPet2.health << "): ";
    }

    int choose;
    while (std::cin >> choose){
        if (choose == 0 || choose == num1 || choose == num2){
            break;
        }

        if (active){
            std::cout << "Select your next slime (0 for back, ";
        }
        else{
            std::cout << "Select your next slime (";
        }

        std::cout <<num1 << " for " << availPet1.petName << "-HP" << availPet1.health;
        if (availNum == 1){
            std::cout << "): ";
        }
        else{
            std::cout << ", " << num2 << " for " << availPet2.petName << "-HP" << availPet2.health << "): ";
        }
    }

    if (choose != 0){
        return availPets[allPetTs[choose - 1]].pet;
    }
    else{
        return static_cast<Pet_T>(0);

    }
}

Skill_T HumanActor::ChooseSkill() {
    // TODO: 选择场上宠物使用的技能
    std::cout << "Select the skills (0 for back, ";
    std::cout << "1 for " << allSkills.find(petOnCourt.skills[0]) -> second.skillName << ", ";
    std::cout << "2 for " << allSkills.find(petOnCourt.skills[1]) -> second.skillName << ", ";
    std::cout << "3 for " << allSkills.find(petOnCourt.skills[2]) -> second.skillName << "): ";

    int choose;
    while (std::cin >> choose && (choose < 0 || choose > 3)){
        std::cout << "Select the skills (0 for back, ";
        std::cout << "1 for " << allSkills.find(petOnCourt.skills[0]) -> second.skillName << ", ";
        std::cout << "2 for " << allSkills.find(petOnCourt.skills[1]) -> second.skillName << ", ";
        std::cout << "3 for " << allSkills.find(petOnCourt.skills[2]) -> second.skillName << "): ";
    }

    return static_cast<Skill_T>(choose);
}

std::string HumanActor::GetName() {
    return "You";
}
