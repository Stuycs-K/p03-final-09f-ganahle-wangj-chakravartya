#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void game_init(struct game *g, int chooser_number) {

  srand(time(NULL));
  g -> chooser_number = chooser_number;
  g -> setpoint = (rand() % 10) + 1;

}

int check_guess(struct game *g, const char *guess) {

  if (g -> chooser_number < g -> setpoint && strncmp(guess, "under", 5) == 0)
    return 1;

  if (g -> chooser_number > g -> setpoint && strncmp(guess, "over", 4) == 0)
    return 1;

  return 0;
}
