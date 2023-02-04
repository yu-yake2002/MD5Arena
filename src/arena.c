#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arena.h>
#include <skills.h>

static Player *players_[NR_PLAYERS];

void InitArena() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    players_[i] = (Player *)malloc(sizeof(Player));
    players_[i]->idx = i + 1;
  }
  NewGame();
}

void QuitArena() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    free(players_[i]);
  }
}

void NewGame() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    players_[i]->valid_ = false;
  }
}

void StartGameLoop() {
  srand(114514);
  char excp[2] = {0, 0};
  while (CountAlive() != 1) {
    AttackHandler();
  }
}

Player *AllocPlayer() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (!players_[i]->valid_) {
      return players_[i];
    }
  }
  return NULL;
}

Player *FindPlayer(char *name) {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players_[i]->valid_ && strcmp(players_[i]->name_, name) == 0) {
      return players_[i];
    }
  }
  return NULL;
}

void PrintAllPlayerName() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players_[i]->valid_) {
      printf("%s\n", players_[i]->name_);
    }
  }
}

void PrintAllPlayerInfo() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players_[i]->valid_) {
      PrintPlayerInfo(players_[i], GetMaxNameLength());
    }
  }
}

void PrintResult() {
  int maxlen = GetMaxNameLength();
  maxlen = maxlen < 8 ? 8 : maxlen;
  printf("+- Winner -");
  for (int i = 8; i < maxlen; ++i) {
    putchar('-');
  }
  printf("+------+--------+\n");
  printf("| %*s | %*s | %*s |\n", maxlen, "name", 4, "kill", 6, "damage");
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players_[i]->valid_ && players_[i]->alive_) {
      printf("| %*s | %*d | %*d |\n", maxlen, players_[i]->name_, 4,
             players_[i]->kill_cnt_, 6, players_[i]->damage_);
    }
  }
  putchar('+');
  for (int i = -1; i < maxlen; ++i) {
    putchar('-');
  }
  printf("-+------+--------+\n");

  printf("+- Loser --");
  for (int i = 8; i < maxlen; ++i) {
    putchar('-');
  }
  printf("+------+--------+\n");
  printf("| %*s | %*s | %*s |\n", maxlen, "name", 4, "kill", 6, "damage");
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players_[i]->valid_ && !players_[i]->alive_) {
      printf("| %*s | %*d | %*d |\n", maxlen, players_[i]->name_, 4,
             players_[i]->kill_cnt_, 6, players_[i]->damage_);
    }
  }
  putchar('+');
  for (int i = -1; i < maxlen; ++i) {
    putchar('-');
  }
  printf("-+------+--------+\n");
}

int CountAlive() {
  int alive_cnt = 0;
  for (int i = 0; i < NR_PLAYERS; ++i) {
    alive_cnt += players_[i]->alive_;
  }
  return alive_cnt;
}

int GetMaxNameLength() {
  int ret = 0;
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players_[i]->valid_) {
      int len = strlen(players_[i]->name_);
      if (len > ret) {
        ret = len;
      }
    }
  }
  return ret;
}

Player *SelectByOrder(int key, bool select_min, const char *excp) {
  Player *ret = NULL;
  int i;
  for (i = 0; i < NR_PLAYERS; ++i) {
    if (players_[i]->alive_ && players_[i]->valid_ && (excp == NULL || !strchr(excp, i + 1))) {
      ret = players_[i];
      break;
    }
  }
  for (; i < NR_PLAYERS; ++i) {
    if (players_[i]->alive_ && players_[i]->valid_ && (excp == NULL || !strchr(excp, i + 1))) {
      if (select_min == true) {
        if (players_[i]->property_[key] < ret->property_[key] ||
            players_[i]->property_[key] == ret->property_[key] &&
            strcmp(players_[i]->md5_, ret->md5_) < 0) {
          ret = players_[i];
        }
      } else {
        if (players_[i]->property_[key] > ret->property_[key] ||
            players_[i]->property_[key] == ret->property_[key] &&
            strcmp(players_[i]->md5_, ret->md5_) > 0) {
          ret = players_[i];
        }
      }
      
    }
  }
  return ret;
}
