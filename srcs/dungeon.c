#include <stdio.h>
#include "dungeon.h"
#include "battle.h"
#include "io.h"

/**
 * @brief ダンジョン開始から終了
 * @param pParty
 * @param pDungeon
 * @return int
 */
int goDungeon(Party* pParty, Dungeon* pDungeon)
{
  printf("%sのパーティ（HP=%d）はダンジョンに到着した\n", pParty->name, pParty->hp);

  showParty(pParty);

  // バトルループ
  int winCount = 0;
  for (int i = 0; i < pDungeon->monsterSize; i++) {
    winCount += doBattle(pParty, &(pDungeon->monsters[i]));
    if (pParty->hp <= 0) {
      printf("%sはダンジョンから逃げ出した...\n", pParty->name);
    } else {
      printf("%sはさらに奥へと進んだ\n\n", pParty->name);
      printf("=============================\n\n");
    }
  }
  return winCount;
}