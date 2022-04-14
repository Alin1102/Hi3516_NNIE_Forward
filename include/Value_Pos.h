/******************************************************************************
  File Name     : Value_Pos.h
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : Value_Pos template struct
******************************************************************************/

#ifndef VALUE_POS_H
#define VALUE_POS_H

// Value Pos 用于一些分数、优先级等的排序操作
template<class T>
struct Value_Pos {
	T value;
	int pos;
	Value_Pos(T v, int p) {
		value = v;
		pos = p;
	}
};

template<class T>
bool cmpLarge2Small(Value_Pos<T> v1, Value_Pos<T> v2) {
	return v1.value > v2.value;
}


#endif /* __VALUE_POS_H__ */