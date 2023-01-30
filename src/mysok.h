#ifndef MYSOK_H
#define MYSOK_H
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>

#define NBL 20
#define NBC 20
#define MAX_CRATES 20

#define MOVE_U 0
#define MOVE_D 1
#define MOVE_L 2
#define MOVE_R 3
#define MOVE_W 4

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

char board_str[] = {' ','_','.','#','$','*','1','u','2','d','a'};
std::string move_str[] = {"Up", "Down", "Left", "Right", "Wait"};

struct sok_board_t {  
  int board[NBL][NBC];
  int board_nbl;
  int man1_x;
  int man1_y;
  int man2_x;
  int man2_y;
  
  sok_board_t();
  void print_board();
  void load(char* _file);
};

sok_board_t::sok_board_t() {
  for(int i = 0; i < NBL; i++)
    for(int j = 0; j < NBC; j++)
      board[i][j] = FREE;
}
void sok_board_t::print_board() {
  for(int i = 0; i < board_nbl; i++) {
    for(int j = 0; j < NBC; j++) {
      if(board[i][j]==END_OF_LINE) break;
      printf("%c", board_str[board[i][j]]);
    }
    printf("\n");
  }
}
void sok_board_t::load(char* _file) {
  FILE *fp = fopen(_file, "r");
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  if (fp == NULL) {
    perror("fopen"); exit(EXIT_FAILURE);
  }
  board_nbl = 0;
  while ((nread = getline(&line, &len, fp)) != -1) {
    if(((int)nread) > 0) {
      bool read_ok = false;
      for(int i = 0; i < nread; i++) {
        if(line[i] == board_str[FREE]) {
          board[board_nbl][i] = FREE;
        } else if(line[i] == board_str[TARGET]) {
          board[board_nbl][i] = TARGET;
        } else if(line[i] == board_str[WALL]) {
          read_ok = true;
          board[board_nbl][i] = WALL;
        } else if(line[i] == board_str[CRATE_ON_FREE]) {
          board[board_nbl][i] = CRATE_ON_FREE;
        } else if(line[i] == board_str[CRATE_ON_TARGET]) {
          board[board_nbl][i] = CRATE_ON_TARGET;
        } else if(line[i] == board_str[MAN1_ON_FREE]) {
          man1_x = board_nbl;
          man1_y = i; 
          board[board_nbl][i] = MAN1_ON_FREE;
        } else if(line[i] == board_str[MAN1_ON_TARGET]) {
          man1_x = board_nbl;
          man1_y = i; 
          board[board_nbl][i] = MAN1_ON_TARGET;
        } else if(line[i] == board_str[MAN2_ON_FREE]) {
          man2_x = board_nbl;
          man2_y = i; 
          board[board_nbl][i] = MAN2_ON_FREE;
        } else if(line[i] == board_str[MAN2_ON_TARGET]) {
          man2_x = board_nbl;
          man2_y = i; 
          board[board_nbl][i] = MAN2_ON_TARGET;
        }
      }
      if(read_ok) {
        board[board_nbl][nread-1]=END_OF_LINE;
        board_nbl++;
      }
    }
  }
  free(line);
  fclose(fp);
}

#endif
