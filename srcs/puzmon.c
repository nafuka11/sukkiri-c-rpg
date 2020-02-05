#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "element.h"
#include "monster.h"
#include "dungeon.h"
#include "party.h"

int main(int argc, char** argv)
{
  if (argc <= 1) {
    printf("エラー: プレイヤー名を指定して起動してください\n");
    return 1;
  }

  srand((unsigned)time(NULL));
  printf("*** Puzzle & Monsters ***\n");

  // パーティの準備
  Monster partyMonsters[4] = {
    {.name = "朱雀", .hp = 150, .maxHp = 150, .elem = FIRE, .attack = 25, .defence = 10},
    {.name = "青龍", .hp = 150, .maxHp = 150, .elem = WIND, .attack = 15, .defence = 10},
    {.name = "白虎", .hp = 150, .maxHp = 150, .elem = EARTH, .attack = 20, .defence = 5},
    {.name = "玄武", .hp = 150, .maxHp = 150, .elem = WATER, .attack = 20, .defence = 15},
  };
  Party party = organizeParty(argv[1], partyMonsters, 4);

  // ダンジョンの準備
  Monster dungeonMonsters[5] = {
    {.name = "スライム", .hp = 100, .maxHp = 100, .elem = WATER, .attack = 10, .defence = 5},
    {.name = "ゴブリン", .hp = 200, .maxHp = 200, .elem = EARTH, .attack = 20, .defence = 15},
    {.name = "オオコウモリ", .hp = 300, .maxHp = 300, .elem = WIND, .attack = 30, .defence = 25},
    {.name = "ウェアウルフ", .hp = 400, .maxHp = 400, .elem = WIND, .attack = 40, .defence = 30},
    {.name = "ドラゴン", .hp = 800, .maxHp = 800, .elem = FIRE, .attack = 50, .defence = 40}
  };
  Dungeon dungeon = {
    .monsters = dungeonMonsters,
    .monsterSize = 5
  };

  // ダンジョンへ
  int winCount = goDungeon(&party, &dungeon);

  // 冒険終了後
  if (winCount == dungeon.monsterSize) {
    printf("%sはダンジョンを制覇した！\n", party.name);
    printf("*** GAME CLEARD! ***\n");
  } else
  {
    printf("*** GAME OVER***\n");
  }

  printf("倒したモンスター数=%d\n", winCount);

  return 0;
}
