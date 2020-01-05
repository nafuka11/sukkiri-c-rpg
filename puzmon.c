/*** include宣言 ***/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

/*** 列挙型宣言 ***/
// 属性
typedef enum ELEMENT {
  FIRE,
  WATER,
  WIND,
  EARTH,
  LIFE,
  EMPTY
} Element;

/*** グローバル定数の宣言 ***/
// 属性別の記号
const char ELEMENT_SYMBOLS[] = {'$', '~', '@', '#', '&', ' '};
// 属性別のカラーコード（ディスプレイ制御シーケンス用）
const int ELEMENT_COLORS[] = {1, 6, 2, 3, 5, 0};
// バトルフィールド宝石最大数
enum { MAX_GEM = 14 };
//
const double ELEMENT_BOOST[EMPTY + 1][EMPTY + 1] = {
  // FIRE WATER WIND EARTH LIFE EMPTY
  {1.0, 0.5, 2.0, 1.0, 1.0, 1.0}, // FIRE
  {2.0, 1.0, 1.0, 0.5, 1.0, 1.0}, // WATER
  {0.5, 1.0, 1.0, 2.0, 1.0, 1.0}, // WIND
  {1.0, 2.0, 0.5, 1.0, 1.0, 1.0}, // EARTH
  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, // LIFE
  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}  // EMPTY
};

/*** 構造体型宣言 ***/
// モンスター
typedef struct MONSTER
{
  char* name;
  int hp;
  int maxHp;
  Element elem;
  int attack;
  int defence;
} Monster;

// ダンジョン
typedef struct DUNGEON
{
  Monster* monsters;
  const int monsterSize;
} Dungeon;

// パーティ
typedef struct PARTY
{
  char* name;
  Monster* monsters;
  const int monsterSize;
  int hp;
  int maxHp;
  int defence;
} Party;

// バトルフィールド
typedef struct BATTLE_FIELD
{
  Party* pParty;
  Monster* pEnemy;
  Element gems[MAX_GEM];
} BattleField;

// 消去可能な宝石の並びの情報
typedef struct BANISH_INFO
{
  Element elem;
  int startIndex;
  int len;
} BanishInfo;

/*** プロトタイプ宣言 ***/
int goDungeon(Party* pParty, Dungeon* pDungeon);
int doBattle(Party* pParty, Monster* pMonster);
Party organizeParty(char* name, Monster monster[], int monsterSize);
void showParty(Party* pParty);
void showBattleField(BattleField* pField);
void onPlayerTurn(BattleField* pField);
void doAttack(BattleField* pField, BanishInfo* pInfo, int combo);
void onEnemyTurn(BattleField* pField);
void doEnemyAttack(BattleField* pField);
bool checkValidCommand(char* input);
void evaluateGems(BattleField* pField);
BanishInfo checkBanishable(Element* gems);
void banishGems(BattleField *pField, BanishInfo* banishInfo, int combo);
void shiftGems(Element* gems, int startIndex, int len);
void spawnGems(Element* gems, int len);
void doRecover(BattleField* field, BanishInfo* pBInfo, int combo);

// ユーティリティ関数
void printMonsterName(Monster* monster);
void fillGems(Element* gems, int startIndex);
void printGems(Element* gems);
void printGem(Element gem);
void printCombo(int combo);
void moveGem(Element* gems, int src, int dest, int len);
void swapGem(Element* gems, int index, int step);
int blurDamage(int damage, int range);
int calcEnemyAttackDamage(BattleField* pField);
int calcAttackDamge(BattleField* pField, Monster* pMonster, BanishInfo* pInfo, int combo);
int calcRecoverDamage(BattleField* pField, BanishInfo* pBInfo, int combo);

/*** 関数宣言 ***/
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

// ダンジョン開始から終了
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

// バトル開始から終了
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

// パーティの編成処理
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

// パーティ情報の表示
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

// プレイヤーターン
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

// プレイヤーの攻撃
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

// 敵のターン
void onEnemyTurn(BattleField* pField)
{
  printf("【%sのターン】\n", pField->pEnemy->name);
  doEnemyAttack(pField);
}

// 敵の攻撃
void doEnemyAttack(BattleField* pField)
{
  int damage = calcEnemyAttackDamage(pField);
  printMonsterName(pField->pEnemy);
  printf("の攻撃！");
  printf("%dのダメージを受けた\n\n", damage);
  pField->pParty->hp -= damage;
}

// バトルフィールド情報の表示
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

// 入力コマンドのバリデーション
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

// バトルフィールドの宝石の評価
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

// 消去可能な宝石を検索
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

// 消去可能なスロットの宝石を消滅
void banishGems(BattleField *pField, BanishInfo* pBInfo, int combo)
{
  Element elm = pField->gems[pBInfo->startIndex];
  // 宝石の消去
  for (int i = 0; i < pBInfo->len; i++) {
    pField->gems[i + pBInfo->startIndex] = EMPTY;
  }
  // 宝石の表示
  printGems(pField->gems);
  // 攻撃の発動
  if (elm == LIFE) {
      doRecover(pField, pBInfo, combo);
  } else {
      doAttack(pField, pBInfo, combo);
  }
}

// 宝石の空きスロットを右詰する
void shiftGems(Element* gems, int startIndex, int len)
{
  moveGem(gems, startIndex, MAX_GEM - len, len);
}

// 宝石の空きスロットにランダムな宝石を生成する
void spawnGems(Element* gems, int len)
{
  fillGems(gems, MAX_GEM - len);
  printGems(gems);
}

// パーティのHP回復処理
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

/*** ユーティリティ関数宣言 ***/
// モンスター名のカラー表示
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

// コンボ数を表示する
void printCombo(int combo)
{
  printf("\x1b[44m\x1b[37m%d COMBO!\x1b[0m", combo);
}

// 引数のスロットをランダムな宝石で埋める
void fillGems(Element* gems, int startIndex)
{
  for (int i = startIndex; i < MAX_GEM; i++) {
    gems[i] = rand() % EMPTY;
  }
}

// スロットに並ぶ宝石を表示
void printGems(Element* gems)
{
  for (int i = 0; i < MAX_GEM; i++) {
    printf(" ");
    printGem(gems[i]);
  }
  printf("\n");
}

// 1個の宝石を表示
void printGem(Element gem)
{
  printf("\x1b[4%dm", ELEMENT_COLORS[gem]);
  printf("\x1b[30m");
  printf("%c", ELEMENT_SYMBOLS[gem]);
  printf("\x1b[0m");
}

// 指定位置の指定個の宝石を別の指定位置へ移動させる
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

// 指定位置の宝石を指定した方向の隣の宝石と入れ替える
void swapGem(Element* gems, int index, int step)
{
  Element tmp = gems[index];
  gems[index] = gems[index + step];
  gems[index + step] = tmp;
}

// ±指定%の範囲でランダムなダメージを返す
int blurDamage(int damage, int percent)
{
  int boost = rand() % (percent * 2 + 1) - percent + 100;
  int result = damage * boost / 100;
  if (result <= 0) {
    return 1;
  }
  return result;
}

// 敵モンスターの攻撃ダメージを算出
int calcEnemyAttackDamage(BattleField* pField)
{
  int damage = pField->pEnemy->attack - pField->pParty->defence;
  return blurDamage(damage, 10);
}

// 味方モンスターの攻撃ダメージを算出
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

// パーティのHP回復量を算出
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
