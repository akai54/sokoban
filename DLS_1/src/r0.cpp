#include "mysok.h"
#include <iostream>

int main(int _ac, char **_av) {
  if (_ac != 2) {
    std::cout << "usage: %s SOK_FILE\n" << _av[0] << std::endl;
    return 0;
  }
  sok_board_t S;
  S.load(_av[1]);
  std::cout << S.nbr_crates << std::endl;

  // Appel Depth Limited Search
  bool solved = iddfs(S);

  if (solved) {
    S.print_board();
    std::cout << "The puzzle was solved successfully." << std::endl;
    // Gestion de memoire
    // Supprimer la liste qui stockait les coords
    /* delete[] crates;
    crates = nullptr; */
    S.nbr_crates = 0;

  } else {
    S.print_board();
    std::cout << "The puzzle was not solved." << std::endl;
  }

  return 0;
}
