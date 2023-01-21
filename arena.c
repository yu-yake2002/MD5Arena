#include "arena.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *propertyArr[] = {"HP",  "ATT", "DEF", "SPD", "DEX",
                              "MAG", "RES", "LER", NULL};

#define NR_PLAYERS 10
static Player players[NR_PLAYERS];

void InitGame() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    players[i].valid = false;
  }
}

Player *AllocPlayer() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (!players[i].valid) {
      return &players[i];
    }
  }
  return NULL;
}

Player *FindPlayer(char *name) {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players[i].valid && 0 == strncmp(name, players[i].name, NAME_LENGTH)) {
      return &players[i];
    }
  }
  return NULL;
}

inline static void UpdateNxAtt(Player *player) {
  player->property[NXATT] += 200 - player->property[SPD];
}

void FillPlayerProperty(Player *player, char *name, unsigned char *md5) {
  if (player) {
    player->valid = true;
    player->alive = true;
    player->kill_cnt = 0;
    strncpy(player->name, name, NAME_LENGTH);
    player->property[HP] = 200 + md5[0];
    for (int i = 1; i < 8; ++i) {
      player->property[i] = 35 + (md5[i] & 0x3f);
    }
    memcpy(player->md5, md5, 16);
    player->property[NXATT] = 0;
    UpdateNxAtt(player);
  }
}

void DeletePlayerProperty(Player *player) {
  if (player) {
    player->valid = false;
  }
}

void PrintAllPlayerName() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players[i].valid) {
      printf("%s\n", players[i].name);
    }
  }
}

void PrintAllPlayerInfo() {
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players[i].valid) {
      PrintPlayerInfo(&players[i]);
    }
  }
}

static int GetMaxNameLength() {
  int ret = 0;
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players[i].valid) {
      int len = strlen(players[i].name);
      if (len > ret) {
        ret = len;
      }
    }
  }
  return ret;
}

void PrintPlayerInfo(Player *player) {
  int maxlen = GetMaxNameLength();
  printf("%*s: ", maxlen, player->name);
  for (int i = 0; i < 8; ++i) {
    printf("%s %d ", propertyArr[i], player->property[i]);
  }
  printf("\n");
}

static void PrintResult() {
  int maxlen = GetMaxNameLength();
  maxlen = maxlen < 8 ? 8 : maxlen;
  printf("+- Winner -");
  for (int i = 8; i < maxlen; ++i) {
    putchar('-');
  }
  printf("+------+--------+\n");
  printf("| %*s | %*s | %*s |\n", maxlen, "name", 4, "kill", 6, "damage");
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players[i].valid && players[i].alive) {
      printf("| %*s | %*d | %*d |\n", maxlen, players[i].name, 4,
             players[i].kill_cnt, 6, players[i].damage);
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
    if (players[i].valid && !players[i].alive) {
      printf("| %*s | %*d | %*d |\n", maxlen, players[i].name, 4,
             players[i].kill_cnt, 6, players[i].damage);
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
    alive_cnt += players[i].alive && players[i].valid;
  }
  return alive_cnt;
}

static Player *SelectMin(int key) {
  int idx = -1;
  for (int i = 0; i < NR_PLAYERS; ++i) {
    if (players[i].alive && players[i].valid) {
      idx = i;
      break;
    }
  }
  for (int i = idx + 1; i < NR_PLAYERS; ++i) {
    if (players[i].alive && players[i].valid) {
      if (players[i].property[key] < players[idx].property[key] ||
          players[i].property[key] == players[idx].property[key] &&
              strcmp(players[i].md5, players[idx].md5) < 0) {
        idx = i;
      }
    }
  }
  return &players[idx];
}

static Player *SelectEnemy(Player *att) {
  Player *ret = NULL;
  int i;
  for (i = 0; i < NR_PLAYERS; ++i) {
    if (players + i != att && players[i].alive && players[i].valid) {
      ret = players + i;
    }
  }
  for (; i < NR_PLAYERS; ++i) {
    if (players + i != att && players[i].alive && players[i].valid &&
        ret->property[HP] > players[i].property[HP]) {
      ret = players + i;
    }
  }
  return ret;
}

static void Damage(Player *src, Player *dst, int damage) {
  dst->property[HP] -= damage;
  src->damage += damage;
  if (dst->property[HP] <= 0) {
    printf("  %s was defeated.\n", dst->name);
    dst->alive = false;
    src->kill_cnt++;
  }
}

static void Attack(Player *src, Player *dst) {
  printf("%s attacked %s, ", src->name, dst->name);
  int damage = 0;
  bool miss =
      ((rand() % 100) > src->property[DEX] + (src->property[ATT] >> 3)) &&
      ((rand() % 100) < (dst->property[DEX] >> 2) + (dst->property[ATT] >> 4));
  if (miss) {
    printf("%s avoid the attack.\n", dst->name);
  } else {
    if (src->property[MAG] >= src->property[ATT] &&
        rand() % 100 < src->property[LER]) {
      damage = src->property[MAG] + (rand() & 0xf) - 8;
    } else {
      damage = src->property[ATT] + (rand() & 0xf) - 8;
    }
    printf("%s suffered %d damage.\n", dst->name, damage);
    Damage(src, dst, damage);
  }
  UpdateNxAtt(src);
}

void StartGame() {
  while (CountAlive() != 1) {
    Player *src = SelectMin(NXATT);
    Player *dst = SelectEnemy(src);
    Attack(src, dst);
  }
  PrintResult();
}