#ifndef __IO_H__
#define __IO_H__

#include "party.h"
#include "monster.h"
#include "battle.h"

void showParty(Party* pParty);
void printMonsterName(Monster* pMonster);
void showBattleField(BattleField* pField);
void printGems(Element* gems);
void printGem(Element gem);
void printCombo(int combo);

#endif
