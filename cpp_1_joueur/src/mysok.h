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

void print_a_board(int board[NBL][NBC]) {

  cout << "début" <<endl;
  for (int i = 0; i < NBL; i++) {
    for (int j = 0; j < NBC; j++) {
      if (board[i][j] == END_OF_LINE) // Si fin, sortir de la boucle
        break;
      // Sinon afficher la case actuelle
      printf("%d", board[i][j]);
    }
    printf("\n");
  }
  cout << "fin" <<endl;
  // for (int i = 0; i < NBL; i++) {
  //   for (int j = 0; j < NBC; j++) {
  //     if (board[i][j] == END_OF_LINE) // Si fin, sortir de la boucle
  //       break;
  //     // Sinon afficher la case actuelle
  //     printf("%c", board_str[board[i][j]]);
  //   }
  //   printf("\n");
  // }
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
          targets.push_back(make_tuple(board_nbl, i));
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

tuple<int, int> apply_move_to_position(tuple<int, int> move, tuple<int, int> my_pos){
  return make_tuple(get<0>(my_pos) + get<0>(move)
                         ,get<1>(my_pos) + get<1>(move));
}

// to test 
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
    cout << "---" << endl;
  }
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

int dist(tuple<int, int> a, tuple<int, int> b){
  // je met pas à la racine carré car on s'en fiche de la distance réelle, on veut juste savoir 
  int res = pow((get<0>(b) - get<0>(a)), 2) + pow((get<1>(b) - get<1>(a)), 2);
  return res;
}

bool is_position_free(tuple<int, int> pos, int board[NBL][NBC]){
  return board[get<0>(pos)][get<1>(pos)] == FREE;
}
bool is_position_of_crate(tuple<int, int> pos, int board[NBL][NBC]){
  return board[get<0>(pos)][get<1>(pos)] == CRATE_ON_FREE;
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
  // if (!position_exist_on_the_board(my_new_pos)){
  //   cout << "x : " << get<0>(my_new_pos) <<", y : "<< get<1>(my_new_pos) << endl;
  //   exit(1);
  // }
  if (my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] == FREE){
    my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] = CRATE_ON_FREE;
  } else if (my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] == TARGET) {
    my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] = CRATE_ON_TARGET;
  }

}

void make_move_on_board(int my_board[NBL][NBC], tuple<int, int> my_pos, tuple<int, int> my_new_pos, int my_index_move){ 
  tuple<int, int> move = move_in_vector[my_index_move];
  // non testée 
  if (my_board[get<1>(my_pos)][get<0>(my_pos)] == MAN1_ON_FREE){
    my_board[get<1>(my_pos)][get<0>(my_pos)] = FREE;
  } else {
    my_board[get<1>(my_pos)][get<0>(my_pos)] = TARGET;
  }

  if (my_board[get<1>(my_new_pos)][get<0>(my_new_pos)] == FREE) {
    // je suis pas sûr, j'ai fais la modif à la va vite : 
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

// osef du déplacement du crate, il faudrait faire en sorte de seulement renvoyer les déplacements du joueur 
deque<int> a_star_man(tuple<int, int> current_pos, deque<int> path_to_the_goal, tuple<int, int> goal, int my_board[NBL][NBC]){
  if (current_pos == goal) { 
    return path_to_the_goal;
  }
  int my_new_board[NBL][NBC];
  tuple<int, int> new_pos;
  Move_with_dist mwd;
  list<Move_with_dist> sorted_move; // du mouvement le plus proche du goal au moins proche
  for (int direction = 0 ; direction < 4; direction++){
    new_pos = make_move(current_pos, direction);
    fill_mwd(mwd, direction, dist(new_pos, goal), new_pos);
    insert_with_sorting(sorted_move, mwd);
  }
  cout << "ùùùùùùùùùùùùùùùùùùùù" << endl;
  cout << "current pos x : " << get<0>(current_pos) << ", y : "<< get<1>(current_pos) << endl;
  cout << "man : " << endl;
  print_list_move(sorted_move);
  for (Move_with_dist move : sorted_move){

    bool is_legal = legal_move_man_1(move_in_vector[move.my_move], current_pos, move.new_pos, my_board);
    if (is_legal) {
      cout << "legal move number : " << move.my_move << endl;
      copy_board(my_board, my_new_board);
      make_move_on_board(my_new_board, current_pos, move.new_pos, move.my_move); // je suis pas sûr c'est direction, et j'ai ajouter des arg à la va vite
      path_to_the_goal.push_back(move.my_move);
      deque<int> res = a_star_man(move.new_pos, path_to_the_goal, goal, my_new_board);
      if (!res.empty()){ // si les futurs move sont légaux
        return res;
      }
      path_to_the_goal.pop_back();
    }
  }
  // for (int direction = 0 ; direction < 5; direction++){
  //   tuple<int, int> new_pos = make_move(current_pos, direction);
  //   int more_near_pos = dist(new_pos, goal) < dist_with_goal;
  //   auto is_legal = legal_move_man_1(move_in_vector[direction], current_pos, new_pos, my_board);
  //   if (more_near_pos && is_legal) {
  //     copy_board(my_board, my_new_board);
  //     make_move_on_board(my_new_board, current_pos, new_pos, move_in_vector[direction]); // je suis pas sûr c'est direction, et j'ai ajouter des arg à la va vite
  //     path_to_the_goal.push_back(direction);
  //     deque<int> res = a_star_man(new_pos, path_to_the_goal, goal, my_new_board);
  //     if (!res.empty()){ // si les futurs move sont légaux
  //       return res;
  //     }
  //     path_to_the_goal.pop_back();
  //   }
  // }
  printf("illegal move\n");

  // aucun move n'est légal, ou alors aucun des futurs move ne l'est si on est arrivé jusque là
  deque<int> vide;
  return vide;
}
void print_deque(deque<int> my_deque){
  for (auto el : my_deque) {
    cout << el << endl;
  }
}
// tuple<int, int> crate_is_movable(tuple<int, int> current_pos, int my_move){
tuple<bool, deque<int>> crate_is_movable(tuple<int, int> man_pos, tuple<int, int> current_pos, int my_move, int my_board[NBL][NBC]){
  // il faut vérifier que le côté opposé de la position est accessible
  // il faudra améliorer cette fonction, en prenant en compte la précédente position théorique du joueur, et de s'il peut accéder à ce fameux côté opposé depuis là où il est
  tuple<int, int> vector_of_move = move_in_vector[my_move];
  int x_pos_opposite_of_move = get<0>(current_pos) + get<0>(vector_of_move) * -1;
  int y_pos_opposite_of_move = get<1>(current_pos) + get<1>(vector_of_move) * -1;
  deque<int> path_to_the_goal;

  tuple<int, int> goal_for_man = make_tuple(x_pos_opposite_of_move, y_pos_opposite_of_move);
  auto path_of_man = a_star_man(man_pos, path_to_the_goal, goal_for_man, my_board);
  if (path_of_man.size() == 0){
    return make_tuple(false, path_of_man);
  } else {
    cout << "chemin du joueur" << endl;
    print_deque(path_of_man);
  }

  if (my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] == FREE || my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] == TARGET) {
    cout << "crate is movable " << endl;
  }else {
    cout << "crate is  not movable : " <<  my_board[y_pos_opposite_of_move][x_pos_opposite_of_move]  << endl;
  }
  // cout << "x_pos_opposite_of_move : " << x_pos_opposite_of_move << endl;
  // cout << "y_pos_opposite_of_move : " << y_pos_opposite_of_move << endl;
  // cout << "my board at this pos : " << my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] << endl;

  // Attention ! j'ai peut-être inversé X et Y
  return make_tuple(my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] == FREE || my_board[y_pos_opposite_of_move][x_pos_opposite_of_move] == TARGET, path_of_man); // && a_star_man();
}
tuple<bool, deque<int>> legal_move_crate(tuple<int, int> man_pos, tuple<int, int> current_pos,tuple<int, int> new_pos, int my_move, int my_board[NBL][NBC]){
  // move illégal si la case du côté opposé est unreachable, faut utilisé current_pos et move
  // Si c'est reachable, alors la nouvelle case se doit d'être free, ça c'est new_pos


  // Attention ! j'ai peut-être inversé les deux get<X>
  bool my_new_pos_is_free = my_board[get<1>(new_pos)][get<0>(new_pos)] == FREE || my_board[get<1>(new_pos)][get<0>(new_pos)] == TARGET;
  if (my_new_pos_is_free) {
    cout << "new pos free " << endl;
  } else {
    cout << "new pos not free : " <<  my_board[get<1>(new_pos)][get<0>(new_pos)]<< endl;
  }

  tuple<int, deque<int>> res = crate_is_movable(man_pos, current_pos, my_move, my_board);

  bool is_movable = get<0>(res);
  deque<int> path_of_man = get<1>(res);
  return  make_tuple(my_new_pos_is_free && is_movable, path_of_man);
}




/*
  Que faire maintenant ? 
J'ai besoin maintenant de calculer les déplacements du man
Il faut test toutes les possibilités de mouvements possible
En faite, déjà il faudrait être certain qu'il soit possible de faire les mouvements quand on est arrivé à destination
Donc il faudrait rajouter une règle dans les legal_move qui vérifie si le joueur sera capable de bouger jusqu'au prochain mouvement
Problème : ce mouvement peut inclure un déplacement de boite, ce qui fait qu'il va aussi falloir mettre à jour le tableau, et donc en créer des copies 

Autre problème : peut-être qu'au final, c'était pas le bon mouvement car bien qu'il permette d'arrivé à destination, il bouge d'autres boites de manière à ce que ça fonctionne pas. Après, c'est pas si grave comme problème, car généralement, ça devrait pas poser problème, et au pire on en a au moins mis un certain nombre

En vrai, à partir du moment où l'on a prouvé que c'est possible de bouger le crate jusqu'à destination, il suffit de prouver qu'on peut arrivé jusqu'à l'endroit souhaiter, et à partir de ce moment, on est sûr de pouvoir la mettre. Un autre problème se pose : peut-être que la position est inateignable. Est-ce possible ? Si en bougeant une boite, on la met devant la position où l'on doit aller, oui. Et si on interdit ce fait, il est possible que l'objectif soit inateignable, car il faut mettre la boite d'un autre point de départ, ou bien il faut en mettre une autre.  Et c'est sur ce point où je sais pas comment m'y prendre, si au final c'est impossible, comment faire pour trouver un autre path ? Ce qu'on peut faire, c'est faire la vérification au tout début, voir lors de l'appel de la fonction si un mouvement est légal, et comme condition qu'il soit légal est que le man puisse y accéder

Donc ce que je pourrais faire, c'est d'abord voir un chemin 
*/

void print_pos(tuple<int, int> pos){
  cout << get<0>(pos) << ", " << get<1>(pos) << endl;
}

void print_path(deque<int> path) {
  for (auto const& direction : path){
       cout << direction << " "; 
  }
  cout << endl << endl;
}


// modif à faire : il ne faut pas éliminer les mouvement moins près comme je le fais mais plutôt faire en priorité ceux qui sont le plus près
// il faudrait donc deux boucle : la première où on filtre les coup interdit, et qu'on trie par priorité du plus près au moins près les coups légaux 
// et une deuxième boucle où l'on réalise les mouvements
// problème : un des critères de legal move est justement le fait des mouvements du man_1 permette le déplacement du crate
// au pire pas grave, on filtre pas les coups interdit tant pis, on se contente juste de ranger les coups de manière bête et méchante
// aussi, il faut interdire le coup inverse du précédent coup du coup, càd si le précédent coup est up, le prochain ne peux pas être down. C'est important comme règle car le mouvement devient inutile
// en faite, pas besoin d'annuler les previous move, dans tout les cas si c'est un mauvais coup, il est mis en bas dans la priorité, et dans certain car ça pourrait être utile peut-êtr edonc pas grave

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
deque<int> a_star_crate(tuple<int, int> man_pos, tuple<int, int> current_pos, deque<int> path_to_the_goal, tuple<int, int> goal, int my_board[NBL][NBC]){
  cout << "seg ?" << endl;
  static int dir = 1;
  if (current_pos == goal) { 
    return path_to_the_goal;
  }
  // int dist_with_goal = dist(current_pos, goal);
  int my_new_board[NBL][NBC];
  tuple<int, int> new_pos;
  Move_with_dist mwd;
  list<Move_with_dist> sorted_move; // du mouvement le plus proche du goal au moins proche
  for (int direction = 0 ; direction < 4; direction++){
    new_pos = make_move(current_pos, direction);
    fill_mwd(mwd, direction, dist(new_pos, goal), new_pos);
    insert_with_sorting(sorted_move, mwd);
  }
  print_list_move(sorted_move);
  for (Move_with_dist move : sorted_move){

    tuple<int, deque<int>> is_legal_and_man_path = legal_move_crate(man_pos, current_pos, move.new_pos, move.my_move, my_new_board);
    bool is_legal = get<0>(is_legal_and_man_path);
    deque<int> man_path = get<1>(is_legal_and_man_path);
    if (is_legal) {
    copy_board(my_board, my_new_board);
      // if (move.my_move != 4){
      //   cout << "sortient ! " <<  move.my_move<< endl;
      //   exit(1);
      // }
      cout << "legalent " << endl;
    cout << "commencement du print " << endl;
      print_a_board(my_new_board);
    cout << "fin du print " << endl;
      man_path.push_back(move.my_move);
      make_moves_on_board(man_pos, man_path, my_new_board);
      // make_move_crate_on_board(my_new_board, current_pos, move.my_move);
      cout << "le move : " << move.my_move << endl;
    cout << "commencement du print " << endl;
      print_a_board(my_new_board);
    cout << "fin du print " << endl;
      // exit(1);
      cout << "moved " << endl;
      // print_a_board(my_new_board);
      push_deque_in_deque(path_to_the_goal, man_path);
      cout << "addent" << endl;
      cout << "addent 2" << endl;
      // dir++;
      // cout << "val : " << dir << endl;
      // cout << "new pos (x, y) : " << get<0>(new_pos) << ", " << get<1>(new_pos)<< endl;
      deque<int> res = a_star_crate(man_pos, move.new_pos, path_to_the_goal, goal, my_board);
      cout << "res" << endl;
      if (!res.empty()){ // si les futurs move sont légaux
        return res;
      }
      for (int i = 0; i < man_path.size(); i++){
        path_to_the_goal.pop_back();
      }
    }
  }
  // for (int direction = 0 ; direction < 5; direction++){
  //   tuple<int, int> new_pos = make_move(current_pos, direction);
  //   int more_near_pos = dist(new_pos, goal) < dist_with_goal;
  //   // IL FAUT ENVOYER ICI LE PATH DU JOUEUR PAR REFERENCE
  //   /* aussi, le déplacement du joueur est susceptible de modifié le tableau. Il y a plusieurs manière de régler le soucis.
  //      la mise à jour du tableau on la fait ici ? Non, par contre il faut récupérer le tableau modifié pour l'appel récursif
  //      Non car on est peut-être sur une "mauvaise" branche, donc il faut faire la modification du tableau uniquement lorsqu'on est sûr que ça fonctionne 
  //      Par contre, du coup, je crois qu'il faudrait créer le board ici, non ? c'est pas le plus opti, on pourrait faire beaucoup plus opti en utilisant seulement une position virtuelle de la boite et du man mais azy osef, c'est pas non plus monstrueux en complexité spaciale (n, vu qu'on réutilise ) 
  //    */
  //   copy_board(my_board, my_new_board);
  //   auto is_legal = legal_move_crate(man_pos, current_pos, new_pos, direction, my_new_board);
  //   if (more_near_pos && is_legal) {
  //     path_to_the_goal.push_back(direction);
  //     deque<int> res = a_star_crate(man_pos, new_pos, path_to_the_goal, goal, my_board);
  //     if (!res.empty()){ // si les futurs move sont légaux
  //       return res;
  //     }
  //     path_to_the_goal.pop_back();
  //   }
  // }
  printf("illegal move\n");

  // aucun move n'est légal, ou alors aucun des futurs move ne l'est si on est arrivé jusque là
  deque<int> vide;
  return vide;
}
deque<int> a_star_crate_init(tuple<int, int> current_pos, tuple<int, int> goal, int my_board[NBL][NBC]){
  deque<int> path_to_the_goal;
  // tuple<int, int> goal = find_nearest_goal();
  tuple<int, int> man_foo = make_tuple(3, 1);
  return a_star_crate(man_foo, current_pos, path_to_the_goal, goal, my_board);
}


// il me faut une fonction qui prend la position de tout les crates, regarde la distance avec la case objectif la plus éloignée, et enregistre ça dans une struct
// ensuite, on range ça dans une liste par ordre de distance minimum
// ensuite, on sélectionne le crate avec la plus petite distance
// on regarde si elle peut atteindre l'objectif le plus lointain (ça renvoie la liste de mouvement du man à partir de la case adjacente au crate de départ). Si oui, on calcule le déplacement du joueur jusqu'à la case objectif
// en faite, on peut faire les deux en même temps, il faut passé la case de départ du joueur, et normalement le legal move calcule tout seul si c'est possible d'y aller. L'avantage et que on peut tester pour plusieur case adjacente de départ différente facilement
// ça doit nous renvoyer plusieurs info : la suite de mouvement du man, ainsi que sa position à la fin, et nouveau tableau (à moins qu'il soit automatiquement modifié dans la fonction)

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
void sok_board_t::path_to_the_goal(){

  int x = 1; 
  int y = 3; 
  // int x = 5; 
  // int y = 7; 
  tuple<int, int> current_pos = {x, y};

  tuple<int, int> goal = {1, 4};
  // tuple<int, int> goal = {15, 6};
  // il faudrait ici faire une copy du board et ensuite l'envoyer, car on modifie juste pour obtenir le path mais on veut pas le modifier en soit, mais là pas grave c'est pour des test
  // this -> board[y][x] = FREE;
  // this -> board[7][15] = FREE;
  this -> print_board ();
  // exit(1);
  auto res = a_star_crate_init(current_pos, goal, this -> board);

  if (res.empty()){
    cout << "no soluce" << endl;
  }
  print_path(res);
  cout << "crates : "<< endl;
  this -> print_crates_on_free_pos();
  cout << "targets : " << endl;
  this -> print_targets();
}



#endif

