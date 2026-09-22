// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>
#include <algorithm> // для std::max

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw 1;
    BitLen = len;
    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
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
    if (pMem) {
        delete[] pMem;
        BitLen = 0;
        MemLen = 0;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) throw 2;
    return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) throw 3;
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
    if (n < 0 || n >= BitLen) throw 4;
    int id = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[id] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) throw 4;
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
        if (MemLen != bf.MemLen) {
            MemLen = bf.MemLen;
            delete[] pMem;
            pMem = new TELEM[MemLen];
        }
        BitLen = bf.BitLen;
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (bf.BitLen != BitLen) {
        return false;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return false;
        }
    }
    return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField res(max((*this).BitLen, bf.BitLen));
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        res.pMem[i] |= bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField res(max((*this).BitLen, bf.BitLen));
    int minMemLen = min(MemLen, bf.MemLen);
    for (int i = 0; i < minMemLen; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void ) // отрицание
{
    TBitField res = *this;
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = ~res.pMem[i];
    }

    // Чистим хвост, чтобы незначащие элементы оставались 0
    int bits_last = BitLen % (sizeof(TELEM) * 8);
    if (bits_last != 0) {
        int bits_trash = (sizeof(TELEM) * 8) - bits_last;
        TELEM mask = ~(TELEM)0 >> bits_trash; // Реверсивная маска
        res.pMem[MemLen - 1] &= mask; // Хвост снова нули
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char tmp;
    istr >> tmp;
    int i = 0;
    while (tmp == '0' || tmp == '1') {
        if (tmp == '0') bf.ClrBit(i);
        else bf.SetBit(i);
        i++;
        istr >> tmp;
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
