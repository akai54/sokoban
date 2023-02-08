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
  std::cout << S.man1_x << "," << S.man1_y << std::endl;
  updateSeenArray(S);
  for (int i = 0; i < NBL; i++) {
    for (int j = 0; j < NBC; j++)
      std::cout << S.seen[i][j] << " ";
    std::cout << std::endl;
  }
  return 0;
}
