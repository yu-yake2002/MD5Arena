#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arena.h>
#include <player.h>

static const char *propertyArr[] = {"HP",  "ATT", "DEF", "SPD", "DEX",
                              "MAG", "RES", "LER", NULL};

void ConstructPlayer(Player *player, char *name, unsigned char *md5) {
  player->property_[HP] = 200 + md5[0];
  for (int i = 1; i < 8; ++i) {
    player->property_[i] = 35 + (md5[i] & 0x3f);
  }
  player->property_[SHD] = 0;
  player->property_[TOTHP] = player->property_[HP];
  player->property_[NXATT] = 0;
  UpdateNxAtt(player);
  player->skill_ = md5[15] & 0xf;
  
  player->kill_cnt_ = 0;
  player->damage_ = 0;
  strncpy(player->name_, name, NAME_LENGTH);
  memcpy(player->md5_, md5, 16);
  player->valid_ = true;
  player->alive_ = true;
}

void DestructPlayer(Player *player) {
  player->valid_ = false;
}

void UpdateNxAtt(Player *player) {
  player->property_[NXATT] += 200 - player->property_[SPD];
}

void PrintPlayerInfo(Player *player, int maxlen) {
  printf("%*s: ", maxlen, player->name_);
  for (int i = 0; i < 8; ++i) {
    printf("%s %d ", propertyArr[i], player->property_[i]);
  }
  printf("SKILL: %d", player->skill_);
  printf("\n");
}

int PlayerHeal(Player *player, int hp) {
  int ret = player->property_[HP];
  player->property_[HP] += hp;
  if (player->property_[HP] > player->property_[TOTHP]) {
    player->property_[HP] = player->property_[TOTHP];
  }
  return player->property_[HP] - ret;
}