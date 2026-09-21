// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw 1;
    BitLen = len;
    MemLen = BitLen / (sizeof(TELEM) * 8) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    if (!pMem) {
        delete[] pMem;
        BitLen = 0;
        MemLen = 0;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0) throw 2;
    return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0) throw 3;
    TELEM res = 1;
    int idBit = n % (sizeof(TELEM) * 8);
    res <<= idBit;
    return res;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n <= 0 || n > BitLen) throw 4;
    int id = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[id] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n <= 0 || n >= BitLen) throw 4;
    int id = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[id] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw 4;
    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    TELEM res = pMem[idx];
    res &= mask;
    if (res == 0) {
        return 0;
    } else {
        return 1;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        if (MemLen != bf.MemLen) {
            delete[] pMem;
            pMem = new TELEM[MemLen];
        }
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  return FAKE_INT;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return FAKE_INT;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator~(void) // отрицание
{
    return FAKE_BITFIELD;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int val;
    while (true) {
        istr >> val;
        if (!istr) break;
        bf.SetBit(val);
    }
    if (istr.eof()) {
        istr.clear();
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
        if (i % 31 == 0 && i != 0) ostr << " ";
    }
    return ostr;
}
