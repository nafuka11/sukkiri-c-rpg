#include "party.h"

/**
 * @brief パーティの編成処理
 * @param playerName
 * @param monsters
 * @param monsterSize
 * @return Party
 */
Party organizeParty(char* playerName, Monster monsters[], int monsterSize)
{
  int maxHpSum = 0;
  int defenceSum = 0;
  for (int i = 0; i < monsterSize; i++) {
    maxHpSum += monsters[i].maxHp;
    defenceSum += monsters[i].defence;
  }
  Party party = {
    .name = playerName,
    .monsters = monsters,
    .monsterSize = monsterSize,
    .hp = maxHpSum,
    .maxHp = maxHpSum,
    .defence = defenceSum / monsterSize
  };
  return party;
}
