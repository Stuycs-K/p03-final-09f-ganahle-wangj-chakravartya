#ifndef GAME_H
#define GAME_H

struct game {

  int chooser_number;
  int setpoint;
  int round_over; // 1 for finished, 0 for not finished, for clarity and ease

};

void game_init(struct game *g, int chooser_number);
int check_guess(struct game *g, const char *guess);

#endif
