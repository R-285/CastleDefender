#include <SFML\Audio.hpp>
#include "Player.h"
#include "Animation.h"
#include "Entity.h"
#include "Bullet.h"
#include "Enemy.h"
#include <Windows.h>
#include <list>	

//��������� ������������ �������� ����� � �����
bool Collision(PLAYER Hero, Entity *it) {
	bool touch = false;
	Entity *b = it;
	for (int iH = Hero.x; iH <= Hero.x + Hero.w; iH++)
		for (int jH = Hero.y - Hero.h; jH <= Hero.y; jH++)
			for (int iE = b->x; iE <= b->x + b->w; iE++)
				for (int jE = b->y; jE <= b->y + b->h; jE++) {
					if ((iH == iE) && (jH == jE))
						touch = true;
				}
	return touch;
}

//��������� ������������ ���� � �����
bool Collisions(Entity *Heros, Entity *it) {
	bool touch = false;
	Entity *b = it;
	Entity *Hero = Heros;
	for (int iH = Hero->x; iH <= Hero->x + Hero->w; iH++)
		for (int jH = Hero->y; jH <= Hero->y + Hero->h + 7; jH++)
			for (int iE = b->x; iE <= b->x + b->w; iE++)
				for (int jE = b->y; jE <= b->y + b->h; jE++) {
					if ((iH == iE) && (jH == jE))
						touch = true;
				}
	return touch;
}

//����
void menu(RenderWindow &window) {
	Music menuMusic;  //������ ����
	menuMusic.openFromFile("files\\menuMusic.ogg");
	menuMusic.play();

	SoundBuffer bufferSelectionMenu;  //���� ������ � ����
	bufferSelectionMenu.loadFromFile("files\\coins.ogg");
	Sound soundSelectionMenu(bufferSelectionMenu);

	//����
	Texture menu_texture1, menu_texture2, menu_texture3, about_texture;  //��������� ����
	menu_texture1.loadFromFile("files\\111.png");
	menu_texture2.loadFromFile("files\\222.png");
	menu_texture3.loadFromFile("files\\333.png");
	about_texture.loadFromFile("files\\About.png");
	Sprite menu1(menu_texture1), menu2(menu_texture2), menu3(menu_texture3), about(about_texture);

	bool Menu = 1;
	signed char cursorOnButton = 0;
	int MenuNum;

	//����������� ���� �������� � ����
	menu1.setPosition(100, 30);
	menu2.setPosition(100, 90);
	menu3.setPosition(100, 150);

	while (Menu)
	{
		//���� ������ - �����
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);

		MenuNum = 0;
		window.clear(Color::Black);  //������������ ���� (���� �����������)

									 //���� � ��� ������ ��������� �� �����-�� ������, �� ��� �������������� ������
		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) {
			menu1.setColor(Color::Yellow);
			MenuNum = 1;
			if (cursorOnButton == 0) cursorOnButton = 1;
		}
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) {
			menu2.setColor(Color::Yellow);
			MenuNum = 2;
			if (cursorOnButton == 0) cursorOnButton = 1;
		}
		if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) {
			menu3.setColor(Color::Yellow);
			MenuNum = 3;
			if (cursorOnButton == 0) cursorOnButton = 1;
		}

		//������� �������, ��� �� �� �� ������, ���� �� ������������� �� �� ���.
		if (!IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window)) &&
			!IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window)) &&
			!IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window)))  cursorOnButton = 0;

		//������������ ������� ����� ������� ���� �� ������
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (MenuNum == 1) Menu = false;
			if (MenuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (MenuNum == 3) { window.close(); Menu = false; }
		}

		if (cursorOnButton == 1) { cursorOnButton = 2; soundSelectionMenu.play(); }

		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}

	menuMusic.stop();
}


int main() {
	//������ ����
	RenderWindow window(VideoMode(630, 360), "Game");   //������� ����� ��� ����
	Clock clock;

	//��
	Texture t, bullet_t, enemy_t;
	t.loadFromFile("files\\fang.png"); //��������� ���������
	bullet_t.loadFromFile("files\\bullet.png");
	enemy_t.loadFromFile("files\\Mario_tileset.png");
	AnimationManager anim;
	anim.create("walk", t, 0, 244, 40, 50, 6, 0.005, 40); //� ������ �� ���������� � ���� ������ ������ AnimationManager
	anim.create("jump", t, 0, 528, 29, 30, 4, 0.0045, 38);
	anim.create("duck", t, 0, 436, 80, 20, 1, 0.005, 0);
	anim.create("stay", t, 3, 185, 38, 52, 3, 0.002, 45);
	anim.create("shoot", t, 3, 576, 44, 47, 4, 0.004, 44);
	anim.create("walkAndShoot", t, 4, 901, 50, 47, 5, 0.0045, 50);
	PLAYER Hero(anim, 30, 325);   //������� ����� (����� �������� ��� ����������, ����������� �, ���������� Y)

								  //�������� ����
	AnimationManager anim2;
	anim2.create("move", bullet_t, 8, 10, 6, 6, 1, 0, 0);
	anim2.create("explode", bullet_t, 27, 6, 16, 16, 3, 0.005, 16, false);

	//�������� �����
	AnimationManager anim3;
	anim3.create("move", enemy_t, 0, 0, 16, 16, 2, 0.002, 19);
	anim3.create("explode", enemy_t, 59, 8, 16, 8, 1, 0, 0, false);


	//������ ��� �������� ������������ ��������
	std::list<Entity*> entities;
	std::list<Entity*>::iterator it;

	//��������� ��������� ����������� (��������, ��������� X, ���������� Y, ������ ��������, �������� ������������)
	entities.push_back(new Enemy(anim3, 552, 239, 16, 0.04));
	entities.push_back(new Enemy(anim3, 40, 239, 16, 0.04));
	entities.push_back(new Enemy(anim3, 550, 329, 16, 0.04));
	entities.push_back(new Enemy(anim3, 30, 89, 16, 0.04));
	entities.push_back(new Enemy(anim3, 210, 149, 16, 0.04));


	//��������� ��� ������ � ��� �����
	Music music;  //������ ����
	music.openFromFile("files\\music.ogg");

	SoundBuffer bufferGameOver;  //���� ������ �� ����
	bufferGameOver.loadFromFile("files\\GameOver.ogg");
	Sound soundGameOver(bufferGameOver);
	SoundBuffer bufferJump;  //���� ������
	bufferJump.loadFromFile("files\\Jump.ogg");
	Sound soundJump(bufferJump);
	SoundBuffer bufferShot;  //���� ������ �� ����
	bufferShot.loadFromFile("files\\Shot.ogg");
	Sound soundShot(bufferShot);

	//��������� ���� � �������� ������ ������� ����� ����
	menu(window);
	music.play();


	//�������� ����� ��� � ��� �������
	Texture texturForeGraund, texturDoor, texturBackGraund, texturChandelier, texturShield, texturChest;
	texturForeGraund.loadFromFile("files\\ForeGraund1.png");
	texturDoor.loadFromFile("files\\door.png");
	texturBackGraund.loadFromFile("files\\BackGraund.png");
	texturChandelier.loadFromFile("files\\Chandelier.png");
	texturShield.loadFromFile("files\\Shield.png");
	texturChest.loadFromFile("files\\Chest.png");

	Sprite sWall1(texturForeGraund), sWall2(texturBackGraund), sDoor(texturDoor), sFlag(texturDoor), sChandelier(texturChandelier),
		sShield(texturShield), sChest(texturChest);
	sWall1.setTextureRect(IntRect(0, 0, 30, 30));
	sWall2.setTextureRect(IntRect(0, 0, 30, 30));
	sChest.setTextureRect(IntRect(0, 0, 30, 30));
	sDoor.setTextureRect(IntRect(199, 47, 50, 81));
	sFlag.setTextureRect(IntRect(19, 199, 90, 41));
	sChandelier.setTextureRect(IntRect(0, 0, 56, 44));
	sShield.setTextureRect(IntRect(0, 0, 64, 69));

	float time;

	//��� ������� �������
	while (window.isOpen()) {

		//��������� "����" ����
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 500;  // ����� ���������� �������� ����
		if (time > 40) time = 40;

		//�������� ������
		window.clear(Color::White);

		//��������� �������� ����
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}

		//��������� ������� ������
		if (Keyboard::isKeyPressed(Keyboard::Left)) Hero.key["Left"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Right)) Hero.key["Right"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Down)) Hero.key["Down"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			soundJump.play(); Hero.key["Space"] = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::F)) {
			Hero.key["F"] = true;
			if ((soundShot.getPlayingOffset().asMicroseconds()) < 0.6) {
				soundShot.play();
				if (Hero.dir) entities.push_back(new Bullet(anim2, Hero.x, Hero.y - (Hero.h*0.68), Hero.dir));
				else entities.push_back(new Bullet(anim2, Hero.x + Hero.w, Hero.y - (Hero.h*0.68), Hero.dir));
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			music.stop();
			soundGameOver.play();
			Sleep(time * 1400);
			window.close();
		}

		//��� ��� ������������ ��������
		for (it = entities.begin(); it != entities.end();) {
			Entity *b = *it;
			b->update(time);
			if (b->life == false) { it = entities.erase(it); delete b; }
			else it++;
		}

		//�������������� ������������ ��������
		for (it = entities.begin(); it != entities.end(); it++) {
			if ((*it)->Name == "enemy") //��������������� ������ �������������� ����� � ���-����
			{
				Entity *enemy = *it;

				//1). � ������� ������
				if (Collision(Hero, enemy)) {
					if (Hero.dy > 0) {
						Hero.dy = -0.1;
						enemy->life = false;
					}
					else {
						music.stop();
						soundGameOver.play();
						Sleep(time * 1400);
						window.close();
					}
				}

				//2). � ����� �� ���������
				for (std::list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)
				{
					Entity *bullet = *it2;
					if (bullet->Name == "bullet")
						if (Collisions(bullet, enemy)) {
							bullet->life = false; enemy->life = false;
						}
				}
			}
		}

		/////////////////////////////////�������/////////////////////////////////////
		//���������� ������ ����		
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (TileMap[i][j] == 'B') {
					sWall1.setPosition(j * 30, i * 30);
					window.draw(sWall1);
					continue;
				}
				if (TileMap[i][j] == ' ') {
					sWall2.setPosition(j * 30, i * 30);
					window.draw(sWall2);
					continue;
				}

				if (TileMap[i][j] == '0') {
					sChest;
					sChest.setPosition(j * 30, i * 30);
					window.draw(sChest);
					continue;
				}
			}
		}

		//������ ������� ����
		sDoor.setPosition(200, 250);
		window.draw(sDoor);

		sFlag.setPosition(130, 75);
		window.draw(sFlag);

		sFlag.setPosition(430, 75);
		window.draw(sFlag);

		sChandelier.setPosition(430, 200);
		window.draw(sChandelier);

		sChandelier.setPosition(60, 140);
		window.draw(sChandelier);

		sShield.setPosition(280, 140);
		window.draw(sShield);

		//��� ��� ����� � ��� ����������
		Hero.update(time, window);

		//��� ��� ������������ �������� � �� ����������
		for (it = entities.begin(); it != entities.end(); it++)
			(*it)->draw(window);

		//���������� �� �����
		window.display();
	}

	return 0;
}