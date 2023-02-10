#include "mysok.h"
#include <iostream>

int main(int _ac, char **_av) {
  if (_ac != 2) {
    std::cout << "usage: %s SOK_FILE\n" << _av[0] << std::endl;
    return 0;
  }
  int choix;
  sok_board_t S;
  S.load(_av[1]);
  while (choix != 9) {
    if (choix == 9)
      exit(0);
    S.print_board();
    std::cout << "Press 0 to go up." << std::endl;
    std::cout << "Press 1 to go down." << std::endl;
    std::cout << "Press 2 to go left." << std::endl;
    std::cout << "Press 3 to go right." << std::endl;
    std::cout << "Press 9 to exit" << std::endl;
    std::cout << "Choice: " << std::endl;
    std::cin >> choix;
    move_man(S, choix);
    S.print_board();
  }
  return 0;
}
