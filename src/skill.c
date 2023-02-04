#include <stdio.h>
#include <stdlib.h>
#include <arena.h>
#include <skills.h>
#include <player.h>

static inline bool CalcMiss(int attmaj, int attmin, int defmaj, int defmin) {
  return ((rand() % 100) > attmaj + (attmin >> 3)) ||
         ((rand() % 100) < (defmaj >> 2) + (defmin >> 4));
}

static inline int CalcDamage(int att, int def) {
  return att + (rand() & 0xf) - (def >> 3);
}

// Return value:
// true: magical attack
// false: physical attack
static inline bool BeforeAttackHandler(Player *src, Player *dst) {
  return src->property_[MAG] >= src->property_[ATT] && rand() % 100 < src->property_[LER];
}

// Return value:
// Positive: physical damage
// Negative: magical damage
// Zero: missed
static int AtAttackHandler(Player *src, Player *dst, bool magic_att) {
  int attmaj, attmin, defmaj, defmin;
  if (magic_att) {
    attmaj = src->property_[MAG];
    attmin = src->property_[DEX];
    defmaj = dst->property_[RES];
    defmin = dst->property_[DEX];
  } else {
    attmaj = src->property_[ATT];
    attmin = src->property_[DEX];
    defmaj = dst->property_[DEF];
    defmin = dst->property_[DEX];
  }
  switch (src->skill_) {
  case COUP_DE_GRACE:
    if (dst->property_[HP] < (dst->property_[TOTHP] >> 1)) {
      printf("%s used COUP DE GRACE to attack %s.\n", src->name_, dst->name_);
      attmaj += attmaj >> 2;
    } else {
      printf("%s attacked %s.\n", src->name_, dst->name_);
    }
    break;
  case LAST_STAND:
    if (src->property_[HP] < (src->property_[TOTHP] >> 1)) {
      printf("%s used LAST STAND to attack %s.\n", src->name_, dst->name_);
      attmaj += attmaj >> 3;
    } else {
      printf("%s attacked %s.\n", src->name_, dst->name_);
    }
    break;
  case GATHERING_STORM:
    printf("%s used GATHERING STORM to gain attack on %s.\n", src->name_, dst->name_);
    attmaj += src->property_[NXATT] >> 5;
    break;
  default:
    printf("%s attacked %s.\n", src->name_, dst->name_);
    break;
  }
  switch (dst->skill_) {
  case UNFLINCHING:
    printf("  %s used UNFLINCHING for defense.\n", dst->name_);
    defmaj += (dst->property_[TOTHP] - dst->property_[HP]) >> 4;
    break;
  case CONDITIONING:
    if (src->property_[NXATT] >= 600) {
      printf("  %s used CONDITIONING for defense.\n", dst->name_);
      defmaj += 12;
    }
    break;
  default:
    break;
  }
  
  if (CalcMiss(attmaj, attmin, defmaj, defmin)) {
    return 0;
  } else {
    return magic_att ? -CalcDamage(attmaj, defmaj) : CalcDamage(attmaj, defmaj);
  }
}

// Return value:
// Actual damage on enemy
static int DamageHandler(Player *src, Player *dst, int damage) {
  int absorb_damage = 0;
  if (dst->property_[SHD] > 0) {
    if (damage > dst->property_[SHD]) {
      absorb_damage = dst->property_[SHD];
      dst->property_[SHD] = 0;
    } else {
      absorb_damage = damage;
      dst->property_[SHD] -= damage;
    }
    printf("  %s's shield absorbed %d damage.\n", dst->name_, absorb_damage);
  }
  damage -= absorb_damage;
  if (damage > dst->property_[HP]) {
    damage = dst->property_[HP];
  }
  if (damage) {
    printf("  %s suffered %d damage.\n", dst->name_, damage);
    dst->property_[HP] -= damage;
    src->damage_ += damage;
    if (dst->property_[HP] <= 0) {
      printf("  %s was defeated.\n", dst->name_);
      dst->alive_ = false;
      src->kill_cnt_++;
    }
  }
  return damage;
}

static void AfterAttackHandler(Player *src, Player *dst, int damage, bool magic_att) {
  if (damage == 0) {
    printf("  %s avoided the attack.\n", dst->name_);
    return;
  }
  int tmp;
  switch (src->skill_) {
  case TRIUMPH:
    if (!dst->alive_) {
      tmp = PlayerHeal(src, (src->property_[TOTHP] - src->property_[HP]) >> 2);
      printf("  %s used TRIUMPH, and healed %d HP.\n", src->name_, tmp);
    }
    break;
  case GRASP_OF_THE_UNDYING:
    tmp = PlayerHeal(src, src->property_[TOTHP] >> 3);
    printf("  %s used GRASP OF THE UNDYING, and healed %d HP.\n", src->name_, tmp);
    break;
  case CONQUEROR:
    tmp = PlayerHeal(src, damage >> 2);
    printf("  %s used CONQUEROR, and healed %d HP.\n", src->name_, tmp);
    break;
  case PHASE_RUSH:
    src->property_[NXATT] -= 10;
    printf("  %s used PHASE RUSH, and got 10 bonus speed before next attack.\n", src->name_);
    break;
  case ULTIMATE_HUNTER:
    if (!dst->alive_) {
      src->property_[SPD] += 15;
      printf("  %s used ULTIMATE HUNTER, and obtained 15 bonus speed.\n", src->name_);
    }
    break;
  case HAIL_OF_BLADES:
    if (src->property_[NXATT] <= 1000) {
      src->property_[NXATT] -= 20;
      printf("  %s used HAIL OF BLADES, and obtained 20 bonus speed.\n", src->name_);
    }
    break;
  default:
    break;
  }
  switch (dst->skill_) {
  case SECOND_WIND:
    if (dst->alive_) {
      tmp = PlayerHeal(dst, (dst->property_[TOTHP] - dst->property_[HP]) >> 4);
      printf("  %s used SECOND WIND, and healed %d HP.\n", dst->name_, tmp);
    }
    break;
  case NULLIFYING_ORB:
    if (magic_att) {
      printf("  %s used NULLIFYING ORB, obtaining a shield.\n", dst->name_);
      dst->property_[SHD] = 50;
    }
    break;
  default:
    break;
  }
}

static inline bool InRange(int l, int r, int val) {
  return l <= val && val < r;
}

static void TimeHandler(Player *src) {
  int prev_time = src->property_[NXATT];
  UpdateNxAtt(src);
  int next_time = src->property_[NXATT];
  switch (src->skill_)
  {
  case BISCUIT_DELIVERY:
    if (InRange(prev_time, next_time, 200) || InRange(prev_time, next_time, 400) || InRange(prev_time, next_time, 600)) {
      printf("%s used BISCUIT DELIVERY and healed %d HP.\n", src->name_, PlayerHeal(src, 30));
    }
    break;
  case MAGICAL_FOOTWEAR:
    if (InRange(prev_time, next_time, 500)) {
      printf("%s used MAGICAL FOOTWEAR and gained 10 speed.\n", src->name_);
      src->property_[SPD] += 10;
    }
    break;
  default:
    break;
  }
}

int AttackHandler() {
  char excp[2] = {0, 0};
  Player *src = SelectByOrder(NXATT, true, NULL);
  excp[0] = src->idx;
  Player *dst = SelectByOrder(HP, true, excp);
  int dmg;
  bool magic_att;
  switch (src->skill_) {
  case LETHAL_TEMPO:
    if (rand() & 0x3 == 0) {
      printf("%s used LETHAL TEMPO, and made continuous attacks!\n",
             src->name_);
      for (int i = 0; i < 3; ++i) {
        magic_att = BeforeAttackHandler(src, dst);
        dmg = AtAttackHandler(src, dst, magic_att);
        DamageHandler(src, dst, abs(dmg));
        AfterAttackHandler(src, dst, abs(dmg), dmg < 0);
        if (!dst->alive_) {
          // choose a living enemy
          dst = SelectByOrder(HP, true, excp); 
          if (dst == NULL) {
            break;
          }
        }
      }
      break;
    }
  default:
    magic_att = BeforeAttackHandler(src, dst);
    dmg = AtAttackHandler(src, dst, magic_att);
    DamageHandler(src, dst, abs(dmg));
    AfterAttackHandler(src, dst, abs(dmg), dmg < 0);
    break;
  }
  TimeHandler(src);
}
