#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "party.h"
#include "monster.h"
#include "battle.h"
#include "dungeon.h"

void showParty(Party* pParty);
void printMonsterName(Monster* pMonster);
void showBattleField(BattleField* pField);
void printGems(Element* gems);
void printGem(Element gem);
void printCombo(int combo);
void printDungeonResult(int winCount, Party *party, Dungeon *dungeon);

#endif
