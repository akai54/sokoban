/*
  - Taille du grille de jeu (NBL et NBC)
  - Nombre maximum de caisses (MAX_CRATES)
  - Constantes de direction de mouvement (MOVE_U, MOVE_D, MOVE_L, MOVE_R,
  MOVE_W)
  - Constantes de position sur le grille (OUT, FREE, TARGET, WALL,
  CRATE_ON_FREE, CRATE_ON_TARGET, MAN1_ON_FREE, MAN1_ON_TARGET, MAN2_ON_FREE,
  MAN2_ON_TARGET, END_OF_LINE)
  - Représentation en chaîne du grille de jeu (board_str)
  - Représentation en chaîne des mouvements (move_str)
*/

#ifndef MYSOK_H
#define MYSOK_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <string>

// La taille du grille de jeu
#define NBL 20 // Nombre de lignes
#define NBC 20 // Nombre de colonnes

// Le nombre maximum de caisses
#define MAX_CRATES 20

// La direction de mouvement
#define MOVE_U 0
#define MOVE_D 1
#define MOVE_L 2
#define MOVE_R 3
#define MOVE_W 4

// La position sur le grille
#define OUT 0
#define FREE 1
#define TARGET 2
#define WALL 3
#define CRATE_ON_FREE 4
#define CRATE_ON_TARGET 5
#define MAN1_ON_FREE 6
#define MAN1_ON_TARGET 7
#define MAN2_ON_FREE 8
#define MAN2_ON_TARGET 9
#define END_OF_LINE 10

// Représentation en chaîne du grille de jeu
char board_str[] = {' ', '_', '.', '#', '$', '*', '1', 'u', '2', 'd', 'a'};

// Représentation en chaîne des mouvements
std::string move_str[] = {"Up", "Down", "Left", "Right", "Wait"};

// Structure sok_board_t définissant un grille de jeu Sokoban
struct sok_board_t {
  int board[NBL][NBC]; // grille de jeu
  int board_nbl;       // Nombre de lignes du grille
  int crates = 0;      // Nombres total des caisses
  int man1_x;          // Coordonnées du joueur 1 (x)
  int man1_y;          // Coordonnées du joueur 1 (y)
  int man2_x;          // Coordonnées du joueur 2 (x)
  int man2_y;          // Coordonnées du joueur 2 (y)

  // Constructeur par défaut
  sok_board_t();

  // Afficher le grille de jeu
  void print_board();

  // Charger le grille de jeu à partir d'un fichier
  void load(char *_file);
};

// Une fonction qui renvoie la valeur d'une case
int get_coords_name(sok_board_t &S, int x, int y) { return S.board[x][y]; }

bool can_move(sok_board_t &S, int x, int y) {
  if (get_coords_name(S, x, y) == OUT || get_coords_name(S, x, y) == WALL ||
      get_coords_name(S, x, y) == CRATE_ON_TARGET ||
      get_coords_name(S, x, y) == END_OF_LINE) {
    return false;
  } else if (get_coords_name(S, x, y) == CRATE_ON_FREE) {
    if (get_coords_name(S, x + 1, y) == TARGET) {
      return true;
    }
    return false;
  }
  return true;
}

void move_man(sok_board_t &S, int direction) {
  switch (direction) {
  case MOVE_U:
    if (can_move(S, S.man1_x - 1, S.man1_y) == false) {
      std::cout << "Can't go up" << std::endl;
      break;
    }
    S.board[S.man1_x][S.man1_y] = FREE;
    S.man1_x -= 1;
    S.board[S.man1_x][S.man1_y] = MAN1_ON_FREE;
    break;
  case MOVE_D:
    if (can_move(S, S.man1_x + 1, S.man1_y) == false) {
      std::cout << "Can't go down" << std::endl;

      break;
    }
    // Si la prochaine case est une caisse alors on la pousse
    if (get_coords_name(S, S.man1_x + 1, S.man1_y) == CRATE_ON_FREE) {
      S.board[S.man1_x + 1][S.man1_y] = FREE;
      S.board[S.man1_x + 2][S.man1_y] = CRATE_ON_TARGET;
      S.crates--;
    }
    S.board[S.man1_x][S.man1_y] = FREE;

    S.man1_x += 1;
    S.board[S.man1_x][S.man1_y] = MAN1_ON_FREE;
    break;
  case MOVE_L:
    if (can_move(S, S.man1_x, S.man1_y - 1) == false) {
      std::cout << "Can't go left" << std::endl;
      break;
    }
    std::cout << "trying to move left" << std::endl;
    S.board[S.man1_x][S.man1_y] = FREE;
    S.man1_y -= 1;
    S.board[S.man1_x][S.man1_y] = MAN1_ON_FREE;
    break;
  case MOVE_R:
    if (can_move(S, S.man1_x, S.man1_y + 1) == false) {
      std::cout << "Can't go right" << std::endl;
      break;
    }
    S.board[S.man1_x][S.man1_y] = FREE;
    S.man1_y += 1;
    S.board[S.man1_x][S.man1_y] = MAN1_ON_FREE;
    break;
  }
}

// Constructeur par défaut
sok_board_t::sok_board_t() {
  // Initialiser le grille avec des cases libres
  for (int i = 0; i < NBL; i++)
    for (int j = 0; j < NBC; j++)
      board[i][j] = FREE;
}

// Afficher le grille de jeu
void sok_board_t::print_board() {
  for (int i = 0; i < board_nbl; i++) {
    for (int j = 0; j < NBC; j++) {
      if (board[i][j] == END_OF_LINE) // Si fin, sortir de la boucle
        break;
      // Sinon afficher la case actuelle
      printf("%c", board_str[board[i][j]]);
    }
    printf("\n");
  }
}

// Charge la grille à partir d'un fichier
void sok_board_t::load(char *_file) {
  FILE *fp = fopen(_file, "r"); // Ouvre le fichier en mode lecture
  char *line = NULL;            // Ligne lue du fichier
  size_t len = 0;               // Longueur de la ligne
  ssize_t nread;                // Nombre d'octets lus

  // Vérifie si le fichier a pu être ouvert
  if (fp == NULL) {
    perror("fopen");    // Affiche une erreur
    exit(EXIT_FAILURE); // Quitte le programme
  }

  board_nbl = 0;
  // Boucle pour lire chaque ligne dans le fichier
  while ((nread = getline(&line, &len, fp)) != -1) {
    // Vérifie si la ligne a été lue avec succès
    if (((int)nread) > 0) {
      bool read_ok = false;
      // Boucle pour parcourir chaque caractère de la ligne
      for (int i = 0; i < nread; i++) {
        if (line[i] == board_str[FREE]) {
          board[board_nbl][i] = FREE;
        } else if (line[i] == board_str[TARGET]) {
          board[board_nbl][i] = TARGET;
        } else if (line[i] == board_str[WALL]) {
          read_ok = true;
          board[board_nbl][i] = WALL;
        } else if (line[i] == board_str[CRATE_ON_FREE]) {
          crates++;
          board[board_nbl][i] = CRATE_ON_FREE;
        } else if (line[i] == board_str[CRATE_ON_TARGET]) {
          board[board_nbl][i] = CRATE_ON_TARGET;
        } else if (line[i] == board_str[MAN1_ON_FREE]) {
          man1_x = board_nbl;
          man1_y = i;
          board[board_nbl][i] = MAN1_ON_FREE;
        } else if (line[i] == board_str[MAN1_ON_TARGET]) {
          man1_x = board_nbl;
          man1_y = i;
          board[board_nbl][i] = MAN1_ON_TARGET;
        } else if (line[i] == board_str[MAN2_ON_FREE]) {
          man2_x = board_nbl;
          man2_y = i;
          board[board_nbl][i] = MAN2_ON_FREE;
        } else if (line[i] == board_str[MAN2_ON_TARGET]) {
          man2_x = board_nbl;
          man2_y = i;
          board[board_nbl][i] = MAN2_ON_TARGET;
        }
      }
      if (read_ok) {
        board[board_nbl][nread - 1] = END_OF_LINE;
        board_nbl++;
      }
    }
  }
  free(line);
  fclose(fp);
}

#endif
