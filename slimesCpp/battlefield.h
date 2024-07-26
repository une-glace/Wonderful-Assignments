#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "actor.h"
#include "helper.h"


class BattleField {
public:
    explicit BattleField(int maxRounds);
    void Reset();
    void Run();
    void SetActor(Actor *, Actor *);

    DefaultedMap<std::string, int, 0> attr; // 动态属性，如天气等

private:
    void BeforeGame_();
    void StateCheck_();
    std::vector<ActionInfo> ChooseActionPhase_();
    void PerformActionPhase_(std::vector<ActionInfo> &);
    void HandleBeatenPetsPhase_();
    bool ShowInformationPhase_();
    std::string Whose(std::string); // "Your" or "Enemy's" 

    int rounds_;
    int maxRounds_;
    Actor *actors_[2];
};

#endif //BATTLEFIELD_H
