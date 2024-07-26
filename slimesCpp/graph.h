#ifndef GRAPH_H
#define GRAPH_H

#include "battlefield.h"
#include <easy2d/easy2d.h>

#define WINDOW_WIDTH 1375
#define WINDOW_HEIGHT 792
#define SLIME_X_PLAYER 180
#define SLIME_X_ENEMY 970
#define SLIME_Y 500

class GraphSlime {
private:
	easy2d::Scene* battle_scene;
	easy2d::Sprite* player_slime;
	easy2d::Sprite* enemy_slime;
	Actor* player;
	Actor* enemy;

	easy2d::Sprite* SetSlimeOnPitch(bool PorE, float x, float y);
	void SetHPBar(easy2d::Sprite* slimeImg, int hp, int hpMax, float x, float y);
	void SetIcons(easy2d::Sprite* slimeImg, DefaultedMap<std::string, int, 0>& attr, float x, float y);
	std::string GetSlimeImg(const Pet* slime, bool PorE);
public:
	GraphSlime(Actor* left, Actor* right);
	~GraphSlime();
	void Update();
};

#endif //GRAPH_H
