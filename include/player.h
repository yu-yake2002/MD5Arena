#ifndef PLAYER_H
#define PLAYER_H
#include <stdbool.h>

// properties
enum {HP, ATT, DEF, SPD, DEX, MAG, RES, LER, SHD, TOTHP, NXATT};

#define NAME_LENGTH 32

typedef struct {
  int idx;
  int property_[11];
  int skill_;
  int kill_cnt_, damage_;
  char name_[NAME_LENGTH + 1];
  char md5_[16];
  bool valid_;
  bool alive_;
} Player;

void ConstructPlayer(Player *player, char *name, unsigned char *md5);
void DestructPlayer(Player *player);
void PrintPlayerInfo(Player *player, int maxlen);
void UpdateNxAtt(Player *player);

// Return healed hp.
int PlayerHeal(Player *player, int hp);

#endif
