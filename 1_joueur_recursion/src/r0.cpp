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
  for (int i = 0; i < S.nbr_crates; i++) {
    std::cout << "Crate " << i + 1 << ": x = " << crates[i].x
              << ", y = " << crates[i].y << std::endl;
  }
  while (choix != 9) {
    if (choix == 9) {
      // Gestion de memoire
      // Supprimer la liste qui stockait les coords
      delete[] crates;
      crates = nullptr;
      S.nbr_crates = 0;
      exit(0);
    }
    S.print_board();
    std::cout << "Press 0 to go up." << std::endl;
    std::cout << "Press 1 to go down." << std::endl;
    std::cout << "Press 2 to go left." << std::endl;
    std::cout << "Press 3 to go right." << std::endl;
    std::cout << "Press 9 to exit" << std::endl;
    std::cout << "Choice: " << std::endl;
    std::cin >> choix;
    move_man(S, choix);
  }
  return 0;
}
