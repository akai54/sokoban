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
#include <deque>
#include <vector>
#include<cmath>
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

  deque<tuple<int, int>> crates_on_free;

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
  void path_to_the_goal();
  void print_crates_on_free_pos();
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
          crates_on_free.push_back(make_tuple(board_nbl, i));
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


Règle : 
Il faut interdire les mouvements de boite vers l'objectif. Si une boite est mis sur la case objectif, alors on est à une distance infinie
il faut aussi interdire les déplacement de boite à côté d'une autre boite coller au même mur. Car ça rend impossible tout mouvement
Sinon, de manière évidente, il est interdit de mettre une boite dans un coin, ou sur un mur continue avec deux coins
*/
tuple<int, int> find_nearest_goal(){
  // à créer
  // en vrai osef pour l'instant, on a qu'à décrire des points manuellement
  return make_tuple(0, 0);
}
tuple<int, int> make_move(tuple<int, int> current_pos, int my_move){
  tuple<int, int> vector_of_move = move_in_vector[my_move];
  return make_tuple(get<0>(current_pos) + get<0>(vector_of_move),
                    get<1>(current_pos) + get<1>(vector_of_move));
}


// tuple<int, int> crate_is_movable(tuple<int, int> current_pos, int my_move){
bool crate_is_movable(tuple<int, int> current_pos, int my_move, int my_board[NBL][NBC]){
  // il faut vérifier que le côté opposé de la position est accessible
  // il faudra améliorer cette fonction, en prenant en compte la précédente position théorique du joueur, et de s'il peut accéder à ce fameux côté opposé depuis là où il est
  tuple<int, int> vector_of_move = move_in_vector[my_move];
  int x_pos_opposite_of_move = get<0>(current_pos) + get<0>(vector_of_move) * -1;
  int y_pos_opposite_of_move = get<1>(current_pos) + get<1>(vector_of_move) * -1;
  // if (my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] == FREE) {
  //   cout << "crate is movable " << endl;
  // }else {
  //   cout << "crate is  not movable : " <<  my_board[y_pos_opposite_of_move][x_pos_opposite_of_move]  << endl;
  // }
  // cout << "x_pos_opposite_of_move : " << x_pos_opposite_of_move << endl;
  // cout << "y_pos_opposite_of_move : " << y_pos_opposite_of_move << endl;
  // cout << "my board at this pos : " << my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] << endl;

  // Attention ! j'ai peut-être inversé X et Y
  return my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] == FREE;
}
bool legal_move_crate(tuple<int, int> current_pos,tuple<int, int> new_pos, int my_move, int my_board[NBL][NBC]){
  // move illégal si la case du côté opposé est unreachable, faut utilisé current_pos et move
  // Si c'est reachable, alors la nouvelle case se doit d'être free, ça c'est new_pos


  // Attention ! j'ai peut-être inversé les deux get<X>
  bool my_new_pos_is_free = my_board[get<1>(new_pos)][get<0>(new_pos)] == FREE;
  // if (my_new_pos_is_free) {
  //   cout << "new pos free " << endl;
  // } else {
  //   cout << "new pos not free : " <<  my_board[get<1>(new_pos)][get<0>(new_pos)]<< endl;
  // }


  return  my_new_pos_is_free && crate_is_movable(current_pos, my_move, my_board);
}


int dist(tuple<int, int> a, tuple<int, int> b){
  // je met pas à la racine carré car on s'en fiche de la distance réelle, on veut juste savoir 
  int res = pow((get<0>(b) - get<0>(a)), 2) + pow((get<1>(b) - get<1>(a)), 2);
  return res;
}

void copy_board(int to_copy[NBL][NBC], int where_copied[NBL][NBC]){
  for (int l = 0; l < NBL; l++){
    for (int c = 0; c < NBL; c++){
      where_copied[l][c] = to_copy[l][c];
    }
  }
}

void print_pos(tuple<int, int> pos){
  cout << get<0>(pos) << ", " << get<1>(pos) << endl;
}

void print_path(deque<int> path) {
  for (auto const& direction : path){
       cout << direction << " "; 
  }
  cout << endl << endl;
}


deque<int> a_star_crate(tuple<int, int> current_pos, deque<int> path_to_the_goal, tuple<int, int> goal, int my_board[NBL][NBC]){
  static int dir = 1;
  if (current_pos == goal) { 
    return path_to_the_goal;
  }
  int dist_with_goal = dist(current_pos, goal);
  for (int direction = 0 ; direction < 5; direction++){
    tuple<int, int> new_pos = make_move(current_pos, direction);
    int more_near_pos = dist(new_pos, goal) < dist_with_goal;
    // cout << "pos : ";
    // print_pos(current_pos);
    // cout << "new pos : ";
    // print_pos(new_pos);
    // cout << "goal : ";
    // print_pos(goal);
    // cout << "the distance : " << dist(current_pos, goal) << endl;
    // cout << "the new distance : " << dist(new_pos, goal) << endl;
    // cout << "is more near ? : " << more_near_pos << endl;
    auto is_legal = legal_move_crate(current_pos, new_pos, direction, my_board);
    // cout << "is legal and more near? : " << (legal_move_crate(current_pos, new_pos, direction, my_board) && more_near_pos ) << endl;
    // cout << "is legal " << is_legal << endl;
    // cout << "more near ? " <<  more_near_pos<< endl;
    // cout << "my_move :  " << direction << endl;
    // cout << "---"<< endl;
    if (more_near_pos && is_legal) {
      // cout << "entrée " <<  endl;
      // cout << "dist pos : " << dist_with_goal << endl;
      // cout << "dist newpos : " << dist(new_pos, goal) << endl;
      // cout << "---"<< endl;
      // copy_board(my_board, my_new_board);
      // cout << "direction " << direction <<  endl;
      // print_path(path_to_the_goal);
      path_to_the_goal.push_back(direction);
      // cout << "ajout de la direction : " << endl;
      // print_path(path_to_the_goal);
      // cout << "appel récursif" << endl;
      // cout << "direction " << direction <<  " numéro " << dir <<endl;
      dir++;
      deque<int> res = a_star_crate(new_pos, path_to_the_goal, goal, my_board);
      // cout << "fin appel récursif" << endl;
      // opti réalisable : return le res avec la plus petite taille
      if (!res.empty()){ // si les futurs move sont légaux

        // printf("legal move\n");
        return res;
      }
      path_to_the_goal.pop_back();
    }
  }
  printf("illegal move\n");

  // aucun move n'est légal, ou alors aucun des futurs move ne l'est si on est arrivé jusque là
  deque<int> vide;
  return vide;
}
deque<int> a_star_crate_init(tuple<int, int> current_pos, tuple<int, int> goal, int my_board[NBL][NBC]){
  deque<int> path_to_the_goal;
  // tuple<int, int> goal = find_nearest_goal();
  return a_star_crate(current_pos, path_to_the_goal, goal, my_board);
}

void sok_board_t::print_crates_on_free_pos(){
  for (auto const& pos: this -> crates_on_free){
    cout << "x : " << get<0>(pos)<< ", y : " << get<1>(pos) << " | "; 
  }
  cout << endl;
}
void sok_board_t::path_to_the_goal(){
  int x = 5; 
  int y = 7; 
  tuple<int, int> current_pos = {x, y};
  tuple<int, int> goal = {15, 6};
  // il faudrait ici faire une copy du board et ensuite l'envoyer, car on modifie juste pour obtenir le path mais on veut pas le modifier en soit, mais là pas grave c'est pour des test
  this -> board[y][x] = FREE;
  this -> board[7][15] = FREE;
  this -> print_board ();
  // exit(1);
  auto res = a_star_crate_init(current_pos, goal, this -> board);

  if (res.empty()){
    cout << "no soluce" << endl;
  }
  print_path(res);
  this -> print_crates_on_free_pos();
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

