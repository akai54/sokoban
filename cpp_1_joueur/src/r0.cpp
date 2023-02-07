#include "mysok.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <tuple>

int main(int _ac, char **_av) {
  if (_ac != 2) {
    printf("usage: %s SOK_FILE\n", _av[0]);
    return 0;
  }
  sok_board_t S;
  S.load(_av[1]);
  S.print_board();
  S.print_board_brut();

  a_star_crate_init(make_tuple(0,5));
  return 0;
}
