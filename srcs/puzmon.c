#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "element.h"
#include "monster.h"
#include "dungeon.h"
#include "party.h"
#include "io.h"

int main(int argc, char** argv)
{
  if (argc <= 1) {
    printf("エラー: プレイヤー名を指定して起動してください\n");
    return 1;
  }

  srand((unsigned)time(NULL));
  printf("*** Puzzle & Monsters ***\n");

  Party party = initParty(argv[1]);
  Dungeon dungeon = initDungeon();

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
