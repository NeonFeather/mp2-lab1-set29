// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp){}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField){}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf){}

TSet::operator TBitField(){
  return BitField;
}

int TSet::GetMaxPower(void) const {
  return BitField.GetLength();
}

int TSet::IsMember(const int Elem) const {
  return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem){
  BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem){
  BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s){
  BitField = s.BitField;
  return *this;
}

int TSet::operator==(const TSet& s) const {
  return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const {
  return BitField != s.BitField;
}

TSet TSet::operator+(const TSet& s){
  return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem){
  TSet newSet(*this);
  newSet.InsElem(Elem);
  return newSet;
}

TSet TSet::operator-(const int Elem){
  TSet newSet(*this);
  newSet.DelElem(Elem);
  return newSet;
}

TSet TSet::operator*(const TSet& s){
  return TSet(BitField & s.BitField);
}

TSet TSet::operator~(){
  return TSet(~BitField);
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s){
  int maxPower;
  istr >> maxPower;
  if (maxPower < 0)
    throw "error size";

  TSet temp(maxPower);

  int elemCount;
  istr >> elemCount;
  if (elemCount < 0 || elemCount >= maxPower)
    throw "error element count";

  // Читаем элементы множества
  for (int i = 0; i < elemCount; ++i){
    int elem;
    istr >> elem;
    if (elem < 0 || elem >= maxPower)
      throw "out of range";
    temp.InsElem(elem);
  }

  s = temp;
  return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s){
  for (int i = 0; i < s.GetMaxPower(); ++i)
    if (s.IsMember(i))
      ostr << i << ' ';
  return ostr;
}
