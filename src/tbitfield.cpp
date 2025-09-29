// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>

const int BITS_BYTE = 8;
const int BITS_BLOCK = BITS_BYTE * sizeof(TELEM);

TBitField::TBitField(int len){
  if (len < 0)
    throw "Negative length";
  BitLen = len;
  if (BitLen / BITS_BLOCK + (BitLen % BITS_BLOCK != 0)){
    MemLen = 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i)
    pMem[i] = 0;
  }
  else{
    MemLen = 0;
    pMem = nullptr;
  }
}


TBitField::TBitField(const TBitField &bf){
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  if (MemLen != 0){
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i)
      pMem[i] = bf.pMem[i];
  }
  else
    pMem = nullptr;
}

TBitField::~TBitField(){
  if (pMem != nullptr)
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const {
  if (n < 0 || n >= BitLen)
    throw "Out of range";
  return n / BITS_BLOCK;
}

TELEM TBitField::GetMemMask(const int n) const {
  if (n < 0 || n >= BitLen)
    throw "Out of range";
  return (1 << (n % BITS_BLOCK));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const {
  return BitLen;
}

void TBitField::SetBit(const int n){
  if (n < 0 || n >= BitLen)
    throw "Out of range";
  int memIndex = GetMemIndex(n);
  int mask = GetMemMask(n);
  pMem[memIndex] |= mask;
}

void TBitField::ClrBit(const int n) {
  if (n < 0 || n >= BitLen)
    throw "Out of range";
  int memIndex = GetMemIndex(n);
  int mask = GetMemMask(n);
  pMem[memIndex] &= ~mask;
}

int TBitField::GetBit(const int n) const {
  if (n < 0 || n >= BitLen)
    throw "Out of range";
  int memIndex = GetMemIndex(n);
  return (pMem[memIndex] >> (n % BITS_BLOCK)) & 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf){
  if (this == &bf)
    return *this;
  if (pMem != nullptr)
    delete[] pMem;
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  if (MemLen != 0){
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i)
      pMem[i] = bf.pMem[i];
  }
  else
    pMem = nullptr;
  return *this;
}

int TBitField::operator==(const TBitField &bf) const {
  if (BitLen != bf.BitLen || MemLen != bf.MemLen)
    return 0;
  for (int i = 0; i < MemLen; ++i)
    if (pMem[i] != bf.pMem[i])
      return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const {
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) {
  int maxBitLen = std::max(BitLen, bf.BitLen);
  TBitField newBitField(maxBitLen);
  for (int i = 0; i < MemLen; ++i)
    newBitField.pMem[i] |= pMem[i];
  for (int i = 0; i < bf.MemLen; ++i)
    newBitField.pMem[i] |= bf.pMem[i];
  return newBitField;
}

TBitField TBitField::operator&(const TBitField &bf){
  int maxBitLen = std::max(BitLen, bf.BitLen);
  int minMemLen = std::min(MemLen, bf.MemLen);

  TBitField newBitField(maxBitLen);
  for (int i = 0; i < minMemLen; ++i)
    newBitField.pMem[i] = pMem[i] & bf.pMem[i];
  return newBitField;
}

TBitField TBitField::operator~(void){
  TBitField newBitField(BitLen);
  for (int i = 0; i < BitLen; ++i)
    if (!GetBit(i))
      newBitField.SetBit(i); 
  return newBitField;
}

// ввод/вывод

istream& operator>>(istream &istr, TBitField &bf) {
  string bitString;
  istr >> bitString;
  for (int i = 0; i < bitString.length(); ++i) {
    char c = bitString[i];
    if (c != '0' && c != '1')
      throw "Invalid bit string";
  }  
  TBitField newBitField(bitString.size());
  for (int i = 0; i < bitString.size(); ++i)
      if (bitString[i] == '1')
      newBitField.SetBit(i);
  bf = newBitField;
  return istr;
}

ostream& operator<<(ostream &ostr, const TBitField &bf) {
  for (int i = 0; i < bf.BitLen; ++i)
    ostr << bf.GetBit(i);
  return ostr;
}
