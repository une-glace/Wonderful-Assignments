#include "define.h"
#include "actor.h"
#include "helper.h"
#include "battlefield.h"

#include <cmath>

const std::map<std::pair<Type_T, Type_T>, double> typeRatio{
        {{Type::Normal, Type::Normal}, 1.},
        {{Type::Normal, Type::Grass},  1.},
        {{Type::Normal, Type::Fire},   1.},
        {{Type::Normal, Type::Water},  1.},
        {{Type::Normal, Type::Thunder}, 1.},
        {{Type::Grass,  Type::Normal}, 1.},
        {{Type::Grass,  Type::Grass},  .5},
        {{Type::Grass,  Type::Fire},   .5},
        {{Type::Grass,  Type::Water},  2.},
        {{Type::Grass, Type::Thunder}, 1.},
        {{Type::Fire,   Type::Normal}, 1.},
        {{Type::Fire,   Type::Grass},  2.},
        {{Type::Fire,   Type::Fire},   .5},
        {{Type::Fire,   Type::Water},  .5},
        {{Type::Fire, Type::Thunder}, 1.},
        {{Type::Water,  Type::Normal}, 1.},
        {{Type::Water,  Type::Grass},  .5},
        {{Type::Water,  Type::Fire},   2.},
        {{Type::Water,  Type::Water},  .5},
        {{Type::Water, Type::Thunder}, 1.},
        {{Type::Thunder,  Type::Normal}, 1.},
        {{Type::Thunder,  Type::Grass},  .5},
        {{Type::Thunder,  Type::Fire},   1.},
        {{Type::Thunder,  Type::Water},  2.},
        {{Type::Thunder, Type::Thunder}, .5},
};

// double cannot be passed by template until c++20
DefaultedMap<std::pair<Weather_T, Type_T>, int, 100> weatherRatio{
        {{Weather::Sunny, Type::Fire}, 150},
        {{Weather::Sunny, Type::Water}, 50},
        {{Weather::Rainy, Type::Fire}, 50},
        {{Weather::Rainy, Type::Water}, 150},
};

const std::map<Pet_T, Pet> allPets{
        {Pet::G, {Pet::G, Type::Grass, "Green", 110, 110, 10., 10., 10., {Skill::Tackle, Skill::Leaf, Skill::Synthesis}}},
        {Pet::R, {Pet::R, Type::Fire, "Red", 100, 100, 12., 10., 11., {Skill::Claw, Skill::Flame, Skill::Sunny}}},
        {Pet::B, {Pet::B, Type::Water, "Blue", 100, 100, 10., 14., 9., {Skill::Tackle, Skill::Stream, Skill::Rainy}}},
        {Pet::Y, {Pet::Y, Type::Thunder, "Yellow", 90, 90, 10., 10., 12., {Skill::Quick, Skill::Volt, Skill::Thunder}}},
        {Pet::P, {Pet::P, Type::Normal, "Pink", 120, 120, 9., 8., 7., {Skill::Slap, Skill::Sing, Skill::Paint}}}
};

const std::map<Potion_T, Potion> allPotions{
        {Potion::Revival, {Potion::Revival, "Revival Potion"}},
        {Potion::Attack,  {Potion::Attack,  "Attack Potion"}},
};

const std::map<Action_T, Action> allActions{
        {Action::Escape, {Action::Escape, "escape", 8}},
        {Action::Change, {Action::Change, "change", 6}},
        {Action::Skill, {Action::Skill, "skill", 0}},
        {Action::Potion, {Action::Potion, "potion", 5}}
};

// 普通伤害型技能的回调函数
// 以该函数实现为例，实现其余具有特殊效果的技能
// 返回值中，
// .success代表是否成功使用技能
// .damage表示对地方造成的伤害
// .prompt为输出语句，使用技能的完整输出为 "XXX’s X uses XXXX! ..."，感叹号之后的输出为.prompt的内容
SkillCallback defaultCb = [](BattleField *field, Actor *me, const Skill *skill) {
    std::stringstream ss;
    Pet &myPet = me->petOnCourt, &oppPet = me->opp->petOnCourt;

    bool success = true;
    // 伤害计算过程中各个量按照浮点数处理，最终四舍五入后转换成整型
    // 伤害 = 类型倍率 * 天气倍率（这里是百分数所以需要除以100.） * 威力 * 攻击力 / 防御
    int damage = int(round(typeRatio.at({skill->type, oppPet.type}) *
                           double(weatherRatio[{Weather_T(field->attr["weather"]), skill->type}]) / 100.
                           * skill->power * myPet.attack / oppPet.defense));
    std::cout << "Damage:" << damage << std::endl;
    //PRINT(ss, "Damage:", damage);

    if (oppPet.attr["faint"]) {
        oppPet.attr["faint"] = oppPet.attr["faintCnt"] = 0;
        if (oppPet.health - damage > 0.) {
            std::cout << me->opp->GetName() + "'s " + oppPet.petName + " wakes up" << std::endl;
            //PRINT(ss, me->opp->GetName() + "'s", oppPet.petName, "wakes up");
        }
    }

    SkillCbReturn ret;
    ret.success = success;
    ret.damage = damage;
    ret.prompt = ss.str();
    return ret;
};

// Slap技能回调函数
SkillCallback SlapCb = [](BattleField *field, Actor *me, const Skill *skill) {
    // TODO: Slap技能回调函数
    std::stringstream ss;
    Pet &myPet = me->petOnCourt, &oppPet = me->opp->petOnCourt;

    bool success = true;
    // 伤害计算过程中各个量按照浮点数处理，最终四舍五入后转换成整型
    // 伤害 = 类型倍率 * 天气倍率（这里是百分数所以需要除以100.） * 威力 * 攻击力 / 防御
    int damage_ = int(round(typeRatio.at({skill->type, oppPet.type}) *
                           double(weatherRatio[{Weather_T(field->attr["weather"]), skill->type}]) / 100.
                           * skill->power * myPet.attack / oppPet.defense));
    if (oppPet.attr["faint"]){
        damage_ *= 2;
    }
    int damage = int(round(damage_));
    std::cout << "Damage:" << damage << std::endl;
    //PRINT(ss, "Damage:", damage);

    if (oppPet.attr["faint"]) {
        oppPet.attr["faint"] = oppPet.attr["faintCnt"] = 0;
        if (oppPet.health - damage > 0.) {
            std::cout << me->opp->GetName() + "'s " + oppPet.petName + " wakes up" << std::endl;
            //PRINT(ss, me->opp->GetName() + "'s", oppPet.petName, "wakes up");
        }
    }

    SkillCbReturn ret;
    ret.success = success;
    ret.damage = damage;
    ret.prompt = ss.str();
    return ret;
};

// Sing技能回调函数
SkillCallback SingCb = [](BattleField *field, Actor *me, const Skill *skill) {
    // TODO: Sing技能回调函数
    std::stringstream ss;
    Pet &myPet = me->petOnCourt, &oppPet = me->opp->petOnCourt;

    bool success = true;
    if (oppPet.attr["faint"]){
        std::cout << "Waste" << std::endl;
        //PRINT(ss, "Waste");
    }
    else{
        oppPet.attr["faint"] = true;
        oppPet.attr["faintCnt"] = 3;
        std::cout << me -> opp -> GetName() + "'s " + oppPet.petName + " falls asleep" << std::endl;
        //PRINT(ss, me->opp->GetName() + "'s", oppPet.petName, "falls asleep");
    }

    SkillCbReturn ret;
    ret.success = success;
    ret.damage = 0;
    ret.prompt = ss.str();
    return ret;
};

// Paint技能回调函数
SkillCallback PaintCb = [](BattleField *field, Actor *me, const Skill *skill) {
    // TODO: Paint技能回调函数
    std::stringstream ss;
    Pet &myPet = me->petOnCourt, &oppPet = me->opp->petOnCourt;

    bool success = true;

    if (oppPet.attr["success"]){
        success = false;
    }

    if (success){
        std::cout << me -> opp -> GetName() + "'s " + oppPet.petName + " is painted" << std::endl;
        //PRINT(ss, me -> opp -> GetName() + "'s", oppPet.petName, "is painted");
        if (!oppPet.attr["weak"]){
            oppPet.attr["weak"] = 1;
            oppPet.defense /= 2;
        }
        else{
            std::cout << "Failure" << std::endl;
            //PRINT(ss, "Failure");
        }
    }

    SkillCbReturn ret;
    ret.success = success;
    ret.damage = 0;
    ret.prompt = ss.str();
    return ret;
};


// Synthesis技能回调函数
SkillCallback SynthesisCb = [](BattleField *field, Actor *me, const Skill *skill) {
    // TODO: Synthesis技能回调函数
    std::stringstream ss;
    Pet &myPet = me->petOnCourt, &oppPet = me->opp->petOnCourt;

    bool success = true;

    int recover = int(round(110 * 0.3));
    if (field -> attr["weather"] == Weather_T::Sunny){
        recover = int(round(110 * 0.4));
    }
    if (field -> attr["weather"] == Weather_T::Rainy){
        recover = int(round(110 * 0.2));
    }
    std::cout << "Recover:" << recover << std::endl;
    //PRINT(ss, "Recover:", recover);
    myPet.health += recover;

    SkillCbReturn ret;
    ret.success = success;
    ret.damage = 0;
    ret.prompt = ss.str();
    return ret;
};

// Sunny技能回调函数
SkillCallback SunnyCb = [](BattleField *field, Actor *me, const Skill *skill) {
    // TODO: Sunny技能回调函数
    std::stringstream ss;
    Pet &myPet = me->petOnCourt, &oppPet = me->opp->petOnCourt;

    bool success = true;

    std::cout << "Weather becomes Sunny" << std::endl;
    //PRINT(ss, "Weather becomes Sunny");
    field -> attr["weather"] = Weather_T::Sunny;
    field -> attr["weatherCnt"] = 5;

    SkillCbReturn ret;
    ret.success = success;
    ret.damage = 0;
    ret.prompt = ss.str();
    return ret;
};

SkillCallback RainyCb = [](BattleField *field, Actor *me, const Skill *skill) {
    // TODO: Rainy技能回调函数
    std::stringstream ss;
    Pet &myPet = me->petOnCourt, &oppPet = me->opp->petOnCourt;

    bool success = true;

    std::cout <<"Weather becomes Rainy" << std::endl;
    //PRINT(ss, "Weather becomes Rainy");
    field->attr["weather"] = Weather_T::Rainy;
    field->attr["weatherCnt"] = 5;

    SkillCbReturn ret;
    ret.success = success;
    ret.damage = 0;
    ret.prompt = ss.str();
    return ret;
};

// Thunder技能回调函数
SkillCallback ThunderCb = [](BattleField *field, Actor *me, const Skill *skill) {
    // TODO: Thunder技能回调函数
    std::stringstream ss;
    Pet &myPet = me->petOnCourt, &oppPet = me->opp->petOnCourt;

    bool success = true;

    if (field->attr["weather"] != Weather_T::Rainy) {
        success = false;
    }
    int damage = int(round(typeRatio.at({skill->type, oppPet.type}) *
                           double(weatherRatio[{Weather_T(field->attr["weather"]), skill->type}]) / 100.
                           * skill->power * myPet.attack / oppPet.defense));
    if (success) {
        std::cout << "Damage:" << damage << std::endl;
        //PRINT(ss, "Damage:", damage);
        if (oppPet.attr["faint"]) {
            oppPet.attr["faint"] = oppPet.attr["faintCnt"] = 0;
            if (oppPet.health - damage > 0.) {
                std::cout << me->opp->GetName() + "'s " + oppPet.petName + " wakes up" << std::endl;
                //PRINT(ss, me->opp->GetName() + "'s", oppPet.petName, "wakes up");
            }
        }
    }
    else {
        std::cout << "Failure" << std::endl;
        damage = 0;
    }

    SkillCbReturn ret;
    ret.success = success;
    ret.damage = damage;
    ret.prompt = ss.str();
    return ret;
};


const std::map<Skill_T, Skill> allSkills{
        {Skill::Tackle, {Skill::Tackle, "Tackle", Skill::attr::Attack, Type::Normal, 20., 0, defaultCb}},
        {Skill::Claw, {Skill::Claw, "Claw", Skill::attr::Attack, Type::Normal, 15., 0, defaultCb}},
        {Skill::Quick, {Skill::Quick, "Quick", Skill::attr::Attack, Type::Normal, 15., 1, defaultCb}},
        {Skill::Slap, {Skill::Slap, "Slap", Skill::attr::Attack, Type::Normal, 15., 0, SlapCb}},
        {Skill::Sing, {Skill::Sing, "Sing", Skill::attr::Supple, Type::Normal, 0., 0, SingCb}},
        {Skill::Paint, {Skill::Paint, "Paint", Skill::attr::Supple, Type::Normal, 0., 0, PaintCb}},
        {Skill::Leaf, {Skill::Leaf, "Leaf", Skill::attr::Attack, Type::Grass, 20., 0, defaultCb}},
        {Skill::Synthesis, {Skill::Synthesis, "Synthesis", Skill::attr::Supple, Type::Grass, 0., 0, SynthesisCb}},
        {Skill::Flame, {Skill::Flame, "Flame", Skill::attr::Attack, Type::Fire, 20., 0, defaultCb}},
        {Skill::Sunny, {Skill::Sunny, "Sunny", Skill::attr::Supple, Type::Fire, 0., 1, SunnyCb}},
        {Skill::Stream, {Skill::Stream, "Stream", Skill::attr::Attack, Type::Water, 20., 0, defaultCb}},
        {Skill::Rainy, {Skill::Rainy, "Rainy", Skill::attr::Supple, Type::Water, 0., 1, RainyCb}},
        {Skill::Volt, {Skill::Volt, "Volt", Skill::attr::Attack, Type::Thunder, 25., 0, defaultCb}},
        {Skill::Thunder, {Skill::Thunder, "Thunder", Skill::attr::Attack, Type::Thunder, 50., 0, ThunderCb}}
};
