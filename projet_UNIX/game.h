#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/WindowBase.hpp>
#include "joueur.h"
#include <unistd.h>

// Rédigé par Maxime et relu par Antoine

// Definition de la structure game contenent les tableau nécéssaire au déroulment du jeux  

struct game {
	int tab[11][11];
	int obs[7][2];
public:
	game();
	sf::Color getColor(int i);
	void affichergrille(sf::RenderWindow& window);
	void update(sf::RenderWindow& w, joueur& J1, joueur& J2, sf::Font& font);
	void afficherjoueur(sf::RenderWindow& w, joueur& J);
	void obstacle();
	bool rejouer(int win);
};
