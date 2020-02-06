#include "element.h"

// 属性別の記号
const char ELEMENT_SYMBOLS[] = {'$', '~', '@', '#', '&', ' '};
// 属性別のカラーコード（ディスプレイ制御シーケンス用）
const int ELEMENT_COLORS[] = {1, 6, 2, 3, 5, 0};
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
