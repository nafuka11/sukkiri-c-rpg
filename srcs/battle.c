#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "io.h"
#include "battle.h"

/**
 * @brief バトル開始から終了
 * @param pParty
 * @param pMonster
 * @return int 1=勝利 0=敗北
 */
int doBattle(Party* pParty, Monster* pMonster)
{
  printMonsterName(pMonster);
  printf("が現れた！\n\n");

  // バトルフィールドの初期化
  BattleField field = {
    .pParty = pParty,
    .pEnemy = pMonster,
  };
  fillGems(field.gems, 0);

  while(true) {
    onPlayerTurn(&field);
    // 撃破判定
    if (pMonster->hp <= 0) {
      printMonsterName(pMonster);
      printf("を倒した！\n");
      return 1;
    }
    onEnemyTurn(&field);
    // 敗北判定
    if (pParty->hp <= 0) {
      printf("%sは倒れた...\n", pParty->name);
      return 0;
    }
  }
}
/**
 * @brief 入力コマンドのバリデーション
 * @param input
 * @return true
 * @return false
 */
bool checkValidCommand(char* input)
{
  if (strlen(input) != 2) {
    return false;
  }
  if (input[0] == input[1]) {
    return false;
  }
  int maxCode = 'A' + MAX_GEM - 1;
  return ((input[0] >= 'A') && (input[0] <= maxCode) && (input[1] >= 'A') && input[1] <= maxCode);
}

/**
 * @brief プレイヤーターン
 * @param pField
 */
void onPlayerTurn(BattleField* pField)
{
  printf("【%sのターン】\n", pField->pParty->name);
  showBattleField(pField);

  // 入力ループ
  int src = 0;
  int dest = 0;
  while (true) {
    // 入力文字数超過判定をするため、2+1文字読み込み。残りは改行を除き読み飛ばす
    printf("コマンド？> ");
    char inputStr[3] = {};
    if (scanf("%3[^\n]%*[^\n]", inputStr) == EOF) {
      continue;
    }
    // 改行の読み飛ばし
    scanf("%*c");
    if (checkValidCommand(inputStr)) {
      src = inputStr[0] - 'A';
      dest = inputStr[1] - 'A';
      break;
    }
  }
  // 宝石の移動・評価
  moveGem(pField->gems, src, dest, 1);
  evaluateGems(pField);
}

/**
 * @brief プレイヤーの攻撃
 * @param pField
 * @param pInfo 消去した宝石の情報
 * @param combo コンボ数
 */
void doAttack(BattleField* pField, BanishInfo* pInfo, int combo)
{
  // 攻撃モンスターの設定
  for (int i = 0; i < pField->pParty->monsterSize; i++) {
    if (pField->pParty->monsters[i].elem == pInfo->elem) {
      Monster* pMonster = &(pField->pParty->monsters[i]);
      // ダメージ処理
      int damage = calcAttackDamge(pField, pMonster, pInfo, combo);
      printMonsterName(pMonster);
      printf("の攻撃！ ");
      if (combo > 1) {
        printCombo(combo);
      }
      printf("\n");
      printf("%sに%dのダメージを与えた\n\n", pField->pEnemy->name, damage);
      pField->pEnemy->hp -= damage;
      break;
    }
  }
}

/**
 * @brief 敵のターン
 * @param pField
 */
void onEnemyTurn(BattleField* pField)
{
  printf("【%sのターン】\n", pField->pEnemy->name);
  doEnemyAttack(pField);
}

/**
 * @brief 敵の攻撃
 * @param pField
 */
void doEnemyAttack(BattleField* pField)
{
  int damage = calcEnemyAttackDamage(pField);
  printMonsterName(pField->pEnemy);
  printf("の攻撃！");
  printf("%dのダメージを受けた\n\n", damage);
  pField->pParty->hp -= damage;
}

/**
 * @brief バトルフィールドの宝石の評価
 * @param pField
 */
void evaluateGems(BattleField* pField)
{
  int combo = 0;
  while (true) {
    BanishInfo banishInfo = checkBanishable(pField->gems);
    if (banishInfo.len > 0) {
      combo++;
      banishGems(pField, &banishInfo, combo);
      shiftGems(pField->gems, banishInfo.startIndex, banishInfo.len);
      spawnGems(pField->gems, banishInfo.len);
    } else {
      break;
    }
  }
}

/**
 * @brief 消去可能な宝石を検索
 * @param gems
 * @return BanishInfo
 */
BanishInfo checkBanishable(Element* gems)
{
  const int BANISH_GEMS = 3;
  // 見つからない場合の値
  BanishInfo result = {.elem = EMPTY, .startIndex = 0, .len = 0};

  int startIndex = 0;
  int len = 1;

  for (int i = 1; i < MAX_GEM; i++) {
    if (gems[i-1] == gems[i]) {
      len++;
    } else {
      if (len >= BANISH_GEMS) {
        break;
      }
      startIndex = i;
      len = 1;
    }
    if (len >= BANISH_GEMS) {
      // 3個以上なら消去可能として返す。左側から優先して評価
      result.elem = gems[startIndex];
      result.startIndex = startIndex;
      result.len = len;
    }
  }
  return result;
}

/**
 * @brief 消去可能なスロットの宝石を消滅
 * @param pField
 * @param pBInfo
 * @param combo
 */
void banishGems(BattleField *pField, BanishInfo* pBInfo, int combo)
{
  Element elm = pField->gems[pBInfo->startIndex];
  // 宝石の消去
  for (int i = 0; i < pBInfo->len; i++) {
    pField->gems[i + pBInfo->startIndex] = EMPTY;
  }
  // 宝石の表示
  printGems(pField->gems);
  // 回復 or 攻撃の発動
  if (elm == LIFE) {
      doRecover(pField, pBInfo, combo);
  } else {
      doAttack(pField, pBInfo, combo);
  }
}

/**
 * @brief 宝石の空きスロットを右詰する
 * @param gems
 * @param startIndex
 * @param len
 */
void shiftGems(Element* gems, int startIndex, int len)
{
  moveGem(gems, startIndex, MAX_GEM - len, len);
}

/**
 * @brief 宝石の空きスロットにランダムな宝石を生成する
 * @param gems
 * @param len
 */
void spawnGems(Element* gems, int len)
{
  fillGems(gems, MAX_GEM - len);
  printGems(gems);
}

/**
 * @brief パーティのHP回復処理
 * @param pField
 * @param pBInfo
 * @param combo
 */
void doRecover(BattleField* pField, BanishInfo* pBInfo, int combo)
{
  printf("%sは命の宝石を使った! ", pField->pParty->name);
  if (combo > 1) {
    printCombo(combo);
  }
  printf("\n");
  int damage = calcRecoverDamage(pField, pBInfo, combo);
  printf("HPが%d回復した！\n", damage);
  pField->pParty->hp += damage;
}

/*** ユーティリティ関数 ***/

/**
 * @brief 引数のスロットをランダムな宝石で埋める
 * @param gems
 * @param startIndex
 */
void fillGems(Element* gems, int startIndex)
{
  for (int i = startIndex; i < MAX_GEM; i++) {
    gems[i] = rand() % EMPTY;
  }
}

/**
 * @brief 指定位置の指定個の宝石を別の指定位置へ移動させる
 * @param gems
 * @param src
 * @param dest
 * @param len
 */
void moveGem(Element* gems, int src, int dest, int len)
{
  printGems(gems);
  int step = (src < dest) ? 1 : -1;
  for (int i = src; i != dest; i += step) {
    // 複数宝石のループ
    for (int k = len - 1; k >= 0; k--) {
      swapGem(gems, i+k, step);
    }
    printGems(gems);
  }
}

/**
 * @brief 指定位置の宝石を指定した方向の隣の宝石と入れ替える
 * @param gems
 * @param index
 * @param step
 */
void swapGem(Element* gems, int index, int step)
{
  Element tmp = gems[index];
  gems[index] = gems[index + step];
  gems[index + step] = tmp;
}

/**
 * @brief ±指定%の範囲でランダムなダメージを返す
 * @param damage
 * @param percent
 * @return int
 */
int blurDamage(int damage, int percent)
{
  int boost = rand() % (percent * 2 + 1) - percent + 100;
  int result = damage * boost / 100;
  if (result <= 0) {
    return 1;
  }
  return result;
}

/**
 * @brief 敵モンスターの攻撃ダメージを算出
 * @param pField
 * @return int 攻撃ダメージ
 */
int calcEnemyAttackDamage(BattleField* pField)
{
  int damage = pField->pEnemy->attack - pField->pParty->defence;
  return blurDamage(damage, 10);
}

/**
 * @brief 味方モンスターの攻撃ダメージを算出
 * @param pField
 * @param pMonster
 * @param pInfo
 * @param combo
 * @return int 攻撃ダメージ
 */
int calcAttackDamge(BattleField* pField, Monster* pMonster, BanishInfo* pInfo, int combo)
{
  int damage = (pMonster->attack - pField->pEnemy->defence)
               * ELEMENT_BOOST[pMonster->elem][pField->pEnemy->elem]
               * pow(1.5, pInfo->len - 3 + combo);
  printf("ダメージ = (味方攻撃力%d - 敵防御力%d) * 属性補正%.2f * コンボ補正%.2f\n",
         pMonster->attack,
         pField->pEnemy->defence,
         ELEMENT_BOOST[pMonster->elem][pField->pEnemy->elem],
         pow(1.5, pInfo->len - 3 + combo));
  return blurDamage(damage, 10);
}

/**
 * @brief パーティのHP回復量を算出
 * @param pField
 * @param pBInfo
 * @param combo
 * @return int HP回復量
 */
int calcRecoverDamage(BattleField* pField, BanishInfo* pBInfo, int combo)
{
  int damage = 20.0 * pow(1.5, pBInfo->len - 3 + combo);
  printf("ダメージ = 20 * コンボ補正%.2f\n",
         pow(1.5, pBInfo->len - 3 + combo));
  damage = blurDamage(damage, 10);
  // HP最大値を上限に回復する
  int canRecover = pField->pParty->maxHp - pField->pParty->hp;
  if (canRecover < damage) {
    damage = canRecover;
  }
  return damage;
}
