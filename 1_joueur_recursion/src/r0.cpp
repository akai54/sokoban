#include "mysok.h"
#include <iostream>

int main(int _ac, char **_av) {
  if (_ac != 2) {
    std::cout << "usage: %s SOK_FILE\n" << _av[0] << std::endl;
    return 0;
  }
  sok_board_t S;
  S.load(_av[1]);
  S.print_board();
  move_man(S, MOVE_R);
  S.print_board();
  move_man(S, MOVE_D);
  S.print_board();
  move_man(S, MOVE_L);
  S.print_board();
  move_man(S, MOVE_L);
  S.print_board();
  move_man(S, MOVE_L);
  return 0;
}
