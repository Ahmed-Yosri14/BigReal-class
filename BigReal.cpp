#include "BigReal.hpp"
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
bool BigReal::isValid(string &s) const
{
    if (s.empty()) return false;
    bool dot{};
    for (each c: s)
    {
        if (c == '.')
        {
            if (dot) return false;
            else dot = 1;
        }
        else if (!isdigit(c)) return false;
    }
    if (!dot) s += ".0";
    else if (s.front() == '.') s = '0'+ s;
    else if (s.back() == '.') s += '0';
    return true;
}


void BigReal::removeTrailingZeros()
{
    while (l.size() > 1 && !l.back()) l.pop_back();
    while (r.size() > 1 && !r.front()) r.pop_front();
}


string BigReal::getString() const
{
    string s;
    for (each d: r) s.push_back(d + '0');
    s.push_back('.');
    for (each d: l) s.push_back(d + '0');
    if (sign) s.push_back('-');
    reverse(all(s));
    return s;
}

BigReal::BigReal(const BigReal &src) : l{src.l}, r{src.r}, sign{src.sign}
{
}
BigReal::BigReal(string s)
{
    *this = s;
}
BigReal::BigReal(double d) : BigReal(to_string(d))
{
}
BigReal BigReal::operator=(const BigReal &src)
{
    l = src.l, r = src.r, sign = src.sign;
    return *this;
}
BigReal BigReal::operator=(string s)
{
    l.clear(), r.clear();
    sign = 0;
    if (!s.empty() && s.front() == '-') sign = 1, s.erase(0, 1);
    else if (!s.empty() && s.front() == '+') s.erase(0, 1);

    if (!isValid(s)) throw("Invalid decimal number");
    else
    {
        bool dot{};
        reverse(all(s));
        for (each c: s)
        {
            if (c == '.') dot = true;
            else if (dot) l.push_back(c - '0');
            else r.push_back(c - '0');
        }
    }
    removeTrailingZeros();
    return *this;
}
BigReal BigReal::operator=(double d)
{
    return *this = to_string(d);
}
bool BigReal::operator==(const BigReal &rhs) const
{
    if (sign != rhs.sign || l.size() != rhs.l.size() || r.size() != rhs.r.size()) return false;

    for (int i = 0; i < rhs.l.size(); ++i)
    {
        if (l[i] != rhs.l[i]) return false;
    }
    for (int i = 0; i < rhs.r.size(); ++i)
    {
        if (r[i] != rhs.r[i]) return false;
    }
    return true;
}
bool BigReal::operator!=(const BigReal &rhs) const
{
    return !(*this == rhs);
}
bool BigReal::operator>(const BigReal &rhs) const
{
    if (sign != rhs.sign) return !sign;
    if (rhs.l.size() != l.size()) return (l.size() > rhs.l.size()) ^ sign;
    for (ll i = l.size() - 1; i >= 0; i--)
    {
        if (l[i] != rhs.l[i]) return (l[i] > rhs.l[i]) ^ sign;
    }
    for (ll i{}; i < r.size() && rhs.r.size(); i++)
    {
        if (r[i] != rhs.r[i]) return (r[i] > rhs.r[i]) ^ sign;
    }
    if (r.size() == rhs.r.size()) return false;
    return (r.size() > rhs.r.size()) ^ sign;
}

bool BigReal::operator<(const BigReal &rhs) const
{
    if (sign != rhs.sign) return !sign;
    if (rhs.l.size() != l.size()) return (l.size() < rhs.l.size()) ^ sign;
    for (ll i = l.size() - 1; i >= 0; i--)
    {
        if (l[i] != rhs.l[i]) return (l[i] < rhs.l[i]) ^ sign;
    }
    for (ll i{}; i < r.size() && rhs.r.size(); i++)
    {
        if (r[i] != rhs.r[i]) return (r[i] < rhs.r[i]) ^ sign;
    }
    if (r.size() == rhs.r.size()) return false;
    return (r.size() < rhs.r.size()) ^ sign;
}

bool BigReal::operator>=(const BigReal &rhs) const
{
    return (*this > rhs || *this == rhs);
}

bool BigReal::operator<=(const BigReal &rhs) const
{
    return (*this < rhs || *this == rhs);
}

BigReal BigReal::operator+=(BigReal rhs)
{
    int c{}, si{1};
    if (sign != rhs.sign)
    {
        sign = !sign;
        si *= -1;
        if (rhs > *this && !sign || *this > rhs && sign)  swap(*this, rhs);
        else sign = !sign;
    }

    while (rhs.l.size() > l.size()) l.push_back(0);
    while (rhs.r.size() > r.size()) r.push_front(0);
    while (rhs.l.size() < l.size()) rhs.l.push_back(0);
    while (rhs.r.size() < r.size()) rhs.r.push_front(0);

    for (int i{}; i < r.size() && i < rhs.r.size(); i++)
    {
        int tmp = r[i] + si * rhs.r[i] + c;
        if (tmp < 0) tmp += 10, c = -1;
        else c = tmp / 10, tmp %= 10;
        r[i] = tmp;
    }
    for (int i{}; i < l.size() && i < rhs.l.size(); i++)
    {
        int tmp = l[i] + si * rhs.l[i] + c;
        if (tmp < 0) tmp += 10, c = -1;
        else c = tmp / 10, tmp %= 10;
        l[i] = tmp;
    }
    if (c > 0) l.push_back(c);
    removeTrailingZeros();
    return *this;
}
BigReal BigReal::operator+(BigReal &rhs)
{
    BigReal tmp = *this;
    tmp += rhs;
    return tmp;
}
BigReal BigReal::operator-=(BigReal rhs)
{
    rhs.sign = !rhs.sign;
    *this += rhs;
    return *this;
}
BigReal BigReal::operator-(BigReal rhs)
{
    BigReal tmp = *this;
    rhs.sign = !rhs.sign;
    tmp += rhs;
    return tmp;
}

ostream& operator<<(ostream &os, const BigReal &r)
{
    os << r.getString();
    return os;
}

istream& operator>>(istream &is, BigReal &r)
{
    string s;
    is >> s;
    r = s;
    return is;
}
