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
#include <climits>
#include <iostream>
#include <string.h>
#include <string>
#include <tuple>
#include <deque>
#include <vector>
#include <cmath>
#include <algorithm>
#include <list>
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
  deque<tuple<int, int>> targets;

  // Constructeur par défaut
  sok_board_t();

  // Afficher le grille de jeu
  void print_board();

  // Charger le grille de jeu à partir d'un fichier
  void load(char *_file);

  void print_board_brut();

  void path_to_the_goal();
  void print_crates_on_free_pos();
  void print_targets();
  void print_pos_man1();

  void set_new_pos_man1(tuple<int, int> new_pos);
};

// Constructeur par défaut
sok_board_t::sok_board_t() {
  // Initialiser le grille avec des cases libres
  for (int i = 0; i < NBL; i++)
    for (int j = 0; j < NBC; j++)
      board[i][j] = FREE;
}

void sok_board_t::print_pos_man1(){
  cout << "pos man1, x : " << this -> man1_x << ", y : " << this -> man1_y << endl;
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

void print_a_board(int board[NBL][NBC]) {

  cout << "début" <<endl;
  for (int i = 0; i < NBL; i++) {
    for (int j = 0; j < NBC; j++) {
      if (board[i][j] == END_OF_LINE) // Si fin, sortir de la boucle
        break;
      // Sinon afficher la case actuelle
      printf("%c", board_str[board[i][j]]);
    }
    printf("\n");
  }
  cout << "fin" <<endl;
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
          targets.push_back(make_tuple(i, board_nbl));
        } else if (line[i] == board_str[WALL]) {
          read_ok = true;
          board[board_nbl][i] = WALL;
        } else if (line[i] == board_str[CRATE_ON_FREE]) {
          board[board_nbl][i] = CRATE_ON_FREE;
          crates_on_free.push_back(make_tuple(i, board_nbl));
        } else if (line[i] == board_str[CRATE_ON_TARGET]) {
          board[board_nbl][i] = CRATE_ON_TARGET;
        } else if (line[i] == board_str[MAN1_ON_FREE]) {
          man1_y = board_nbl;
          man1_x = i;
          board[board_nbl][i] = MAN1_ON_FREE;
        } else if (line[i] == board_str[MAN1_ON_TARGET]) {
          man1_y = board_nbl;
          man1_x = i;
          board[man1_y][man1_x] = MAN1_ON_TARGET;
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

bool position_exist_on_the_board(tuple<int, int> pos){
  if (get<0>(pos) >= NBC || get<1>(pos) >= NBC || get<0>(pos) < 0 || get<1>(pos) < 0){
    return false;
  }
  return true;
}

tuple<int, int> apply_move_to_position(tuple<int, int> move, tuple<int, int> my_pos){
  return make_tuple(get<0>(my_pos) + get<0>(move)
                    ,get<1>(my_pos) + get<1>(move));
}

typedef struct move_with_dist{
  int my_move;
  tuple<int, int> new_pos;
  int dist;
} Move_with_dist;

void fill_mwd(Move_with_dist& mwd, int my_move, int dist, tuple<int, int> new_pos){
  mwd.my_move = my_move;
  mwd.dist = dist;
  mwd.new_pos = new_pos;
}
// inspiré d'ici https://stackoverflow.com/questions/35840580/can-i-make-stdlist-insert-new-elements-with-order-or-got-to-use-stdsort
void insert_with_sorting(list<Move_with_dist>& moves_priority, Move_with_dist to_insert){
  if (moves_priority.size() < 1){
    moves_priority.push_back(to_insert);
  } else {
    auto begin = moves_priority.begin();
    auto end = moves_priority.end();
    while((begin != end) && (begin -> dist < to_insert.dist)) {
      begin++;
    }
    moves_priority.insert(begin, to_insert);
  }
}


void print_list_move(list<Move_with_dist>& moves_priority){
  for (auto i : moves_priority){
    cout << "my move : "<< i.my_move << endl;
    cout << "dist : "<< i.dist << endl;
    cout << "newpos x : "<< get<0>(i.new_pos)<< ", y :" << get<1>(i.new_pos) << endl;
    cout << "---" << endl;
  }
}

tuple<int, int> make_move(tuple<int, int> current_pos, int my_move){
  tuple<int, int> vector_of_move = move_in_vector[my_move];
  return make_tuple(get<0>(current_pos) + get<0>(vector_of_move),
                    get<1>(current_pos) + get<1>(vector_of_move));
}

int dist(tuple<int, int> a, tuple<int, int> b){
  // je met pas à la racine carré car on s'en fiche de la distance réelle, on veut juste savoir 
  int res = pow((get<0>(b) - get<0>(a)), 2) + pow((get<1>(b) - get<1>(a)), 2);
  return res;
}

tuple<int, int> find_nearest_goal(tuple<int, int> my_crate, deque<tuple<int, int>>& list_of_target){
  // vérifier si il y a une place de dispo 
  int min_dist = dist(my_crate, list_of_target[0]);
  int new_dist, index_nearest_target;

  for (int i = 0; i < (int) list_of_target.size(); i++) {
    cout << " " << endl;
    new_dist = dist(my_crate, list_of_target[i]);
    if (min_dist > new_dist){
      min_dist = new_dist;
      index_nearest_target = i;
    }
  }
  return list_of_target[index_nearest_target];
}
int find_the_nearest_crate_from_target(deque<tuple<int, int>>& crates, tuple<int, int>& random_target){
  int min_dist = dist(crates[0], random_target);
  int my_nearest = 0;
  int new_dist;
  for (int i = 0; i < (int) crates.size(); i++){
    new_dist = dist(crates[i], random_target);
    if (new_dist < min_dist){
      min_dist = new_dist;
      my_nearest = i;
    }
  }
  return my_nearest;
}
bool is_position_free(tuple<int, int> pos, int board[NBL][NBC]){
  return board[get<1>(pos)][get<0>(pos)] == FREE;
}
bool is_position_of_crate(tuple<int, int> pos, int board[NBL][NBC]){
  return board[get<1>(pos)][get<0>(pos)] == CRATE_ON_FREE;
}

bool legal_move_man_1(tuple<int, int> move, tuple<int, int> my_pos, tuple<int, int> my_new_pos, int board[NBL][NBC]){
  if (!position_exist_on_the_board(my_new_pos)){
    return false;
  }

  if (is_position_free(my_new_pos, board)){
    return true;
  }

  if (is_position_of_crate(my_new_pos, board)){
    // calcul de la nouvelle position du crate après le mouvement
    tuple<int, int> new_pos_of_crate = apply_move_to_position(move, my_new_pos);

    if (position_exist_on_the_board(new_pos_of_crate)){
      if (is_position_free(new_pos_of_crate, board)){
        return true;
      }
    }
  }
  return false;
}

void copy_board(int to_copy[NBL][NBC], int where_copied[NBL][NBC]){
  for (int l = 0; l < NBL; l++){
    for (int c = 0; c < NBC; c++){
      where_copied[l][c] = to_copy[l][c];
    }
  }
}
void make_move_crate_on_board(int my_board[NBL][NBC], tuple<int, int> my_pos, int move){
  if (my_board[get<1>(my_pos)][get<0>(my_pos)] == CRATE_ON_FREE){
    my_board[get<1>(my_pos)][get<0>(my_pos)] = FREE;
  } else if (my_board[get<1>(my_pos)][get<0>(my_pos)] == CRATE_ON_FREE) {
    my_board[get<1>(my_pos)][get<0>(my_pos)] = TARGET;
  }

  tuple<int, int> my_new_pos= make_move(my_pos, move);
  if (my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] == FREE){
    my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] = CRATE_ON_FREE;
  } else if (my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] == TARGET) {
    my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] = CRATE_ON_TARGET;
  }

}

void make_move_on_board(int my_board[NBL][NBC], tuple<int, int> my_pos, tuple<int, int> my_new_pos, int my_index_move){ 
  if (my_board[get<1>(my_pos)][get<0>(my_pos)] == MAN1_ON_FREE){
    my_board[get<1>(my_pos)][get<0>(my_pos)] = FREE;
  } else {
    my_board[get<1>(my_pos)][get<0>(my_pos)] = TARGET;
  }

  if (my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] == FREE) {
    my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] = MAN1_ON_FREE;
  } else if (my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] == CRATE_ON_FREE){

    make_move_crate_on_board(my_board, my_new_pos, my_index_move);
    my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] = MAN1_ON_FREE;
  } else if (my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] == CRATE_ON_TARGET)
    {
      make_move_crate_on_board(my_board, my_new_pos, my_index_move);
      my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] = MAN1_ON_TARGET;
    }
}

deque<int> a_star_man(tuple<int, int> current_pos, deque<int> path_to_the_goal, tuple<int, int> goal, int my_board[NBL][NBC], int previous_move){
  if (current_pos == goal) { 
    return path_to_the_goal;
  }
  int my_new_board[NBL][NBC];
  tuple<int, int> new_pos;
  Move_with_dist mwd;
  list<Move_with_dist> sorted_move; // du mouvement le plus proche du goal au moins proche
  for (int direction = 0 ; direction < 4; direction++){
    if (direction != previous_move){
      new_pos = make_move(current_pos, direction);
      fill_mwd(mwd, direction, dist(new_pos, goal), new_pos);
      insert_with_sorting(sorted_move, mwd);
    }
  }
  // print_a_board(my_board);
  for (Move_with_dist move : sorted_move){


    bool is_legal = legal_move_man_1(move_in_vector[move.my_move], current_pos, move.new_pos, my_board);
    if (is_legal) {
      copy_board(my_board, my_new_board);
      make_move_on_board(my_new_board, current_pos, move.new_pos, move.my_move); 
      path_to_the_goal.push_back(move.my_move);
      deque<int> res = a_star_man(move.new_pos, path_to_the_goal, goal, my_new_board, move.my_move);
      if (!res.empty()){ // si les futurs move sont légaux
        return res;
      }
      path_to_the_goal.pop_back();
    }
  }
  deque<int> vide;
  return vide;
}
void print_deque(deque<int> my_deque){
  for (auto el : my_deque) {
    cout << el << endl;
  }
}
tuple<bool, deque<int>> crate_is_movable(tuple<int, int> man_pos, tuple<int, int> current_pos, int my_move, int my_board[NBL][NBC]){
  // il faut vérifier que le côté opposé de la position est accessible
  // il faudra améliorer cette fonction, en prenant en compte la précédente position théorique du joueur, et de s'il peut accéder à ce fameux côté opposé depuis là où il est
  tuple<int, int> vector_of_move = move_in_vector[my_move];
  int x_pos_opposite_of_move = get<0>(current_pos) + get<0>(vector_of_move) * -1;
  int y_pos_opposite_of_move = get<1>(current_pos) + get<1>(vector_of_move) * -1;
  deque<int> path_to_the_goal;

  tuple<int, int> goal_for_man = make_tuple(x_pos_opposite_of_move, y_pos_opposite_of_move);
  auto path_of_man = a_star_man(man_pos, path_to_the_goal, goal_for_man, my_board, -1);
  if (path_of_man.size() == 0){
    return make_tuple(false, path_of_man);
  } 
  return make_tuple(my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] == FREE || my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] == TARGET, path_of_man); 
}
tuple<bool, deque<int>> legal_move_crate(tuple<int, int> man_pos, tuple<int, int> current_pos,tuple<int, int> new_pos, int my_move, int my_board[NBL][NBC]){
  // move illégal si la case du côté opposé est unreachable, faut utilisé current_pos et move
  // Si c'est reachable, alors la nouvelle case se doit d'être free, ça c'est new_pos

  bool my_new_pos_is_free = my_board[get<1>(new_pos)][get<0>(new_pos)] == FREE || my_board[get<1>(new_pos)][get<0>(new_pos)] == TARGET;
  tuple<int, deque<int>> res = crate_is_movable(man_pos, current_pos, my_move, my_board);

  bool is_movable = get<0>(res);
  deque<int> path_of_man = get<1>(res);
  return  make_tuple(my_new_pos_is_free && is_movable, path_of_man);
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

void push_deque_in_deque (deque<int>& my_deque, deque<int>& deque_to_add){
  for (int el : deque_to_add){
    my_deque.push_back(el);
  }
}

void make_moves_on_board(tuple<int, int>& man_pos, deque<int> my_moves, int my_board[NBL][NBC]){
  for (int my_move : my_moves){
    tuple<int, int> new_pos = make_move(man_pos, my_move);
    make_move_on_board(my_board, man_pos, new_pos, my_move);
    man_pos = new_pos;
  }
}
deque<int> a_star_crate(tuple<int, int> man_pos, tuple<int, int> current_pos, deque<int> path_to_the_goal, tuple<int, int> goal, int my_board[NBL][NBC], int previous_move){
  if (current_pos == goal) { 
    return path_to_the_goal;
  }
  int my_new_board[NBL][NBC];
  tuple<int, int> new_pos;
  Move_with_dist mwd;
  list<Move_with_dist> sorted_move; // du mouvement le plus proche du goal au moins proche
  for (int direction = 0 ; direction < 4; direction++){
    if (direction != previous_move){
      new_pos = make_move(current_pos, direction);
      fill_mwd(mwd, direction, dist(new_pos, goal), new_pos);
      insert_with_sorting(sorted_move, mwd);
    }
  }
  for (Move_with_dist move : sorted_move){


    copy_board(my_board, my_new_board);
    tuple<int, deque<int>> is_legal_and_man_path = legal_move_crate(man_pos, current_pos, move.new_pos, move.my_move, my_new_board);
    bool is_legal = get<0>(is_legal_and_man_path);
    deque<int> man_path = get<1>(is_legal_and_man_path);
    if (is_legal) {
      man_path.push_back(move.my_move);
      make_moves_on_board(man_pos, man_path, my_new_board);
      push_deque_in_deque(path_to_the_goal, man_path);
      deque<int> res = a_star_crate(man_pos, move.new_pos, path_to_the_goal, goal, my_board, move.my_move);
      if (!res.empty()){ // si les futurs move sont légaux
        return res;
      }
      for (int i = 0; i < (int) man_path.size(); i++){
        path_to_the_goal.pop_back();
      }
    }
  }
  deque<int> vide;
  return vide;
}
deque<int> a_star_crate_init(tuple<int, int> current_pos, tuple<int, int> goal, int my_board[NBL][NBC], tuple<int, int> man_pos){
  deque<int> path_to_the_goal;
  return a_star_crate(man_pos, current_pos, path_to_the_goal, goal, my_board, -1);
}

void sok_board_t::print_targets(){
  for (auto const& pos: this -> targets){
    cout << "x : " << get<0>(pos)<< ", y : " << get<1>(pos) << " | "; 
  }
  cout << endl;
}
void sok_board_t::print_crates_on_free_pos(){
  for (auto const& pos: this -> crates_on_free){
    cout << "x : " << get<0>(pos)<< ", y : " << get<1>(pos) << " | "; 
  }
  cout << endl;
}

void sok_board_t::set_new_pos_man1(tuple<int, int> new_pos){
  this -> man1_x = get<0>(new_pos);
  this -> man1_y = get<1>(new_pos);
}
void sok_board_t::path_to_the_goal(){
  
  tuple<int, int> current_pos = {this -> man1_x, this -> man1_y};
  deque<int> entire_path;
  tuple<int, int> crate_pos;
  int crate_pos_index;
  
  while (this -> crates_on_free.size() > 0) {
    crate_pos_index = find_the_nearest_crate_from_target(this->crates_on_free, this -> targets[0]);
    crate_pos = this -> crates_on_free[crate_pos_index];
    this -> crates_on_free.erase(this -> crates_on_free.begin() + crate_pos_index);
    cout << " " << endl;

    tuple<int, int> goal = find_nearest_goal(crate_pos, this -> targets);
    cout << "goal x : "   << get<0>(goal) << ", y : " << get<1>(goal) << 
      "; crate_pos x : " << get<0>(crate_pos) << ", y : " << get<1>(crate_pos) << endl;
    auto res = a_star_crate_init(crate_pos, goal, this -> board, current_pos);
    if (res.empty()){
      cout << "Pas de solution" << endl;
    }
    push_deque_in_deque(entire_path, res);
    make_moves_on_board(current_pos, res, this -> board);
    this -> set_new_pos_man1(current_pos);
    this -> print_board ();
    cout << "chemin solution : " << endl;
    for (int i: entire_path){
      cout <<  i << endl;
    }
  }
}

#endif
