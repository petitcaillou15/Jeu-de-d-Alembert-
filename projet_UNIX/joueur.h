#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <iostream>

//Rédigé par Maxime et relu par Antoine

// Création de la strucuture joueur gardant en mémoire la position et les lancés de dés
struct joueur {
	int x_;
	int y_;
	int id_;
	int xd_;
	int yd_;
public:
	joueur(int x = 0, int y = 0, int id = 0, int xd = 0, int yd = 0);
	void setx(int x) {
		x_ = x;
	}
	void sety(int y) {
		y_ = y;
	}
	void setid(int id) {
		id_ = id;
	}
	void setxd(int xd){
		xd_ = xd;
		}
	void setyd(int yd) {
		yd_ = yd;
	}
	bool estSurObstacle(int x, int y, int obstacle[7][2]);
	void throwdice(int obstacle[7][2]);
	void writepipe(int fd, int numb);
	int readpipe(int fd);
	void afficherinfo(sf::RenderWindow& w, sf::Font& font);

};