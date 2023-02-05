#ifndef SKILLS_H
#define SKILLS_H

// skills
enum {
  // I. Attacker's skills

  // I.1. at attack
  
  // I.1.1 Lethal Tempo. 致命节奏
  // 12.5% trigger posibility
  // Attacker can do 2~3 continuous attacks. 
  LETHAL_TEMPO, 
  
  // I.1.2 Coup de Grace. 致命一击
  // Deal 25% increased damage to enemy below 50% maximum health. 
  COUP_DE_GRACE, 

  // I.1.3 Last Stand. 坚毅不倒
  // Deal 12.5% increased damage to enemy when you're below 50% maximum health.
  LAST_STAND, 

  // I.1.4 Gathering Storm. 风暴聚集
  // Gain increasing adaptive force at time t that grants 3.125%t ATT. 
  GATHERING_STORM,
  
  // I.2. after attack
  
  // I.2.1 Triumph. 凯旋
  // You heals 25% of missing health after killing an enemy.
  TRIUMPH, 

  // I.2.2 Grasp of the Undying. 不灭之握
  // You heals 12.5% of total health after a successful attack. 
  GRASP_OF_THE_UNDYING, 

  // I.2.3 Conqueror. 征服者
  // You heal 25% of damage of your last attack. 
  CONQUEROR, 
  
  // I.2.4 Phase Rush. 相位猛冲
  // 50% trigger posibility
  // A sucessful attack grants you 20 bonus speed before next attack. 
  PHASE_RUSH,

  // I.2.5 Ultimate Hunter. 终极猎手
  // Earn 15 bonus speed whenever killing an enemy.
  ULTIMATE_HUNTER,

  // I.2.6 Hail of Blades. 丛刃
  // Own 20 extra speed before t=1000.
  HAIL_OF_BLADES, 
  
  // II. Defenders' skills
  
  // II.1 at attack
  
  // II.1.1 Unflinching. 坚定
  // Gain 6.25% (based on missing health) defence.
  UNFLINCHING, 

  // II.1.2 Conditioning. 调节
  // Gain 12 bonus defence after time == 600.
  CONDITIONING, 
  
  // II.2 after attack

  // II.2.1 Second Wind. 复苏之风
  // After taking damage from an enemy, gain 6.25% of your missing health.
  SECOND_WIND,
  
  // II.2.2 Nullifying Orb. 无效化之法球
  // After taking magic damage, gain a shield that absorbs 50 damage.
  NULLIFYING_ORB, 

  // III. Other skills
  // III.1 related to time
  // III.1.1 Biscuit Delivery. 饼干配送
  // Heal 30 HP at every attack at t=200, 400, 600.
  BISCUIT_DELIVERY, 

  // III.1.2 Magical Footwear. 神奇之鞋
  // Gain 10 bonus speed after t=500.
  MAGICAL_FOOTWEAR,
};

int AttackHandler();

#endif