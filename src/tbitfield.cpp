// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::invalid_argument("Длина поля должна быть неотрицательной");
    BitLen = len;
    MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
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
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Индекс бита вне поля");

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Индекс бита вне поля");

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Индекс бита вне поля");

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;

}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 1;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 1;
        }
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxBitLen = max(BitLen, bf.BitLen);
    TBitField result(maxBitLen);
    int minMemLen = min(MemLen, bf.MemLen);

    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }
    if (MemLen > minMemLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            result.pMem[i] = pMem[i];
        }
    }
    else if (bf.MemLen > minMemLen) {
        for (int i = minMemLen; i < bf.MemLen; i++) {
            result.pMem[i] = bf.pMem[i];
        }
    }

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxBitLen = max(BitLen, bf.BitLen);
    TBitField result(maxBitLen);
    int minMemLen = min(MemLen, bf.MemLen);

    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }
    int bitsInLast = BitLen % (sizeof(TELEM) * 8);
    if (bitsInLast != 0) {
        TELEM mask = (1 << bitsInLast) - 1;
        result.pMem[MemLen - 1] &= mask;
    }
    return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf)
{
    char ch;
    int bitCounter = 0;
    for (int i = 0; i < bf.MemLen; i++)
        bf.pMem[i] = 0;
    while (bitCounter < bf.BitLen && istr.get(ch))
    {
        if (ch == '1')
        {
            bf.SetBit(bitCounter);
            bitCounter++;
        }
        else if (ch == '0')
        {
            bf.ClrBit(bitCounter);
            bitCounter++;
        }
        else if (ch == ' ' || ch == '\t' || ch == '\n')
        {
            continue;
        }
    }
    return istr;
}
ostream& operator<<(ostream& ostr, const TBitField& bf)
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        ostr << (bf.GetBit(i) ? '1' : '0');
    }
    return ostr;
}