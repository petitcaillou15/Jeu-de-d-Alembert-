#include "game.h"
#include <iostream>
#include <random>

//Rédigé par Maxime et relu par Antoine

/*
0 = blanc
1 = gris
2 = vert
3 = violet
*/


// Définition des différentes méthodes de la structure game


// Constructeur : initialise les valeur dans le tableau,  pour chaque case il y a un id qui correspond à la couleur de la case
game::game() {
	// on met toutes les cases en blancs
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			tab[i][j] = 0;
		}
	}
	// cases (11,11) et (1,1) en verts
	tab[10][10] = 2;
	tab[0][0] = 2;

	// Cases (6,6) en violet
	tab[5][5] = 3;
}

// Renvoi la bonne couleur en fonction de l'id de la case
sf::Color game::getColor(int i) {
	#define WHITE sf::Color(255, 255, 255)
	#define GRAY sf::Color(112, 128, 144)
	#define GREEN sf::Color(68,186,93)
	#define PURPLE sf::Color(86,74,148)
	#define BLACK sf::Color(0,0,0)

	switch (i) {
		case 0:
			return WHITE;
		case 1:
			return GRAY;
		case 2:
			return GREEN;
		case 3:
			return PURPLE;
		default:
			std::cerr << "Problème dans l'id de la case" << std::endl;
			return BLACK;
	}
}

void game::affichergrille(sf::RenderWindow& window) {

	#define BLACK sf::Color(0,0,0)

	int tailleSprite = round(3 * window.getSize().x/55);

	// Calcul de la position du l'image dans la grille. La grille prend 3/5 de la fenêtre 
	int xDepart = round(window.getSize().x / 5);

	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			sf::RectangleShape rectangle(sf::Vector2f(tailleSprite, tailleSprite));
			rectangle.setFillColor(getColor(tab[i][j]));
			rectangle.setOutlineColor(BLACK);
			rectangle.setOutlineThickness(1.f);
			rectangle.setPosition(xDepart + i * tailleSprite, j * tailleSprite);
			window.draw(rectangle);
		}
	}
}

// Affiche les jetons reorésentant les joueurs
void game::afficherjoueur(sf::RenderWindow& w, joueur& J) {
	#define RED sf::Color(206, 24, 24)
	#define BLUE sf::Color(82,160,224)
	// On prend comme rayon du cercle la moitier d'une case et la positionne au milieu de celle-ci
	int tailleCase = round(3 * w.getSize().x / 55);
	sf::CircleShape circle(tailleCase / 4);
	int xdepart = round(w.getSize().x / 5 + tailleCase / 4);
	int ydepart = tailleCase / 4;
	circle.setPosition(xdepart + J.x_*tailleCase, ydepart + J.y_*tailleCase);
	circle.setOutlineColor(sf::Color(0, 0, 0));
	circle.setOutlineThickness(1.f);
	if (J.id_ == 1) {
		circle.setFillColor(BLUE);
	}
	else if (J.id_ == 2) {
		circle.setFillColor(RED);
	}
	else {
		std::cerr << "erreur lors de l'affichage du joueur (id != 1 ou 2)" << std::endl;
	}
	w.draw(circle);
}

void game::update(sf::RenderWindow& w, joueur& J1, joueur& J2, sf::Font& font) {
	w.clear(sf::Color(0, 0, 0));
	this->affichergrille(w);
	J1.afficherinfo(w, font);
	J2.afficherinfo(w, font);
	this->afficherjoueur(w, J1);
	this->afficherjoueur(w, J2);
}

// Génère des obstacles de manière aléatoire et garde leur position dans un tableau
void game::obstacle() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 10); // génère un entier aléatoire entre 0 et 10
	for (int i = 0; i < 7; i++) {
		obs[i][0] = 0;
		obs[i][1] = 0;
	}

	for (int i = 0; i < 7; i++) {
		while (obs[i][0] == 0 && obs[i][1] == 0) {
			int xtemp = distrib(gen);
			int ytemp = distrib(gen);
			bool dansletableau = false;

			// On verifie que la position de l'obstacle génère n'est pas déjà dans le tableau
			for (int j = 0; j < i; j++) {
				if (xtemp == obs[j][0] && ytemp == obs[j][1]) {
					dansletableau = true;
				}
			}

			// Si la position génèrée est déjà dans le tableau ou l'une des arrivées on recommence 
			if ((xtemp == 0 && ytemp == 0) || (xtemp == 5 && ytemp == 5) || (xtemp == 10 && ytemp == 10) || dansletableau == true) {

			}
			else {
				obs[i][0] = xtemp;
				obs[i][1] = ytemp;
				tab[xtemp][ytemp] = 1;
			}
		}
	}
}

// Demande au joueur s'il veut rejouer
bool game::rejouer(int win) {

	sf::RenderWindow window(sf::VideoMode(520, 240), "Rejouer ?");
	sf::Font font;
	if (!font.loadFromFile("Res/Poppins-Regular.ttf")) {
		std::cerr << "Erreur : impossible de charger la police" << std::endl;
	}

	sf::Text textQuestion;
	if (win == 1) {
		textQuestion.setString("  Victoire du joueur 1 !\n Voulez-vous rejouer ?");
	}
	else if (win == 2) {
		textQuestion.setString("  Victoire du joueur 2 !\n Voulez-vous rejouer ?");
	}

	textQuestion.setFont(font);
	textQuestion.setFillColor(sf::Color::White);
	textQuestion.setCharacterSize(30);
	textQuestion.setOrigin(textQuestion.getGlobalBounds().getSize() / 2.f + textQuestion.getLocalBounds().getPosition());
	textQuestion.setPosition(window.getSize().x / 2, 50);

	sf::RectangleShape rectangleY({ 150.f, 80.f });
	rectangleY.setPosition(80, 120);
	rectangleY.setFillColor(sf::Color::White);

	sf::RectangleShape rectangleN({ 150.f, 80.f });
	rectangleN.setPosition(280, 120);
	rectangleN.setFillColor(sf::Color::White);

	sf::Text textY("Y", font);
	textY.setFillColor(sf::Color::Black);
	textY.setPosition(140, 140);

	sf::Text textN("N", font);
	textN.setFillColor(sf::Color::Black);
	textN.setPosition(340, 140);

	while (window.isOpen())
	{

		int mouseX = sf::Mouse::getPosition(window).x;
		int mouseY = sf::Mouse::getPosition(window).y;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (rectangleY.getGlobalBounds().contains(mouseX, mouseY)) {
				window.close();
				return true;
			}
			if (rectangleN.getGlobalBounds().contains(mouseX, mouseY)) {
				window.close();
				return false;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
			window.close();
			return true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
			window.close();
			return false;
		}

		window.clear();
		window.draw(textQuestion);
		window.draw(rectangleY);
		window.draw(rectangleN);
		window.draw(textY);
		window.draw(textN);
		window.display();
	}
	return false;
}
