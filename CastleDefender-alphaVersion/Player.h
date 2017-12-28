#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"

//Класс главного героя
using namespace sf;

const int H = 12, W = 21;


String TileMap[H] = {  //Я Карта.... Я Карта (ost. Даша путешественница) 
	"BBBBBBBBBBBBBBBBBBBBB",
	"B                   B",
	"B0                  B",
	"BBB       B         B",
	"B                  BB",
	"B    BBB     BBB    B",
	"B                   B",
	"B                   B",
	"BBBBB     BBB     BBB",
	"B                   B",
	"B              B   0B",
	"BBBBBBBBBBBBBBBBBBBBB",
};

class PLAYER {

public:

	float x, y, dx, dy = 0.01, w, h;
	AnimationManager anim;
	bool shoot, dir;
	enum { stay, walk, duck, jump } STATE;
	std::map<std::string, bool> key;

	PLAYER(AnimationManager &a, int xH, int yH) {
		anim = a;
		STATE = stay;
		x = xH;
		y = yH;

	}

	//Проверка нажатия клавиш
	void KeyCheck() {
		if (key["Left"]) {
			dir = 1;
			if (STATE != duck) dx = -0.1;
			if (STATE == stay) STATE = walk;
		}
		if (key["Right"]) {
			dir = 0;
			if (STATE != duck) dx = 0.1;
			if (STATE == stay) STATE = walk;
		}
		if (key["Space"])
			if (STATE == stay || STATE == walk) {
				dy = -0.2; STATE = jump;
			}
		if (key["Down"])
			if (STATE == stay) {
				dx = 0;
				STATE = duck;
			}
		if (key["F"]) shoot = true;

		/////////////////////Отпускаем клавиши////////////////
		if (!(key["Right"] || key["Left"])) {
			dx = 0;
			if (STATE == walk) STATE = stay;
		}
		if (!key["Down"])
			if (STATE == duck) STATE = stay;
		if (!key["F"])shoot = false;
		key["Right"] = key["Left"] = key["Down"] = key["Space"] = key["F"] = false;
	}

	void AnimationS(float time) {
		if (STATE == stay) { anim.set("stay"); h = 50; w = 41; }
		if (STATE == walk) { anim.set("walk"); h = 52; w = 41; }
		if (STATE == jump) { anim.set("jump"); h = 30; w = 29; }
		if (STATE == duck) { anim.set("duck"); h = 20; w = 80; }
		if (shoot && (STATE == stay)) { anim.set("shoot"); h = 52; w = 45; }
		if (shoot && (STATE == walk)) { anim.set("walkAndShoot"); h = 47; w = 50; }
		if (dir) anim.flip();
		anim.tick(time);
	}

	//Движение и положение
	void update(float time, RenderWindow &window)
	{
		KeyCheck();
		AnimationS(time);

		x += dx*time;
		Collision(0);

		dy += 0.0002*time;
		y += dy*time;
		Collision(1);

		anim.draw(window, x, y);
	}

	//Проверска взаимодействия героя и окружения карты
	void Collision(int num) {
		for (int i = (y - h) / 30; i < y / 30; i++)
			for (int j = x / 30; j < (x + w) / 30; j++) {
				if (TileMap[i][j] == 'B') {
					if ((dx > 0) && (num == 0)) x = j * 30 - w;
					if ((dx < 0) && (num == 0)) x = (j + 1) * 30;
					if ((dy > 0) && (num == 1)) { y = i * 30; dy = 0; if (dx != 0) STATE = walk; else STATE = stay; }
					if ((dy < 0) && (num == 1)) { y = (i + 1) * 30 + h; dy = 0; }
				}
				if (TileMap[i][j] == '0') TileMap[i][j] = ' ';

			}
	}

};