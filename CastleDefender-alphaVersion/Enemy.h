#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

//Противник
class Enemy : public Entity
{
public:
	int w;
	float dx;
	Enemy(AnimationManager &a, int x, int y, int W, float DX) :Entity(a, x, y)
	{
		anim.set("move");
		Name = "enemy";
		w = W;
		dx = DX;
	}

	void update(float time)
	{
		x += dx*time;

		//Collision (см. PLAYER.h)
		for (int i = (y - h) / 30; i < y / 30; i++) {
			for (int j = x / 30; j <= (x + w) / 30; j++) {
				if (TileMap[i][j] == 'B') {
					if (dx > 0) { x = j * 30 - w;	dx *= -1; }
					else { x = (j + 1) * 30;	dx *= -1; }
				}
				if (TileMap[int(i + 1)][j] == ' ') {
					dx *= -1;
				}
			}
		}
		anim.tick(time);
	}
};

#endif ENEMY_H