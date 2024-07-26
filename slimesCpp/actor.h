#ifndef ACTOR_H
#define ACTOR_H

#include "define.h"
#include "helper.h"

#include <map>
#include <vector>


class BattleField;


class Actor {
public:
    virtual ~Actor() = default;
    virtual std::vector<Pet_T> ChooseStartingPet() = 0;
    virtual Action_T ChooseAction() = 0;
    virtual Pet_T ChoosePet(bool) = 0;
    virtual Skill_T ChooseSkill() = 0;
    virtual PotionInfo ChoosePotion() { return {}; }
    virtual std::string GetName() = 0;

    Actor* opp;                                 // 敌方指针
    BattleField* field;                         // 战场指针
    Pet petOnCourt;                             // 当前场上的宠物
    std::map<Pet_T, Pet> availPets;             // 可以换上场的宠物
    std::map<Pet_T, Pet> grave;                 // 战败的宠物
    DefaultedMap<std::string, int, 0> attr;     // 动态属性，如剩余药剂数量等
};

class SunnyActor : public Actor {
public:
    ~SunnyActor() override = default;
    std::vector<Pet_T> ChooseStartingPet() override;
    Action_T ChooseAction() override;
    Pet_T ChoosePet(bool) override;
    Skill_T ChooseSkill() override;
    std::string GetName() override;
};

class RainyActor : public Actor {
public:
    RainyActor();
    ~RainyActor() override = default;
    std::vector<Pet_T> ChooseStartingPet() override;
    Action_T ChooseAction() override;
    Pet_T ChoosePet(bool) override;
    Skill_T ChooseSkill() override;
    PotionInfo ChoosePotion() override;
    std::string GetName() override;
};

class HumanActor : public Actor {
public:
    HumanActor();
    ~HumanActor() override = default;
    std::vector<Pet_T> ChooseStartingPet() override;
    Action_T ChooseAction() override;
    Pet_T ChoosePet(bool) override;
    Skill_T ChooseSkill() override;
    std::string GetName() override;

private:
    std::vector<Pet_T> allPetTs;
};

#endif //ACTOR_H
