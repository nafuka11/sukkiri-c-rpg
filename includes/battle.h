#ifndef __BATTLE_H__
#define __BATTLE_H__

#include <stdbool.h>
#include "party.h"
#include "monster.h"
#include "element.h"

// バトルフィールド
typedef struct BATTLE_FIELD
{
  Party* pParty;
  Monster* pEnemy;
  Element gems[MAX_GEM];
} BattleField;

int doBattle(Party* pParty, Monster* pMonster);
bool checkValidCommand(char* input);
void onPlayerTurn(BattleField* pField);
void doAttack(BattleField* pField, BanishInfo* pInfo, int combo);
void onEnemyTurn(BattleField* pField);
void doEnemyAttack(BattleField* pField);

void evaluateGems(BattleField* pField);
BanishInfo checkBanishable(Element* gems);
void banishGems(BattleField *pField, BanishInfo* banishInfo, int combo);
void shiftGems(Element* gems, int startIndex, int len);
void spawnGems(Element* gems, int len);
void doRecover(BattleField* field, BanishInfo* pBInfo, int combo);

// ユーティリティ関数
void fillGems(Element* gems, int startIndex);
void moveGem(Element* gems, int src, int dest, int len);
void swapGem(Element* gems, int index, int step);
int blurDamage(int damage, int range);
int calcEnemyAttackDamage(BattleField* pField);
int calcAttackDamge(BattleField* pField, Monster* pMonster, BanishInfo* pInfo, int combo);
int calcRecoverDamage(BattleField* pField, BanishInfo* pBInfo, int combo);

#endif
