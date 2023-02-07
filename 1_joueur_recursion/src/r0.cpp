#include "mysok.h"
#include <iostream>

int main(int _ac, char **_av) {
  if (_ac != 2) {
    printf("usage: %s SOK_FILE\n", _av[0]);
    return 0;
  }
  sok_board_t S;
  S.load(_av[1]);
  S.print_board();
  std::cout << S.man1_x << "," << S.man1_y << std::endl;
  move_man(S, 0);
  S.print_board();
  std::cout << S.man1_x << "," << S.man1_y << std::endl;
  move_man(S, 1);
  S.print_board();
  std::cout << S.man1_x << "," << S.man1_y << std::endl;
  move_man(S, 2);
  S.print_board();
  std::cout << S.man1_x << "," << S.man1_y << std::endl;
  move_man(S, 3);
  S.print_board();
  std::cout << S.man1_x << "," << S.man1_y << std::endl;

  return 0;
}
