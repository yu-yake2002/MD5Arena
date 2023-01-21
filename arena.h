#ifndef ARENA_H
#define ARENA_H
#include <stdbool.h>

enum {HP, ATT, DEF, SPD, DEX, MAG, RES, LER, NXATT};

#define NAME_LENGTH 32

typedef struct {
  int property[9]; // Basic properties
  int kill_cnt, damage;
  char name[NAME_LENGTH + 1]; // Name
  unsigned char md5[16];
  bool valid;
  bool alive;
} Player;

void InitGame();
Player *AllocPlayer();
Player *FindPlayer(char *name);
void FillPlayerProperty(Player *player, char *name, unsigned char *property);
void DeletePlayerProperty(Player *player);
void PrintAllPlayerName();
void PrintAllPlayerInfo();
void PrintPlayerInfo(Player *player);
int CountAlive();
void StartGame();
#endif