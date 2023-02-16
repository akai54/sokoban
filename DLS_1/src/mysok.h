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
#include <stack>
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

// Profondeur max pour DLS
#define MAX_DEPTH 25

// Représentation en chaîne du grille de jeu
char board_str[] = {' ', '_', '.', '#', '$', '*', '1', 'u', '2', 'd', 'a'};

// Représentation en chaîne des mouvements
std::string move_str[] = {"Up", "Down", "Left", "Right", "Wait"};

// Stocker les positions du joueur actuel
// Obligatoire si on veut utiliser undo_move
struct State {
  int man_x;
  int man_y;
  int nbr_crate;
  int board[NBL][NBC];
  bool crate_pushed;
};

std::stack<State> states;

// Structure sok_board_t définissant un grille de jeu Sokoban
struct sok_board_t {
  int board[NBL][NBC]; // grille de jeu
  int board_nbl;       // Nombre de lignes du grille
  int nbr_crates = 0;  // Nombres total des caisses
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

// Une structure qui va sauvegarder les coords des caisses.
/* struct Crate {
  int x, y;
}; */

// Une liste dynamique qui va stocker les coords
// Envoyer depuis la structure Crate
// Crate *crates = nullptr;

// Une fonction qui renvoie la valeur d'une case
int get_coords_name(sok_board_t &S, int x, int y) { return S.board[x][y]; }

bool can_move(sok_board_t &S, int x, int y) {
  if (get_coords_name(S, x, y) == OUT || get_coords_name(S, x, y) == WALL ||
      get_coords_name(S, x, y) == CRATE_ON_TARGET ||
      get_coords_name(S, x, y) == TARGET ||

      get_coords_name(S, x, y) == END_OF_LINE) {
    return false;
  } else if (get_coords_name(S, x, y) == CRATE_ON_FREE) {
    return true;
  }
  return true;
}

void move_man(sok_board_t &S, int direction) {
  // Store the current state of the game board and man's position
  State current_state = {S.man1_x, S.man1_y, S.nbr_crates};
  for (int i = 0; i < NBL; i++) {
    for (int j = 0; j < NBC; j++) {
      current_state.board[i][j] = S.board[i][j];
    }
  }
  switch (direction) {
  case MOVE_U:
    if (can_move(S, S.man1_x - 1, S.man1_y) == false) {
      std::cout << "Can't go up" << std::endl;
      break;
    }
    // Si la prochaine case est une caisse alors on la pousse
    // Si seulement c'est une case FREE ou TARGET
    if (get_coords_name(S, S.man1_x - 1, S.man1_y) == CRATE_ON_FREE) {
      if (get_coords_name(S, S.man1_x - 2, S.man1_y) == TARGET) {
        S.board[S.man1_x - 1][S.man1_y] = FREE;
        S.board[S.man1_x - 2][S.man1_y] = CRATE_ON_TARGET;
        S.nbr_crates--;
        current_state.crate_pushed = true;
      }
      if (get_coords_name(S, S.man1_x - 2, S.man1_y) == FREE) {
        S.board[S.man1_x - 1][S.man1_y] = FREE;
        S.board[S.man1_x - 2][S.man1_y] = CRATE_ON_FREE;
      }
      if (get_coords_name(S, S.man1_x - 2, S.man1_y) == CRATE_ON_FREE) {
        break;
      }
      if (get_coords_name(S, S.man1_x - 2, S.man1_y) == WALL) {
        break;
      }
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
      if (get_coords_name(S, S.man1_x + 2, S.man1_y) == TARGET) {
        S.board[S.man1_x + 1][S.man1_y] = FREE;
        S.board[S.man1_x + 2][S.man1_y] = CRATE_ON_TARGET;
        S.nbr_crates--;
        current_state.crate_pushed = true;
      }
      if (get_coords_name(S, S.man1_x + 2, S.man1_y) == FREE) {
        S.board[S.man1_x + 1][S.man1_y] = FREE;
        S.board[S.man1_x + 2][S.man1_y] = CRATE_ON_FREE;
      }
      if (get_coords_name(S, S.man1_x + 2, S.man1_y) == CRATE_ON_FREE) {
        break;
      }
      if (get_coords_name(S, S.man1_x + 2, S.man1_y) == WALL) {
        break;
      }
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
    // Si la prochaine case est une caisse alors on la pousse
    if (get_coords_name(S, S.man1_x, S.man1_y - 1) == CRATE_ON_FREE) {
      if (get_coords_name(S, S.man1_x, S.man1_y - 2) == TARGET) {
        S.board[S.man1_x][S.man1_y - 1] = FREE;
        S.board[S.man1_x][S.man1_y - 2] = CRATE_ON_TARGET;
        S.nbr_crates--;
        current_state.crate_pushed = true;
      }
      if (get_coords_name(S, S.man1_x, S.man1_y - 2) == FREE) {
        S.board[S.man1_x][S.man1_y - 1] = FREE;
        S.board[S.man1_x][S.man1_y - 2] = CRATE_ON_FREE;
      }
      if (get_coords_name(S, S.man1_x, S.man1_y - 2) == CRATE_ON_FREE) {
        break;
      }
      if (get_coords_name(S, S.man1_x, S.man1_y - 2) == WALL) {
        break;
      }
    }
    S.board[S.man1_x][S.man1_y] = FREE;
    S.man1_y -= 1;
    S.board[S.man1_x][S.man1_y] = MAN1_ON_FREE;
    break;
  case MOVE_R:
    if (can_move(S, S.man1_x, S.man1_y + 1) == false) {
      std::cout << "Can't go right" << std::endl;
      break;
    }
    // Si la prochaine case est une caisse alors on la pousse
    if (get_coords_name(S, S.man1_x, S.man1_y + 1) == CRATE_ON_FREE) {
      if (get_coords_name(S, S.man1_x, S.man1_y + 2) == TARGET) {
        S.board[S.man1_x][S.man1_y + 1] = FREE;
        S.board[S.man1_x][S.man1_y + 2] = CRATE_ON_TARGET;
        S.nbr_crates--;
        current_state.crate_pushed = true;
      }
      if (get_coords_name(S, S.man1_x, S.man1_y + 2) == FREE) {
        S.board[S.man1_x][S.man1_y + 1] = FREE;
        S.board[S.man1_x][S.man1_y + 2] = CRATE_ON_FREE;
      }
      if (get_coords_name(S, S.man1_x, S.man1_y + 2) == CRATE_ON_FREE) {
        break;
      }
      if (get_coords_name(S, S.man1_x, S.man1_y + 2) == WALL) {
        break;
      }
    }
    S.board[S.man1_x][S.man1_y] = FREE;
    S.man1_y += 1;
    S.board[S.man1_x][S.man1_y] = MAN1_ON_FREE;
    break;
  }
  std::cout << "---------------------------------" << std::endl;
  S.print_board();
  states.push(current_state);
}

// Si le dernier mouvmeent n'aboutit pas a un chemin valide.
// On utilise la struct state definit en haut pour revenir en arriere.
void undo_move(sok_board_t &S) {
  if (!states.empty()) {
    State previous_state = states.top();
    if (!previous_state.crate_pushed) {
      S.man1_x = previous_state.man_x;
      S.man1_y = previous_state.man_y;
      S.nbr_crates = previous_state.nbr_crate;
      for (int i = 0; i < NBL; i++) {
        for (int j = 0; j < NBC; j++) {
          S.board[i][j] = previous_state.board[i][j];
        }
      }
      states.pop();
    }
  }
}

bool depth_limited_search(sok_board_t &S, int depth) {
  // Si toutes les cases ont ete pousser alors on arrete
  if (S.nbr_crates == 0)
    return true;
  // SI la profondeur definit est atteint on arrete
  if (depth == 0)
    return false;
  // Iterer sur tout les mouvmenets possible.
  for (int i = 0; i < 4; i++) {
    int x = S.man1_x;
    int y = S.man1_y;
    switch (i) {
    case MOVE_U:
      x--;
      break;
    case MOVE_D:
      x++;
      break;
    case MOVE_L:
      y--;
      break;
    case MOVE_R:
      y++;
      break;
    }
    // Si le mouvmeent est possible
    if (can_move(S, x, y)) {
      // Alors on la fait
      move_man(S, i);
      // Rechercher une solution
      if (depth_limited_search(S, depth - 1))
        return true;
      // Annuler le mouvmenet si aucun chemin n'est possible
      undo_move(S);
    }
  }
  return false;
}

bool iddfs(sok_board_t &S) {
  // Demarrer avec une profondeur de 0, et ensuite augmenter
  // jusqu'a ce qu'on atteine la limite etabli en haut
  for (int depth = 0; depth < MAX_DEPTH; depth++) {
    std::cout << "profondeur: " << depth << std::endl;
    if (depth_limited_search(S, depth))
      return true;
  }
  return false;
}

// Constructeur par défaut
sok_board_t::sok_board_t() {
  // Initialiser le grille avec des cases libres
  // Initialiser le tableau seen avec des false
  for (int i = 0; i < NBL; i++)
    for (int j = 0; j < NBC; j++) {
      board[i][j] = FREE;
    }
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
          board[board_nbl][i] = CRATE_ON_FREE;
          // Stocker les coords dans la structure, avant de la
          // copier dans la liste qui va augmenter de taille
          // exactement a chaque nouvelle caisse lu.
          /* Crate c = {board_nbl, i};
          Crate *new_crates = new Crate[nbr_crates + 1];
          for (int i = 0; i < nbr_crates; i++) {
            new_crates[i] = crates[i];
          }
          new_crates[nbr_crates] = c; */
          nbr_crates++;
          /* delete[] crates;
          crates = new_crates; */
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
