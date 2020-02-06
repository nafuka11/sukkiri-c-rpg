#include "party.h"

static Monster partyMonsters[4] = {
  {.name = "朱雀", .hp = 150, .maxHp = 150, .elem = FIRE, .attack = 25, .defence = 10},
  {.name = "青龍", .hp = 150, .maxHp = 150, .elem = WIND, .attack = 15, .defence = 10},
  {.name = "白虎", .hp = 150, .maxHp = 150, .elem = EARTH, .attack = 20, .defence = 5},
  {.name = "玄武", .hp = 150, .maxHp = 150, .elem = WATER, .attack = 20, .defence = 15},
};

/**
 * @brief パーティの初期化
 * @param playerName プレイヤー名
 * @return Party
 */
Party initParty(char *playerName)
{
  return organizeParty(playerName, partyMonsters, 4);
}

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
