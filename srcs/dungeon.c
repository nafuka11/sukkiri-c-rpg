#include <stdio.h>
#include "dungeon.h"
#include "battle.h"
#include "display.h"

static Monster dungeonMonsters[5] = {
  {.name = "スライム", .hp = 100, .maxHp = 100, .elem = WATER, .attack = 10, .defence = 5},
  {.name = "ゴブリン", .hp = 200, .maxHp = 200, .elem = EARTH, .attack = 20, .defence = 15},
  {.name = "オオコウモリ", .hp = 300, .maxHp = 300, .elem = WIND, .attack = 30, .defence = 25},
  {.name = "ウェアウルフ", .hp = 400, .maxHp = 400, .elem = WIND, .attack = 40, .defence = 30},
  {.name = "ドラゴン", .hp = 800, .maxHp = 800, .elem = FIRE, .attack = 50, .defence = 40}
};

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

/**
 * @brief ダンジョン情報を初期化する
 * @return Dungeon ダンジョン情報
 */
Dungeon initDungeon(void)
{
  Dungeon dungeon = {
    .monsters = dungeonMonsters,
    .monsterSize = 5
  };
  return dungeon;
}
