#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "element.h"

typedef struct MONSTER
{
  char* name;
  int hp;
  int maxHp;
  Element elem;
  int attack;
  int defence;
} Monster;

#endif
