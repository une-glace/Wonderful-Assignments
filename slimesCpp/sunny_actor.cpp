#include "actor.h"
#include "battlefield.h"


std::vector<Pet_T> SunnyActor::ChooseStartingPet() {
    // TODO: 返回晴天队的出场宠物
    // 晴天队有PGR，永远首发P
    std::vector<Pet_T> chosenPet;
    chosenPet.push_back(Pet_T::P);
    chosenPet.push_back(Pet_T::G);
    chosenPet.push_back(Pet_T::R);
    std::cout << "Enemy has Pink, Green and Red, starting with Pink." << std::endl;
    petOnCourt = allPets.find(Pet_T::P)->second;
    availPets[Pet_T::G] = allPets.find(Pet_T::G)->second;
    availPets[Pet_T::R] = allPets.find(Pet_T::R)->second;
    return chosenPet;
}

Action_T SunnyActor::ChooseAction() {
    // TODO: 返回选取的动作
    if(petOnCourt.pet == Pet_T::G && petOnCourt.health <= petOnCourt.maxhealth / 2 && !availPets.empty()) {
        return Action_T::Change;
    } else {
        return Action_T::Skill;
    }
}

Pet_T SunnyActor::ChoosePet(bool active) {
    // TODO: 返回即将交换上场的宠物
    if (active){
        return Pet_T::R;
    }
    else {
        if (petOnCourt.pet != Pet_T::G) {
            return Pet_T::G;
        }
        else {
            return Pet_T::R;
        }
    }
}

Skill_T SunnyActor::ChooseSkill() {
    // TODO: 选择场上宠物使用的技能
    if (petOnCourt.pet == Pet_T::P){
        if (opp -> petOnCourt.health * 1. < opp -> petOnCourt.maxhealth * 1. / 3 &&(opp -> petOnCourt.attr["faint"] || opp -> petOnCourt.attr["weak"])){
            return Skill_T::Slap;
        }
        else if (!opp -> petOnCourt.attr["faint"]){
            return Skill_T::Sing;
        }
        else if (!opp -> petOnCourt.attr["weak"]){
            return Skill_T::Paint;
        }
        else {
            return Skill_T::Slap;
        }
    }
    else if (petOnCourt.pet == Pet_T::R){
        if (opp -> petOnCourt.health < petOnCourt.maxhealth * 1. / 5 && (opp -> petOnCourt.type == Type_T::Fire || opp -> petOnCourt.type == Type_T::Water)){
            return Skill_T::Claw;
        }
        else if (field -> attr["weather"] == Weather_T::Sunny){
            return Skill_T::Flame;
        }
        else {
            return Skill_T::Sunny;
        }
    }
    else{
        if (petOnCourt.health >= petOnCourt.maxhealth / 2 || (petOnCourt.health > opp -> petOnCourt.health && availPets.empty())){
            if (opp -> petOnCourt.type == Type_T::Water){
                return Skill_T::Leaf;
            }
            else{
                return Skill_T::Tackle;
            }
        }
        else{
            return Skill_T::Synthesis;
        }
    }
}

std::string SunnyActor::GetName() {
    return "Enemy";//"Sunny Team";
}
