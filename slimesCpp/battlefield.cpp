#include "battlefield.h"

#include "define.h"
#include "helper.h"
#include "graph.h"

#include <string>


BattleField::BattleField(int maxRounds): maxRounds_(maxRounds) {}

void BattleField::Reset() {
    this->rounds_ = 0;
    this->actors_[0] = this->actors_[1] = nullptr;
    this->attr.clear();
}

void BattleField::Run() {
    this->BeforeGame_();

    if (easy2d::Game::init()) { //GRAPH
        GraphSlime graph(this->actors_[0], this->actors_[1]); //GRAPH
        
        bool ending = false;
        while (!ending) {
            LOG("--------------------------------------------------\n");
            this->StateCheck_();
            LOG("Round", ++this->rounds_); LOG('\n');
            std::vector<ActionInfo> actions = this->ChooseActionPhase_();
            this->PerformActionPhase_(actions);
            graph.Update(); //GRAPH
            this->HandleBeatenPetsPhase_();
            ending = this->ShowInformationPhase_();
            graph.Update(); //GRAPH
        }
    } //GRAPH
    easy2d::Game::destroy(); //GRAPH
}

void BattleField::SetActor(Actor *a, Actor *b) {
    this->actors_[0] = a; this->actors_[1] = b;
    a->opp = b; b->opp = a;
    a->field = b->field = this;
}

void BattleField::BeforeGame_() {
    // welcome messages
    LOG("Welcome to Battle of Slimes 2!\n");

    // TODO: 游戏开始前，选择宠物
    actors_[1]->ChooseStartingPet();
    actors_[0]->ChooseStartingPet();
    std::cout << Whose(actors_[0] -> GetName()) << " " <<actors_[0] -> petOnCourt.petName << ": HP " << actors_[0] -> petOnCourt.health << " || " <<
    Whose(actors_[1] -> GetName()) << " " << actors_[1] -> petOnCourt.petName << ": HP " << actors_[1] -> petOnCourt.health << std::endl;

    // game start messages
    LOG("Battle starts!\n");
}

void BattleField::StateCheck_() {
    // TODO: 每回合开始前，检查天气、昏睡回合数等
    if (attr["weather"]){
        attr["weatherCnt"]--;
        if (attr["weatherCnt"] == 0){
            attr["weather"] = 0;
            std::cout << "Weather becomes normal" << std::endl;
        }
    }
    if (actors_[0] -> petOnCourt.attr["faint"]){
        attr["faintCnt"]--;
        if (attr["faintCnt"] == 0){
            actors_[0] -> petOnCourt.attr["faint"] = 0;
            std::cout << Whose(actors_[0] -> GetName()) << actors_[0] -> petOnCourt.petName << " wakes up" << std::endl;
        }
    }
    if (actors_[1] -> petOnCourt.attr["faint"]){
        attr["faintCnt"]--;
        if (attr["faintCnt"] == 0){
            actors_[1] -> petOnCourt.attr["faint"] = 0;
            std::cout << Whose(actors_[1] -> GetName()) << actors_[1] -> petOnCourt.petName << " wakes up" << std::endl;
        }
    }
}

std::vector<ActionInfo> BattleField::ChooseActionPhase_() {
    // TODO: 向各Actor轮询采用的动作并返回
    std::vector<ActionInfo> actions;
    ActionInfo action1, action2;
    action1.action = actors_[0] -> ChooseAction();
    action2.action = actors_[1] -> ChooseAction();
    action1.actor = actors_[0];
    action2.actor = actors_[1];
    action1.priority = allActions.find(action1.action) -> second.priority;
    action2.priority = allActions.find(action2.action) -> second.priority;

    int choose = 0;
    do{
        if (action1.action == Action_T::Change){
            choose = actors_[0] -> ChoosePet(true);
            if (choose != 0){
                action1.u.pet = actors_[0] -> availPets[static_cast<Pet_T>(choose)].pet;
            }
        }
        else{
            choose = actors_[0] -> ChooseSkill();
            if (choose != 0){
                action1.u.skill = actors_[0] -> petOnCourt.skills[static_cast<Skill_T>(choose - 1)];
                action1.priority = allSkills.find(action1.u.skill) -> second.priority;
            }
        }
        if (choose == 0){
            action1.action = actors_[0] -> ChooseAction();
            action1.priority = allActions.find(action1.action) -> second.priority;
        }
    } while (choose == 0);

    if (action2.action == Action_T::Change){
        action2.u.pet = static_cast<Pet_T>(actors_[1] -> ChoosePet(true));
    }
    else if(action2.action == Action_T::Skill){
        action2.u.skill = static_cast<Skill_T>(actors_[1] -> ChooseSkill());
        action2.priority = allSkills.find(action2.u.skill) -> second.priority;
    }
    else if (action2.action == Action_T::Potion){}

    if (action1.priority > action2.priority){
        actions.push_back(action2);
        actions.push_back(action1);
    }
    else if (action1.priority < action2.priority){
        actions.push_back(action1);
        actions.push_back(action2);
    }
    else{
        if (actors_[0] -> petOnCourt.speed > actors_[1] -> petOnCourt.speed){
            actions.push_back(action2);
            actions.push_back(action1);
        }
        else{
            actions.push_back(action1);
            actions.push_back(action2);
        }
    }

    return actions;
}

void BattleField::PerformActionPhase_(std::vector<ActionInfo> &actions) {
    // TODO: 执行Actor选取的动作
    actors_[0] -> petOnCourt.attr["success"] = 0;
    actors_[1] -> petOnCourt.attr["success"] = 0;

    while (!actions.empty()){
        ActionInfo actionInfo = actions.back();
        Actor* actor = actionInfo.actor;
        Actor* enemy = actor -> opp;
        actions.pop_back();

        if (actor -> petOnCourt.health <= 0){
            continue;
        }
        if (actionInfo.action == Action_T::Change){
            actor -> availPets[actor -> petOnCourt.pet] = actor -> petOnCourt;
            actor -> petOnCourt = actor -> availPets[actionInfo.u.pet];
            actor -> availPets.erase(actionInfo.u.pet);
            std::cout << actor -> GetName() << " sends " << actor -> petOnCourt.petName << std::endl;
        }
        else if (actionInfo.action == Action_T::Skill){
            if (actor -> petOnCourt.attr["faint"]){
                std::cout << Whose(actor -> GetName()) << " " << actor -> petOnCourt.petName << " is sleeping" << std::endl;
                continue;
            }
            actor -> petOnCourt.attr["success"] = 1;
            std::cout << Whose(actor -> GetName()) << " " << actor -> petOnCourt.petName << " uses " << allSkills.find(actionInfo.u.skill) -> second.skillName << "! ";
            SkillCbReturn scbr = allSkills.find(actionInfo.u.skill)->second.cb(this, actor, &allSkills.find(actionInfo.u.skill)->second);
            enemy -> petOnCourt.health -= scbr.damage;
        }
        else if (actionInfo.action == Action_T::Potion){}
    }
}

void BattleField::HandleBeatenPetsPhase_() {
    // TODO: 检查并处理本轮中被击败的宠物
    if (actors_[0] -> petOnCourt.health <= 0){
        std::cout << Whose(actors_[0] -> GetName()) << actors_[0] -> petOnCourt.petName << " is beaten" << std::endl;
        if (actors_[0] -> availPets.empty()){
            std::cout << "You lose" << std::endl;
            exit(0);
        }
        Pet_T nextPet = actors_[0] -> ChoosePet(0);
        actors_[0] -> grave[actors_[0] -> petOnCourt.pet] = actors_[0] -> petOnCourt;
        actors_[0] -> petOnCourt = actors_[0] -> availPets[nextPet];
        actors_[0] -> availPets.erase(nextPet);
        std::cout << actors_[0] -> GetName() << " sends " << actors_[0] -> petOnCourt.petName << std::endl;
    }
    if (actors_[1] -> petOnCourt.health <= 0) {
        std::cout << Whose(actors_[1]->GetName()) << actors_[1]->petOnCourt.petName << " is beaten" << std::endl;
        if (actors_[1]->availPets.empty()) {
            std::cout << "You win" << std::endl;
            exit(0);
        }
        Pet_T nextPet = actors_[1]->ChoosePet(0);
        actors_[1]->grave[actors_[1]->petOnCourt.pet] = actors_[1]->petOnCourt;
        actors_[1]->petOnCourt = actors_[1]->availPets[nextPet];
        actors_[1]->availPets.erase(nextPet);
        std::cout << actors_[1]->GetName() << " sends " << actors_[1]->petOnCourt.petName << std::endl;
    }
}

bool BattleField::ShowInformationPhase_() {
    // TODO: 显示剩余HP等信息，并返回游戏是否分出胜负

    std::cout << Whose(actors_[0] -> GetName()) << " " << actors_[0] -> petOnCourt.petName << ": HP " << actors_[0] -> petOnCourt.health << " || " <<
    Whose(actors_[1] -> GetName()) << " " <<actors_[1] -> petOnCourt.petName << ": HP " << actors_[1] -> petOnCourt.health << std::endl;
    return rounds_ == 100;
}

std::string BattleField::Whose(std::string name) {
    return (name == "You" ? "Your" : name + "'s");
}
