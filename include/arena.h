#ifndef ARENA_H
#define ARENA_H
#include <player.h>
#include <stdbool.h>

#define NR_PLAYERS 10

// Alloc memory for players.
void InitArena();
// Free memory for players.
void QuitArena();

// Make all players invalid. Call it after each game.
void NewGame();

void StartGameLoop();

// Return an invalid (unused) player's pointer.
// When all players are occupied, return NULL.
Player *AllocPlayer();
// Return specified player's pointer.
// When no player has the same name, return NULL.
Player *FindPlayer(char *name);

void PrintAllPlayerName();
void PrintAllPlayerInfo();
void PrintResult();
int CountAlive();
int GetMaxNameLength();

Player *SelectByOrder(int key, bool select_min, const char *excp);

#endif