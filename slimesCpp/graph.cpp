#include "graph.h"

using namespace easy2d;

GraphSlime::GraphSlime(Actor* left, Actor* right) {
    // TODO: ��ʼ��ͼ����
    
    player = left;
    enemy = right;
    // �޸Ĵ��ڴ�С
    Window::setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // ����һ������
    battle_scene = new Scene;
    // ����һ������
    auto background = new Sprite("res\\Background.png");
    // �ƶ������λ��
    background->setPos(0, 0);
    // �Ѿ�����ӵ�������
    battle_scene->addChild(background);
    player_slime = SetSlimeOnPitch(0, SLIME_X_PLAYER, SLIME_Y);
    enemy_slime = SetSlimeOnPitch(1, SLIME_X_ENEMY, SLIME_Y);
    battle_scene->addChild(player_slime);
    battle_scene->addChild(enemy_slime);
    
    easy2d::Timer::add([=]() {
        easy2d::Game::quit();  // ������Ȩ�˳�ͼ�δ��ڣ����ؿ���̨��ͼ�δ����Դ��ڣ�
        });

    easy2d::SceneManager::enter(battle_scene);
    easy2d::Game::start();
}

void GraphSlime::Update() {
    // TODO: ����ͼ����
    battle_scene = new Scene();
    auto background = new Sprite("res\\Background.png");
    background->setPos(0, 0);
    battle_scene->addChild(background);
    player_slime = SetSlimeOnPitch(0, SLIME_X_PLAYER, SLIME_Y);
    enemy_slime = SetSlimeOnPitch(1, SLIME_X_ENEMY, SLIME_Y);
    battle_scene->addChild(player_slime);
    battle_scene->addChild(enemy_slime);

    easy2d::Timer::add([=]() {
        easy2d::Game::quit();  // ������Ȩ�˳�ͼ�δ��ڣ����ؿ���̨��ͼ�δ����Դ��ڣ�
        });

    easy2d::SceneManager::enter(battle_scene);
    easy2d::Game::start();

    player_slime->removeAllChildren();
    enemy_slime->removeAllChildren();
    battle_scene->removeAllChildren();
}

easy2d::Sprite* GraphSlime::SetSlimeOnPitch(bool PorE, float x, float y) {  // ����PorE��ʾ��һ�з�
    // TODO: ����ʷ��ķͼƬ����������HPͼƬ��״̬ͼ��ĺ���
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
    // TODO: ����HPͼƬ������HP���֣�
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
    // TODO: ����״̬ͼ��
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
    // TODO: ��ȡʷ��ķͼƬ���ļ���
    // PorE��0������ң�1�������
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


