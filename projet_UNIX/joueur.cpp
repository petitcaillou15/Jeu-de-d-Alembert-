#include <iostream>
#include <random>
#include <unistd.h>
#include<string>
#include "joueur.h"

//Rédigé par Maxime et relu par Antoine

// Constructur 
joueur::joueur(int x, int y, int id, int xd, int yd) {
	x_ = x;
	y_ = y;
	id_ = id;
	xd_ = xd;
	yd_ = yd;
}

// Vérifie si le joueur est sur un obstacle ou non 
bool joueur::estSurObstacle(int x, int y, int obstacle[7][2]) {
	for (int i = 0; i < 7; i++) {
		if (x == obstacle[i][0] && y == obstacle[i][1]) {
			return true;
		}
	}
	return false;
}

// Lance les dés et change les position des joueurs
void joueur::throwdice(int obstacle[7][2]) {
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(-2, 2); // Génère un entier aléatoire entre -2 et 2

	int xdtemp = distrib(gen);
	int ydtemp = distrib(gen);
	int xtemp = x_ + xdtemp;
	int ytemp = y_ - ydtemp; // Sur SFML l'axe des ordonnées augmente vers le bas


	if (xtemp > 10 || xtemp < 0 || ytemp > 10 || ytemp < 0) { // On ne bouge pas si on sort de la grille mais on garde quand même les valeur des dés
		setxd(xdtemp);
		setyd(ydtemp);
	}
	else if (estSurObstacle(xtemp, ytemp, obstacle) == true){ // Si on va arriver sur un obstacle on revient au centre
		setxd(xdtemp);
		setyd(ydtemp);
		setx(5);
		sety(5);
	}
	else {
		// affectation de lancé de dés dans les variables
		setxd(xdtemp);
		setyd(ydtemp);

		// Déplacement du pion en fonction du résultat lancé de dés
		setx(xtemp);
		sety(ytemp);
	}
		
}

void joueur::writepipe(int fd, int numb) {
	if (write(fd, &numb, sizeof(numb)) == -1) {
		std::cout << "erreur lors de l'écriture dansl le tube" << std::endl;
	};
}

int joueur::readpipe(int fd) {
	int y;
	if (read(fd, &y, sizeof(y)) == -1) {
		std::cout << "erreur lors de la lecture dans le tube" << std::endl;
	};
	return y;
}

// Affiche les information des joueur (nom, valeurs des lancés de dés)
void joueur::afficherinfo(sf::RenderWindow& w, sf::Font& font ){
	#define RED sf::Color(206, 24, 24)
	#define BLUE sf::Color(82,160,224)
	#define WHITE sf::Color(255, 255, 255)
	
	// Affichage de l'interface et des lancés de dés des joueur en fonction de leur id
	if (id_ == 1) {
		sf::Text txtJ1;
		txtJ1.setString("Joueur 1");
		txtJ1.setFont(font);
		txtJ1.setCharacterSize(30);
		txtJ1.setFillColor(BLUE);
		txtJ1.setStyle(sf::Text::Bold);
		int x = round(w.getSize().x / 25);
		int y = round(w.getSize().y / 6);
		txtJ1.setPosition(x, y);
		w.draw(txtJ1);

		sf::Text txtde;
		txtde.setString("Lance de des");
		txtde.setFont(font);
		txtde.setCharacterSize(20);
		txtde.setFillColor(WHITE);
		txtde.setStyle(sf::Text::Bold);
		int yd = round(w.getSize().y / 2);
		txtde.setPosition(x, yd);
		w.draw(txtde);

		sf::Text txtvalde;
		txtvalde.setString(std::to_string(xd_) + " | " + std::to_string(yd_));
		txtvalde.setFont(font);
		txtvalde.setCharacterSize(20);
		txtvalde.setFillColor(WHITE);
		txtvalde.setStyle(sf::Text::Bold);
		int xvd = round(w.getSize().x / 15);
		int yvd = round(w.getSize().y*3/5 );
		txtvalde.setPosition(xvd, yvd);
		w.draw(txtvalde);
	}
	else if (id_ == 2)  {
		sf::Text txtJ2;
		txtJ2.setString("Joueur 2");
		txtJ2.setFont(font);
		txtJ2.setCharacterSize(30);
		txtJ2.setFillColor(RED);
		txtJ2.setStyle(sf::Text::Bold);
		int x = round(w.getSize().x*105/ 125);
		int y = round(w.getSize().y / 6);
		txtJ2.setPosition(x, y);
		w.draw(txtJ2);

		sf::Text txtde;
		txtde.setString("Lance de des");
		txtde.setFont(font);
		txtde.setCharacterSize(20);
		txtde.setFillColor(WHITE);
		txtde.setStyle(sf::Text::Bold);
		int yd = round(w.getSize().y / 2);
		txtde.setPosition(x, yd);
		w.draw(txtde);

		sf::Text txtvalde;
		txtvalde.setString(std::to_string(xd_) + " | " + std::to_string(yd_));
		txtvalde.setFont(font);
		txtvalde.setCharacterSize(20);
		txtvalde.setFillColor(WHITE);
		txtvalde.setStyle(sf::Text::Bold);
		int xvd = round(w.getSize().x*65/75);
		int yvd = round(w.getSize().y * 3/5);
		txtvalde.setPosition(xvd, yvd);
		w.draw(txtvalde);
	}
	else {
		std::cerr << "erreur lors de l'affichage du joueur (id != 1 ou 2)" << std::endl;
	}
}



