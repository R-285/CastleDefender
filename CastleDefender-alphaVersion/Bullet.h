#ifndef BULLET_H
#define BULLET_H
#include "Entity.h"

//Класс пуль, наследние от класса прочих динамических объектов (сущностей)
class Bullet :public Entity
{
public:


	Bullet(AnimationManager &a, int x, int y, bool dir) : Entity(a, x, y) {
		anim.set("move");
		Name = "bullet";
		if (dir) {
			dx = -0.3;
		}
		else {
			dx = 0.3;
		}
		w = h = 6;
		life = true;
	}

	//Обработка передвижения пули и столкновения её с объектом
	void update(float time) {
		x += dx*time;

		for (int i = (y / 30); i < ((y + h) / 30); i++)
			for (int j = (x / 30); j < ((x + w) / 30); j++)
				if (TileMap[i][j] == 'B') {
					anim.set("explode");
					dx = 0;
					life = false;
				}
		anim.tick(time);
	}
};
#endif BULLET_H