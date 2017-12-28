#pragma once
//Класс сущностей (прочих динамических объектов, с которыми можно взаимодействовать
class Entity
{

public:
	float x, y, dx, dy, w, h = 16;
	AnimationManager anim;
	bool life, dir;
	std::string Name;

	Entity(AnimationManager &A, int X, int Y) {
		anim = A;
		x = X;
		y = Y;
		dir = 0;
		life = true;
		dx = dy = 0;
	}

	virtual void update(float time) = 0;

	void draw(RenderWindow &window) {
		anim.draw(window, x, y);
	}
};