// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp), MaxPower(mp) {}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower) {}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength()) {}

TSet::operator TBitField() { return TBitField((*this).MaxPower); }

int TSet::GetMaxPower(void) const { return MaxPower; } // получить макс. к-во эл-тов

int TSet::IsMember(const int Elem) const { return (*this).BitField.GetBit(Elem); } // элемент множества?

void TSet::InsElem(const int Elem) { (*this).BitField.SetBit(Elem); } // включение элемента множества

void TSet::DelElem(const int Elem) { (*this).BitField.ClrBit(Elem); } // исключение элемента множества

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (MaxPower == s.MaxPower && BitField == s.BitField) {
        return 1;
    }
    return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(bool)(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return (TSet)(BitField|s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res(BitField);
    res.InsElem(Elem);
    return res;
    /*
   TSet temp(Elem);
   temp.InsElem(Elem);
   return (TSet)(BitField | temp.BitField);
   //return (TSet)(BitField|(TSet temp(ELem).InsElem(Elem));
   */
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet temp(Elem);
    temp.DelElem(Elem);
    return (TSet)(BitField | temp.BitField);
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return (TSet)(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return (TSet)(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    char ch;
    int el;
    istr >> ch;
    do {
        istr >> el >> ch;
        s.InsElem(el);
    } while (ch != '}');
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << '{';
    bool flag = false;
    for (int i = 0; i < s.MaxPower; i++) {
        if (flag == false) {
            ostr << i;
            flag = true;
        }
        if (s.IsMember(i)) {
            ostr << ", " << i;
        }
        
    }
    ostr << "}";
    return ostr;
}
