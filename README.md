# Jeu de d'Alembert

## Règle du jeu

Ce projet consiste à simuler une ou plusieurs partie du jeux de d’Alembert à l’aide de processus et de tubes. Les règles du jeux sont :
    • Une grille de dimension 11x11
    • Les joueurs commencent au centre de la grille (6,6)
    • 7 obstacles sont générés aléatoirement au début de chaque partie 
    • Lorsqu’un joueur arrive sur un obstacle, il revient au milieu
    • Lorsqu’un joueur arrive une case déjà occupé par l'adversaire, l'adversaire revient au milieu
    • Lorsqu’un joueur essaient de quitter la grille, il passe son tour
    • Le but est d’atteindre la case (1,1) ou (11, 11)
    • Lors de leur tour les joueurs tirent deux chiffres au hasard entre -2 et 2 représentent leur déplacement vertical et horizontal.

## Participant

Seuls deux robots peuvent participer à ce jeu car ce n'est qu'un enchaînement de lancé de dés aléatoire. Les deux joueurs sont créés à l'aide de deux "fork" 
et communiquent leur position (au père et à l'autre joueur) grâce à un tube.

## Amélioration

Laisser chaque joueur décider quel dé régit quel déplacement (horizontal ou vertical) aurait donné une dimension plus stratégique au jeu 
et l'aurait rendu jouable un joueur humain.

## Conclusion 

Ce projet a renforcé ma capacité à travailler en équipe et permis d'expérimenter la programmation système. De plus, le frontend a été fait avec SFML qui n'est pas "fork safe"
ce qui a compliqué l'avancé du projet mais cela m'a permis de rencontrer des problèmes et situation dont je n'avais pas l'habitude.
