#include <SFML/Graphics.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "joueur.h"
#include "game.h"

// Rédidgé principalement par Antoine, relu par Maxime

//Organisation de la logique du jeu tour par tour dans les fils
//Affichage, gestion des scores et de la fin de partie (rejouer?) dans le père

int main(int argc, char* argv[])
{
    // Récupère les scores si on rejoue
    //(plus d'explications sur pourquoi passer les scores en arguments du main dans le rapport)
    int score1 = (argc >= 3) ? atoi(argv[1]) : 0; 
    int score2 = (argc >= 3) ? atoi(argv[2]) : 0;
    // S'il y a au moins trois arguments (nom du programme et les deux scores), on prends les arguments comme score
    // 0 sinon
   
    game game; // Création du plateau de jeu
    game.obstacle(); // Création du tableau d'obsctacles

    // Créations des descripteurs de fichiers pour les tubes
    int j1_to_j2[2];
    int j2_to_j1[2];
    int j1_to_p[2];
    int j2_to_p[2];

    //Création des tubes
    pipe(j1_to_j2);
    pipe(j2_to_j1);
    pipe(j1_to_p);
    pipe(j2_to_p);

    // Premier fork -> on a père et joueur 1 (le premier fils)
    pid_t joueur1 = fork();

    if (joueur1 == 0) {
        // JOUEUR 1

        // Fermeture des descripteurs non utilisés par le joueur 1
        close(j1_to_j2[0]);
        close(j2_to_j1[1]);
        close(j1_to_p[0]);
        close(j2_to_p[0]);
        close(j2_to_p[1]);

        //Initialisation
        joueur j1(5, 5, 1);
        int xj2 = 5, yj2 = 5, win = 0;
        sleep(1);

        //Premier tour
        j1.throwdice(game.obs);

        //Envoi des infos au joueur 2
        j1.writepipe(j1_to_j2[1], win);
        j1.writepipe(j1_to_j2[1], j1.x_);
        j1.writepipe(j1_to_j2[1], j1.y_);

        //Envoi des infos au père pour l'affichage
        j1.writepipe(j1_to_p[1], j1.xd_);
        j1.writepipe(j1_to_p[1], j1.yd_);
        j1.writepipe(j1_to_p[1], j1.x_);
        j1.writepipe(j1_to_p[1], j1.y_);
        j1.writepipe(j1_to_p[1], 0);

        while (true) {

            // On vérifie si le joueur 2 a gagné et on récupère ses coordonnées
            win = j1.readpipe(j2_to_j1[0]);
            xj2 = j1.readpipe(j2_to_j1[0]);
            yj2 = j1.readpipe(j2_to_j1[0]);

            if (win == 2) {
                break;
            }
             
            //On vérifie si on commence le tour sur la même case que le joueur 2
            if ((xj2 == j1.x_ && yj2 == j1.y_) && !(xj2 == 5 && yj2 == 5)) {
                // On retourne au centre
                j1.setx(5);
                j1.sety(5);

                j1.writepipe(j1_to_p[1], j1.xd_);
                j1.writepipe(j1_to_p[1], j1.yd_);

                //On envoie un message spécial au père pour le prévenir que le joueur 1 revient au centre
                //Mais qu'il va rejouer immédiatement après
                j1.writepipe(j1_to_p[1], -1);
                j1.writepipe(j1_to_p[1], -1);
                j1.writepipe(j1_to_p[1], win);
            }

            j1.throwdice(game.obs);

            // On vérifie si le joueur 1 a gagné
            if ((j1.x_ == 0 && j1.y_ == 0) || (j1.x_ == 10 && j1.y_ == 10)) {
                win = 1;
            }

            //Envoi des infos au joueur 2
            j1.writepipe(j1_to_j2[1], win);
            j1.writepipe(j1_to_j2[1], j1.x_);
            j1.writepipe(j1_to_j2[1], j1.y_);

            //Envoi des infos au père
            j1.writepipe(j1_to_p[1], j1.xd_);
            j1.writepipe(j1_to_p[1], j1.yd_);
            j1.writepipe(j1_to_p[1], j1.x_);
            j1.writepipe(j1_to_p[1], j1.y_);
            j1.writepipe(j1_to_p[1], win);

            if (win == 1) {
                break;;
            }
            //Fin du tour
        }
        // Si le joueur 1 ou 2 gagne, on termine le processus
        exit(0); 
    }
    else {
        //fork depuis le père -> On a le père et deux fils (les joueurs) de même niveau
        pid_t joueur2 = fork();

        if (joueur2 == 0) {
            // JOUEUR 2

            // Fermeture des descripteurs non utilisés par le joueur 2
            close(j2_to_j1[0]);
            close(j1_to_j2[1]);
            close(j2_to_p[0]);
            close(j1_to_p[0]);
            close(j1_to_p[1]);

            //Initialisation
            joueur j2(5, 5, 2);
            int xj1 = 5, yj1 = 5, win = 0;

            while (true) {

                // On vérifie si le joueur 1 a gagné et on récupère ses coordonnées
                win = j2.readpipe(j1_to_j2[0]);
                xj1 = j2.readpipe(j1_to_j2[0]);
                yj1 = j2.readpipe(j1_to_j2[0]);

                if (win == 1) {
                    break;
                }
                //On vérifie si on commence le tour sur la même case que le joueur 1
                if ((xj1 == j2.x_ && yj1 == j2.y_) && !(xj1 == 5 && yj1 == 5)) {
                    // On retourne au centre
                    j2.setx(5);
                    j2.sety(5);
                    //Envoi du message spécial au père
                    j2.writepipe(j2_to_p[1], j2.xd_);
                    j2.writepipe(j2_to_p[1], j2.yd_);
                    j2.writepipe(j2_to_p[1], -1);
                    j2.writepipe(j2_to_p[1], -1);
                    j2.writepipe(j2_to_p[1], win);
                }

                j2.throwdice(game.obs);
                // On vérifie si le joueur 1 a gagné
                if ((j2.x_ == 0 && j2.y_ == 0) || (j2.x_ == 10 && j2.y_ == 10)) {
                    win = 2;
                }

                //Envoie des infos au joueur 1
                j2.writepipe(j2_to_j1[1], win);
                j2.writepipe(j2_to_j1[1], j2.x_);
                j2.writepipe(j2_to_j1[1], j2.y_);

                //Envoi des infos au père (affichage)
                j2.writepipe(j2_to_p[1], j2.xd_);
                j2.writepipe(j2_to_p[1], j2.yd_);
                j2.writepipe(j2_to_p[1], j2.x_);
                j2.writepipe(j2_to_p[1], j2.y_);
                j2.writepipe(j2_to_p[1], win);

                if (win == 2) {
                    break;
                }

            }
            //Si le joueur 1 ou 2 gagne, on termine le processus
            exit(0);
        }
        else {
            //PERE

            // Fermeture des descripteurs non utilisés par le père
            close(j1_to_j2[0]);
            close(j1_to_j2[1]);
            close(j2_to_j1[0]);
            close(j2_to_j1[1]);
            close(j1_to_p[1]);
            close(j2_to_p[1]);

            // Création de la fenêtre
            sf::RenderWindow window(sf::VideoMode(1650, 990), "Jeu de d'Alembert");

            // Charge une police et créer une variable txt de type text
            sf::Font font;
            if (!font.loadFromFile("Res/Poppins-Regular.ttf")) {
                std::cerr << "Erreur : impossible de charger la police" << std::endl;
                return -1;
            }
            
            //On créé des clones de joueur 1 et 2 afin de stocker leurs infomartion nécessaire à l'affichage
            joueur j1(5, 5, 1);
            joueur j2(5, 5, 2);

            //Première frame
            game.update(window, j1, j2,font);
            window.display();

            int xj1;
            int yj1;
            int xj2;
            int yj2;
            int xd1;
            int xd2;
            int yd1;
            int yd2;
            int win = 0;

            // Temps entre deux tours (µsec)
            int pause = 400000;


            while (window.isOpen())
            {
                // Fermer la fenêtre
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }

                //Tour du joueur 1

                // Récupération des infos du joueur 1
                xd1 = j1.readpipe(j1_to_p[0]);
                yd1 = j1.readpipe(j1_to_p[0]);
                xj1 = j1.readpipe(j1_to_p[0]);
                yj1 = j1.readpipe(j1_to_p[0]);
                win = j1.readpipe(j1_to_p[0]);

                //Gestion de la collision 
                if (xj1 == -1 && yj1 == -1) {
                    //Mise à jour de l'affichage (retour au centre)
                    j1.setx(5);
                    j1.sety(5);
                    game.update(window, j1, j2,font);
                    window.display();
                    usleep(pause);

                    // Nouvelles infos post collision
                    xd1 = j1.readpipe(j1_to_p[0]);
                    yd1 = j1.readpipe(j1_to_p[0]);
                    xj1 = j1.readpipe(j1_to_p[0]);
                    yj1 = j1.readpipe(j1_to_p[0]);
                    win = j1.readpipe(j1_to_p[0]);
                }
                // Mise à jour du clone pour l'affichage
                j1.setxd(xd1);
                j1.setyd(yd1);
                j1.setx(xj1);
                j1.sety(yj1);
                game.update(window, j1, j2,font);
                window.display();
                usleep(pause);

                //Gestion de la fin de partie
                if (win == 1 || win == 2) {

                    //Incrémentaion des scores
                    if (win == 1) score1++;
                    if (win == 2) score2++;

                    //Mise à jour de l'historique des scores
                    FILE* f = fopen("historique_des_scores.txt", "a");
                    if (score1 + score2 == 1) {
                        fprintf(f, "-------------\n"); 
                    }
                    fprintf(f, "J1: %d | J2: %d\n", score1, score2);
                    fclose(f);

                    //On vérifie que les joueurs aient bien terminé
                    waitpid(joueur1, NULL, 0);
                    waitpid(joueur2, NULL, 0);

                    // Fenêtre pour demander si l'on veut rejouer
                    bool rejouer = game.rejouer(win);
                    window.close();

                    // On traduit les scores (entiers) en arguments (caractères) pour la prochaine partie
                    if (rejouer) {
                        char s1[10], s2[10];
                        sprintf(s1, "%d", score1);
                        sprintf(s2, "%d", score2);
                        char* args[] = { argv[0], s1, s2, NULL };
                        execv("/proc/self/exe", args);
                    }
                    break;
                }

                //Tour du joueur 2

                //Récupération des infos du joueur 2
                xd2 = j2.readpipe(j2_to_p[0]);
                yd2 = j2.readpipe(j2_to_p[0]);
                xj2 = j2.readpipe(j2_to_p[0]);
                yj2 = j2.readpipe(j2_to_p[0]);
                win = j2.readpipe(j2_to_p[0]);

                //Gestion des collisions 
                if (xj2 == -1 && yj2 == -1) {
                    //Mise à jour de l'affichage (retour au centre)
                    j2.setx(5);
                    j2.sety(5);
                    game.update(window, j1, j2,font);
                    window.display();
                    usleep(pause);

                    // Nouvelles infos post collision
                    xd2 = j2.readpipe(j2_to_p[0]);
                    yd2 = j2.readpipe(j2_to_p[0]);
                    xj2 = j2.readpipe(j2_to_p[0]);
                    yj2 = j2.readpipe(j2_to_p[0]);
                    win = j2.readpipe(j2_to_p[0]);
                }

                //Mise à jour du clone pour l'affichage
                j2.setxd(xd2);
                j2.setyd(yd2);
                j2.setx(xj2);
                j2.sety(yj2);
                game.update(window, j1, j2,font);
                window.display();
                usleep(pause);

                //Gestion fin de partie
                if (win == 1 || win == 2) {
                    //Incrémentation des scores
                    if (win == 1) score1++;
                    if (win == 2) score2++;
                    //Mise à jour de l'historique des scores
                    FILE* f = fopen("historique_des_scores.txt", "a");
                    if (score1 + score2 == 1) {
                        fprintf(f, "-------------\n");
                    } 
                    fprintf(f, "J1: %d | J2: %d\n", score1, score2);
                    fclose(f);

                    // Fenêtre pour demander si l'on veut rejouer
                    bool rejouer = game.rejouer(win);
                    window.close();
                    
                    // On traduit les scores (entiers) en arguments (caractères) pour la prochaine partie
                    if (rejouer) {
                        char s1[10], s2[10];
                        sprintf(s1, "%d", score1);
                        sprintf(s2, "%d", score2);
                        char* args[] = { argv[0], s1, s2, NULL };
                        execv("/proc/self/exe", args);
                    }
                    break;
                }
            }
        }
    }
    
    return 0;
}