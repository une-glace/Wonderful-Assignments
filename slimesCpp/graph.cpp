#include "graph.h"

using namespace easy2d;

GraphSlime::GraphSlime(Actor* left, Actor* right) {
    // TODO: 初始化图像画面
    
    player = left;
    enemy = right;
    // 修改窗口大小
    Window::setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // 创建一个场景
    battle_scene = new Scene;
    // 创建一个精灵
    auto background = new Sprite("res\\Background.png");
    // 移动精灵的位置
    background->setPos(0, 0);
    // 把精灵添加到场景中
    battle_scene->addChild(background);
    player_slime = SetSlimeOnPitch(0, SLIME_X_PLAYER, SLIME_Y);
    enemy_slime = SetSlimeOnPitch(1, SLIME_X_ENEMY, SLIME_Y);
    battle_scene->addChild(player_slime);
    battle_scene->addChild(enemy_slime);
    
    easy2d::Timer::add([=]() {
        easy2d::Game::quit();  // 将控制权退出图形窗口，返回控制台（图形窗口仍存在）
        });

    easy2d::SceneManager::enter(battle_scene);
    easy2d::Game::start();
}

void GraphSlime::Update() {
    // TODO: 更新图像画面
    battle_scene = new Scene();
    auto background = new Sprite("res\\Background.png");
    background->setPos(0, 0);
    battle_scene->addChild(background);
    player_slime = SetSlimeOnPitch(0, SLIME_X_PLAYER, SLIME_Y);
    enemy_slime = SetSlimeOnPitch(1, SLIME_X_ENEMY, SLIME_Y);
    battle_scene->addChild(player_slime);
    battle_scene->addChild(enemy_slime);

    easy2d::Timer::add([=]() {
        easy2d::Game::quit();  // 将控制权退出图形窗口，返回控制台（图形窗口仍存在）
        });

    easy2d::SceneManager::enter(battle_scene);
    easy2d::Game::start();

    player_slime->removeAllChildren();
    enemy_slime->removeAllChildren();
    battle_scene->removeAllChildren();
}

easy2d::Sprite* GraphSlime::SetSlimeOnPitch(bool PorE, float x, float y) {  // 参数PorE表示玩家或敌方
    // TODO: 设置史莱姆图片，调用设置HP图片和状态图标的函数
    Actor* actor;
    if (!PorE) {
        actor = player;
    }
    else {
        actor = enemy;
    }

    auto slime = new Sprite(GetSlimeImg(&actor->petOnCourt, PorE));
    slime->setPos(x, y);
    slime->setScale(0.3f);
    
    if (!PorE) {
        SetHPBar(slime, actor->petOnCourt.health, actor->petOnCourt.maxhealth, x + 105, y + 200);
    }
    else {
        SetHPBar(slime, actor->petOnCourt.health, actor->petOnCourt.maxhealth, x - 695, y + 200);
    }

    if (!PorE) {
        SetIcons(slime, actor->petOnCourt.attr, x - 150, y - 200);
    }
    else {
        SetIcons(slime, actor->petOnCourt.attr, x - 155, y - 200);
    }

    return slime;
}

void GraphSlime::SetHPBar(easy2d::Sprite* slimeImg, int hp, int hpMax, float x, float y) {
    // TODO: 设置HP图片（包括HP文字）
    auto menu = new Sprite("res\\Menu.png");
    menu->setPos(x, y);
    menu->setScale(3.0f);
    menu->setOpacity(0.6f);

    auto HP = new Text(std::to_string(hp) + "/" + std::to_string(hpMax));
    HP->setFontSize(90);
    HP->setFontWeight(700);
    HP->setAnchor(0.5, 0.5);
    HP->setPos(x + 190, y + 75);

    slimeImg->addChild(menu, 0);
    slimeImg->addChild(HP, 1);
}

void GraphSlime::SetIcons(easy2d::Sprite* slimeImg, DefaultedMap<std::string, int, 0>& attr, float x, float y) {
    // TODO: 设置状态图标
    if (attr["weak"]) {
        auto weak = new Sprite("res\\IconPaint.png");
        weak->setPos(x, y);
        weak->setScale(3.0f);
        slimeImg->addChild(weak);
    }
    if (attr["faint"]) {
        auto faint = new Sprite("res\\IconSleep.png");
        faint->setPos(x, y);
        faint->setScale(3.0f);
        slimeImg->addChild(faint);
        y += 100;
    }
}

std::string GraphSlime::GetSlimeImg(const Pet* slime, bool PorE) {
    // TODO: 获取史莱姆图片的文件名
    // PorE是0代表玩家，1代表敌人
    if (slime->pet == Pet::G) {
        if (!PorE) {
            return "res\\SlimeGrass.png";
        }
        else {
            return "res\\SlimeGrass_r.png";
        }
    }
    else if (slime->pet == Pet_T::R) {
        if (!PorE) {
            return "res\\SlimeFire.png";
        }
        else {
            return "res\\SlimeFire_r.png";
        }
    }
    else if (slime->pet == Pet_T::B) {
        if (!PorE) {
            return "res\\SlimeWater.png";
        }
        else {
            return "res\\SlimeWater_r.png";
        }
    }
    else if (slime->pet == Pet_T::Y) {
        if (!PorE) {
            return "res\\SlimeThunder.png";
        }
        else {
            return "res\\SlimeThunder_r.png";
        }
    }
    else{
        if (!PorE) {
            return "res\\SlimeNormal.png";
        }
        else {
            return "res\\SlimeNormal_r.png";
        }
    }
}

GraphSlime::~GraphSlime() {
    delete battle_scene;
    battle_scene = nullptr;
    delete player_slime;
    player_slime = nullptr;
    delete enemy_slime;
    enemy_slime = nullptr;
}


