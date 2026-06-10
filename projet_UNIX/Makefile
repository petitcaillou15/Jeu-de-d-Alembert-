CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

SRC = main.cpp game.cpp joueur.cpp
OBJ = $(SRC:.cpp=.o)

all: jeu

jeu: $(OBJ)
	$(CXX) $(OBJ) -o jeu -lsfml-graphics -lsfml-window -lsfml-system

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) jeu