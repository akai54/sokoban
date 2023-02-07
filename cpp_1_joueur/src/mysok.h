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
#include <tuple>
#include <queue>
#include <vector>

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

using namespace std;

vector<tuple<int, int>> move_in_vector = {
  { make_tuple(0 ,-1) },
  { make_tuple(0 , 1) },
  { make_tuple(-1, 0) },
  { make_tuple(1 , 0) },
  { make_tuple(0 , 0) },
};

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

// Représentation en chaîne des mouvement
string move_str[] = {"Up", "Down", "Left", "Right", "Wait"};

// Structure sok_board_t définissant un grille de jeu Sokoban
struct sok_board_t {
  int board[NBL][NBC]; // grille de jeu
  int board_nbl;       // Nombre de lignes du grille
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

  void print_board_brut();

  bool position_is_free(tuple<int, int> pos);
  bool is_position_of_crate(tuple<int, int> pos);
  bool legal_move_man_1(tuple<int, int> move);
};

// Constructeur par défaut
sok_board_t::sok_board_t() {
  // Initialiser le grille avec des cases libres
  for (int i = 0; i < NBL; i++)
    for (int j = 0; j < NBC; j++)
      board[i][j] = FREE;
}

void sok_board_t::print_board_brut(){
  for (int i = 0; i < board_nbl; i++) {
    for (int j = 0; j < NBC; j++) {
      if (board[i][j] == END_OF_LINE) // Si fin, sortir de la boucle
        break;
      // Sinon afficher la case actuelle
      printf("%d", board[i][j]);
    }
    printf("\n");
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

/*
Algo global :
Comment s'y prendre ?
J'avais penser à calculer le bloc le plus proche, puis le moins proche, puis le
moins proche
mais c'est pas une bonne idée si on veut ramener tout les blocs à destination,
car des fois, il faut un peu déplacer un bloc pour mettre l'autre
du coup, comment faire ?


 */
bool position_exist_on_the_board(tuple<int, int> pos){
  if (get<0>(pos) >= NBC || get<1>(pos) >= NBC || get<0>(pos) < 0 || get<1>(pos) < 0){
    return false;
  }
  return true;
}

tuple<int, int> apply_move_to_position(tuple<int, int> move, int x, int y){
  return make_tuple(x + get<0>(move)
                         ,y + get<1>(move));
}

bool sok_board_t::position_is_free(tuple<int, int> pos){
  if (this -> board[get<0>(pos)][get<1>(pos)] == FREE){
    return true;
  }
  return false;
}

bool sok_board_t::is_position_of_crate(tuple<int, int> pos){
  if (this -> board[get<0>(pos)][get<1>(pos)] == CRATE_ON_FREE){
    return true;
  }
  return false;
}

/* Algo pour les boites : 
Il y a la position d'un boite en argument
Pour chaque mouvement depuis cette boite, regarder s'il est légal. Si oui, regarder s
*/
tuple<int, int> find_nearest_goal(){
  // à créer
  return make_tuple(0, 0);
}
tuple<int, int> make_move(tuple<int, int> current_pos, int my_move){
  tuple<int, int> vector_of_move = move_in_vector[my_move];
  return make_tuple(get<0>(current_pos) + get<0>(vector_of_move),
                    get<1>(current_pos) + get<1>(vector_of_move));
}
// tuple<int, int> crate_is_movable(tuple<int, int> current_pos, int my_move){
bool crate_is_movable(tuple<int, int> current_pos, int my_move){
  // il faut vérifier que le côté opposé de la position est accessible
  // il faudra améliorer cette fonction, en prenant en compte la précédente position théorique du joueur, et de s'il peut accéder à ce fameux côté opposé depuis là où il est
  return true;
}
bool legal_move_crate(tuple<int, int> current_pos,tuple<int, int> new_pos, tuple<int, int> move){
  // move illégal si la case du côté opposé est unreachable, faut utilisé current_pos et move
  // Si c'est reachable, alors la nouvelle case se doit d'être free, ça c'est new_pos
  return true;
}
queue<int> a_star_crate(tuple<int, int> current_pos, queue<int> path_to_the_goal, tuple<int, int> goal){
  if (current_pos == goal) { // on peut peut-être pas comparer des tuple comme ça, dans le cas échéant faudra créer une fonct
    // je commente car c'est plus simple de pas faire comme ça  
    // path_to_the_goal.push_back(current_pos);  // 
    return path_to_the_goal;
  }
  printf("Original pos %d, %d \n", get<0>(current_pos), get<1>(current_pos));
  for (int i = 0 ; i < 5; i++){
    // printf("%d", i)
    tuple<int, int> new_move = make_move(current_pos, i);
    if (legal_move_crate(current_pos, new_move, move_in_vector[i])) {
      printf("%d, %d \n", get<0>(new_move), get<1>(new_move));
    }
    // il manque quoi ? appel récursif pour chaque légal move
    // remplir les autres fonctions
    // peut-être d'autres choses que j'ai oublier
  }
  return path_to_the_goal;
}
queue<int> a_star_crate_init(tuple<int, int> current_pos){
  queue<int> path_to_the_goal;
  tuple<int, int> goal = find_nearest_goal();
  return a_star_crate(current_pos, path_to_the_goal, goal);
}

bool sok_board_t::legal_move_man_1(tuple<int, int> move){
  tuple<int, int> new_pos = apply_move_to_position(move, this->man1_x, this->man1_y);
  if (!position_exist_on_the_board(new_pos)){
    return false;
  }
  if (this->position_is_free(new_pos)){
    return true;
  }
  if (this->is_position_of_crate(new_pos)){
    // calcul de la nouvelle position du crate après le mouvement
    tuple<int, int> new_pos_of_crate = apply_move_to_position(move, get<0>(new_pos), get<1>(new_pos));

    if (position_exist_on_the_board(new_pos_of_crate)){
      if (this->position_is_free(new_pos_of_crate)){
        return true;
      }
    }
  }
  return false;
}

#endif

