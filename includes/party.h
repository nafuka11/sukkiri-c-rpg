#ifndef __PARTY_H__
#define __PARTY_H__

#include "monster.h"

typedef struct PARTY
{
  char* name;
  Monster* monsters;
  const int monsterSize;
  int hp;
  int maxHp;
  int defence;
} Party;

Party initParty(char *playerName);
Party organizeParty(char* name, Monster monster[], int monsterSize);

#endif
