#ifndef DEFINE_H
#define DEFINE_H

#include "helper.h"

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <functional>

/* XXX 为定义的结构体
 * XXX_T 为枚举类型
 * allXXXs 为按照文档要求填好的各宠物/技能/...的定义 */

// 宠物/技能的属性
struct Type {
    enum { Normal = 1, Grass, Fire, Water, Thunder };
};
using Type_T = decltype(Type::Normal);
extern const std::map<std::pair<Type_T, Type_T>, double> typeRatio;

// 属性ta是否克制属性ta
inline bool IsMoreEffective(Type_T ta, Type_T tb) {
    return typeRatio.at({ta, tb}) > 1.;
}


// 天气
struct Weather {
    enum { Normal = 0, Sunny, Rainy };
};
using Weather_T = decltype(Weather::Normal);
extern DefaultedMap<std::pair<Weather_T, Type_T>, int, 100> weatherRatio;   // 某个天气下某个属性技能的威力倍率，为百分数

struct Pet;
struct Skill;
class BattleField;
class Actor;

// 技能造成的伤害，以及睡眠等特效通过回调函数实现
// 在BattleField::PerformActionPhase_阶段会调用回调函数
struct SkillCbReturn {
    bool success;
    int damage;
    std::string prompt;
};

using SkillCallback = std::function<SkillCbReturn(BattleField *, Actor *, const Skill *)>;


// 技能
struct Skill {
    enum {
        Tackle = 1, Claw, Quick, Slap, Sing, Paint, Leaf, Synthesis, Flame, Sunny, Stream, Rainy, Volt, Thunder
    } skill;
    std::string skillName;
    enum attr { Attack, Supple } attr;
    Type_T type;
    double power;
    int priority;
    SkillCallback cb;
};
using Skill_T = decltype(Skill::Tackle);
extern const std::map<Skill_T, Skill> allSkills;


// 宠物
struct Pet {
    enum { G = 1, R, B, Y, P } pet;
    Type_T type;
    std::string petName;
    int maxhealth;
    int health;
    double attack;
    double defense;
    double speed;
    std::vector<Skill_T> skills;
    DefaultedMap<std::string, int, 0> attr;
};
using Pet_T = decltype(Pet::G);
extern const std::map<Pet_T, Pet> allPets;


// 药剂
struct Potion {
    enum { Revival = 0, Attack } potion;
    std::string potionName;
};
using Potion_T = decltype(Potion::Revival);
extern const std::map<Potion_T, Potion> allPotions;


// 行为
struct Action {
    enum { Escape = 0, Skill, Change, Potion } action;
    std::string actionName;
    int priority;
};
using Action_T = decltype(Action::Change);
extern const std::map<Action_T, Action> allActions;

/* 以下为部分接口返回值结构体定义 */

// Actor::ChoosePotion 接口返回值，返回药剂类型以及使用对象
struct PotionInfo {
    Potion_T potionT;
    Pet_T petT;
};

// BattleField::ChooseActionPhase_ 返回值，用于BattleField::PerformActionPhase_等后续执行动作的阶段
struct ActionInfo {
    Action_T action;
    Actor *actor;
    int priority;
    union {
        Pet_T pet;
        Skill_T skill;
        PotionInfo potion;
    } u;
};

#endif //DEFINE_H
