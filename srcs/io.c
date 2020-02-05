#include <stdio.h>
#include "io.h"

/**
 * @brief パーティ情報の表示
 * @param pParty
 */
void showParty(Party* pParty)
{
  printf("＜パーティ編成＞-------------\n");
  for (int i = 0; i < pParty->monsterSize; i++) {
    printMonsterName(&(pParty->monsters[i]));
    printf(" HP= %d 攻撃= %d 防御= %d\n",
           pParty->monsters[i].hp,
           pParty->monsters[i].attack,
           pParty->monsters[i].defence);
  }
  printf("-----------------------------\n\n");
}

/**
 * @brief モンスター名を属性色で表示
 * @param pMonster
 */
void printMonsterName(Monster* pMonster)
{
  Element elem = pMonster->elem;
  printf("\x1b[3%dm", ELEMENT_COLORS[elem]);
  printf("%c%s%c",
         ELEMENT_SYMBOLS[elem],
         pMonster->name,
         ELEMENT_SYMBOLS[elem]);
  printf("\x1b[0m");
}

/**
 * @brief バトルフィールド情報の表示
 * @param pField
 */
void showBattleField(BattleField* pField)
{
  printf("-----------------------------\n\n");

  // 敵
  printf("\t  ");
  printMonsterName(pField->pEnemy);
  printf("\n\tHP= %4d / %4d\n\n", pField->pEnemy->hp, pField->pEnemy->maxHp);

  // パーティ
  for (int i = 0; i < pField->pParty->monsterSize; i++) {
    printMonsterName(&(pField->pParty->monsters[i]));
    printf("  ");
  }
  printf("\n\tHP= %4d / %4d\n", pField->pParty->hp, pField->pParty->maxHp);

  // 宝石
  printf("-----------------------------\n");
  for (int i = 0; i < MAX_GEM; i++) {
    printf(" %c", 'A' + i);
  }
  printf("\n");
  printGems(pField->gems);
  printf("-----------------------------\n");
}

/**
 * @brief スロットに並ぶ宝石の表示
 * @param gems
 */
void printGems(Element* gems)
{
  for (int i = 0; i < MAX_GEM; i++) {
    printf(" ");
    printGem(gems[i]);
  }
  printf("\n");
}

/**
 * @brief 1個の宝石を表示
 * @param gem
 */
void printGem(Element gem)
{
  printf("\x1b[4%dm", ELEMENT_COLORS[gem]);
  printf("\x1b[30m");
  printf("%c", ELEMENT_SYMBOLS[gem]);
  printf("\x1b[0m");
}

/**
 * @brief コンボ数を表示
 * @param combo
 */
void printCombo(int combo)
{
  printf("\x1b[44m\x1b[37m%d COMBO!\x1b[0m", combo);
}
