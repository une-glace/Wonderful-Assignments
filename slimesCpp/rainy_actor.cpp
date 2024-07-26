#include "actor.h"
#include "battlefield.h"



RainyActor::RainyActor() {
    this->attr["revivalPotion"] = 1;
}

std::vector<Pet_T> RainyActor::ChooseStartingPet() {
    // TODO: 返回雨天队的出场宠物
    // 均匀随机选择首发宠物，例如可以将GBY的列表随机打乱，则各宠物首发概率相同
    std::vector<Pet_T> chosenPet;
    chosenPet.push_back(Pet_T::G);
    chosenPet.push_back(Pet_T::B);
    chosenPet.push_back(Pet_T::Y);
    ShuffleVector(chosenPet);
    petOnCourt = allPets.find(chosenPet[0]) -> second;
    availPets[chosenPet[1]] = allPets.find(chosenPet[1]) -> second;
    availPets[chosenPet[2]] = allPets.find(chosenPet[2]) -> second;
    std::cout << "Enemy has Green, Blue and Yellow, starting with " << allPets.find(chosenPet[0]) -> second.petName << "." << std::endl;
    return chosenPet;
}

Action_T RainyActor::ChooseAction() {
    // TODO: 返回选取的动作

    if (petOnCourt.pet != Pet_T::B && availPets.find(Pet_T::B) == availPets.end()) {
        return Action_T::Potion;
    }

    else if (petOnCourt.pet == Pet_T::G){
        if (opp -> petOnCourt.type == Type_T::Fire && availPets.find(Pet_T::B) != availPets.end()){
            return Action_T::Change;
        }
        else{
            return Action_T::Skill;
        }
    }

    else if (petOnCourt.pet == Pet_T::B){
        if ((field -> attr["weather"] != Weather_T::Sunny && opp -> petOnCourt.type == Type_T::Thunder) || (field -> attr["weather"] == Weather_T::Rainy && opp -> petOnCourt.type == Type_T::Grass)){
            if (availPets.find(Pet_T::G) != availPets.end()){
                return Action_T::Change;
            }
            else{
                if (field -> attr["weather"] == Weather_T::Rainy && availPets.find(Pet_T::Y) != availPets.end()){
                    return Action_T::Change;
                }
                else{
                    return Action_T::Skill;
                }
            }
        }
        else{
            return Action_T::Skill;
        }
    }

    else if (petOnCourt.pet == Pet_T::Y){
        if (opp -> petOnCourt.type == Type_T::Fire){
            if (availPets.find(Pet_T::B) != availPets.end()){
                return Action_T::Change;
            }
            else{
                return Action_T::Skill;
            }
        }
        else{
            if (field -> attr["weather"] == Weather_T::Rainy){
                return Action_T::Skill;
            }
            else{
                if (availPets.find(Pet_T::G) != availPets.end()){
                    return Action_T::Change;
                }
                else{
                    return Action_T::Skill;
                }
            }
        }
    }
}

Pet_T RainyActor::ChoosePet(bool active) {
    // TODO: 返回即将交换上场的宠物
    if (active) {
        if (availPets.find(Pet_T::B) != availPets.end() && petOnCourt.pet == Pet_T::G &&
            opp->petOnCourt.type == Type_T::Fire) {
            return Pet_T::B;
        }
        else if ((petOnCourt.pet == Pet_T::B && availPets.find(Pet_T::G) != availPets.end()) &&
                   (field->attr["weather"] != Weather_T::Sunny && opp->petOnCourt.type == Type_T::Thunder) ||
                   (field->attr["weather"] == Weather_T::Rainy && opp->petOnCourt.type == Type_T::Grass)) {
            return Pet_T::G;
        }
        else if (petOnCourt.pet == Pet_T::Y){
            if (opp -> petOnCourt.type == Type_T::Fire && availPets.find(Pet_T::B) != availPets.end()){
                return Pet_T::B;
            }
            else if (opp -> petOnCourt.type != Type_T::Fire && field -> attr["weather"] != Weather_T::Rainy && availPets.find(Pet_T::G) != availPets.end()){
                return Pet_T::G;
            }
        }
    }
    else{
        if (petOnCourt.pet != Pet_T::B){
            return Pet_T::B;
        }
        else{
            return Pet_T::G;
        }
    }
}

Skill_T RainyActor::ChooseSkill() {
    // TODO: 选择场上宠物使用的技能
    if (petOnCourt.pet == Pet_T::G){
        if (opp -> petOnCourt.type == Type_T::Fire && availPets.find(Pet_T::B) == availPets.end()){
            if (availPets[Pet_T::G].health >= availPets[Pet_T::G].maxhealth * 2. / 3){
                return Skill_T::Tackle;
            }
            else{
                return Skill_T::Synthesis;
            }
        }
        else if (opp -> petOnCourt.type == Type_T::Grass){
            if (petOnCourt.health >= petOnCourt.maxhealth / 2.){
                return Skill_T::Tackle;
            }
            else{
                return Skill_T::Synthesis;
            }
        }
        else if (petOnCourt.health >= petOnCourt.maxhealth / 2. || opp -> petOnCourt.health <= opp -> petOnCourt.maxhealth / 5.){
            return Skill_T::Leaf;
        }
        else{
            return Skill_T::Synthesis;
        }
    }

    else if (petOnCourt.pet == Pet_T::B){
        if (field -> attr["weather"] == Weather_T::Sunny){
            return Skill_T::Rainy;
        }
        else if (availPets.find(Pet_T::G) == availPets.end() && ((field -> attr["weather"] != Weather_T::Sunny && opp -> petOnCourt.type == Type_T::Thunder) || (field -> attr["weather"] == Weather_T::Rainy && opp -> petOnCourt.type == Type_T::Grass))){
            if (!(field -> attr["weather"] == Weather_T::Rainy && availPets.find(Pet_T::Y) != availPets.end())){
                return Skill_T::Stream;
            }
        }
        else if (field -> attr["weather"] != Weather_T::Sunny && opp -> petOnCourt.type ==Type_T::Water){
            return Skill_T::Tackle;
        }
        else{
            if (field -> attr["weather"] == Weather_T::Rainy){
                return Skill_T::Stream;
            }
            else{
                return Skill_T::Rainy;
            }
        }
    }

    else if (petOnCourt.pet == Pet_T::Y){
        if (opp -> petOnCourt.health < opp -> petOnCourt.maxhealth / 5.){
            return Skill_T::Quick;
        }
        else{
            if (opp -> petOnCourt.type == Type_T::Fire){
                if (availPets.find(Pet_T::B) == availPets.end()){
                    return Skill_T::Volt;
                }
            }
            else{
                if (field -> attr["weather"] == Weather_T::Rainy){
                    return Skill_T::Thunder;
                }
                else{
                    if (availPets.find(Pet_T::G) == availPets.end()){
                        if (opp -> petOnCourt.type ==Type_T::Thunder || opp -> petOnCourt.type == Type_T::Grass){
                            return Skill_T::Quick;
                        }
                        else{
                            return Skill_T::Volt;
                        }
                    }
                }
            }
        }
    }
}

PotionInfo RainyActor::ChoosePotion() {
    // TODO: 返回使用的药剂
    if (attr["revivalPotion"] > 0 && availPets.find(Pet_T::B) == availPets.end()){
        return {Potion_T::Revival, Pet_T::B};
    }
}

std::string RainyActor::GetName() {
    return "Enemy";//"Rainy Team";
}

