#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#include "party.h"
#include "monster.h"

typedef struct DUNGEON
{
  Monster* monsters;
  const int monsterSize;
} Dungeon;

int goDungeon(Party* pParty, Dungeon* pDungeon);
Dungeon initDungeon(void);

#endif
