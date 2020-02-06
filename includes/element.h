#ifndef __ELEMENT_H__
#define __ELEMENT_H__

typedef enum ELEMENT {
  FIRE,
  WATER,
  WIND,
  EARTH,
  LIFE,
  EMPTY
} Element;

// 消去可能な宝石の並びの情報
typedef struct BANISH_INFO
{
  Element elem;
  int startIndex;
  int len;
} BanishInfo;

// 属性別の記号
extern const char ELEMENT_SYMBOLS[];
// 属性別のカラーコード（ディスプレイ制御シーケンス用）
extern const int ELEMENT_COLORS[];
// バトルフィールド宝石最大数
enum { MAX_GEM = 14 };
//
extern const double ELEMENT_BOOST[EMPTY + 1][EMPTY + 1];

#endif
