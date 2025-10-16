// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower) {}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength()) {}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0)
        throw invalid_argument("Номер не может быть отрицательным");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0)
        throw invalid_argument("Номер не может быть отрицательным");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0)
        throw invalid_argument("Номер не может быть отрицательным");
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (MaxPower != s.MaxPower)
        return 0;
    if (BitField != s.BitField)
        return 0;
    return 1;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    if (MaxPower != s.MaxPower)
        return 1;
    if (BitField != s.BitField)
        return 1;
    return 0;
}

TSet TSet::operator+(const TSet& s) // объединение
{
    int NewMaxPower;
    if (MaxPower < s.MaxPower)
        NewMaxPower = s.MaxPower;
    else NewMaxPower = MaxPower;
    TSet result(MaxPower);
    result = BitField | s.BitField;
    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0)
        throw invalid_argument("Номер не может быть отрицательным");
    TSet result(MaxPower);
    result.BitField = BitField;
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0)
        throw invalid_argument("Номер не может быть отрицательным");
    TSet result(MaxPower);
    result.BitField = BitField;
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    int NewMaxPower;
    if (MaxPower < s.MaxPower)
        NewMaxPower = s.MaxPower;
    else NewMaxPower = MaxPower;
    TSet result(NewMaxPower);
    result.BitField = this->BitField & s.BitField;
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int elem;
    char ch;
    for (int i = 0; i < s.MaxPower; i++)
        s.DelElem(i);
    while (istr >> elem)
    {
        if (elem >= 0 && elem < s.MaxPower)
            s.InsElem(elem);

        ch = istr.peek();
        if (ch == '\n' || ch == EOF)
            break;
    }
    return istr;

}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    bool first = true;
    for (int i = 0; i < s.MaxPower; i++)
    {
        if (s.IsMember(i))
        {
            if (!first) {
                ostr << ", ";
            }
            ostr << i;
            first = false;
        }
    }
    return ostr;
}
