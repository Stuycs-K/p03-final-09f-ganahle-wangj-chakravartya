#ifndef GAME_H
#define GAME_H

struct game {

  int chooser_number;
  int setpoint;

};

void game_init(struct game *g, int chooser_number);
int check_guess(struct game *g, const char *guess);

#endif
