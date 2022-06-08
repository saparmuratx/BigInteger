#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class BigInteger {

    std::vector<int> value;
    bool sign;

    BigInteger(std::vector<int> val, bool sg) {
        value = val;
        sign = sg;
    }

    template <typename C> static int sz(const C &c) {
        return static_cast<int>(c.size());
    }

    static int cmp(const std::vector<int> &a, const std::vector<int> &b) {
        for (int i = 0; i < sz(a); i++) {
            if (a[i] > b[i]) return 1;
            if (a[i] < b[i]) return -1;
        }
        return 0;
    }

    friend std::ostream &operator<<(std::ostream &out, const BigInteger &r);
    friend std::istream &operator>>(std::istream &in, const BigInteger &r);

    friend BigInteger operator+(const BigInteger &a, const BigInteger &b);
    friend BigInteger operator-(const BigInteger &a, const BigInteger &b);
    friend BigInteger operator*(const BigInteger &a, const BigInteger &b);
    friend BigInteger operator%(const BigInteger &a, const BigInteger &b);
    friend BigInteger operator/(const BigInteger &a, const BigInteger &b);

    friend bool operator>(const BigInteger &a, const BigInteger &b);
    friend bool operator<(const BigInteger &a, const BigInteger &b);
    friend bool operator>=(const BigInteger &a, const BigInteger &b);
    friend bool operator<=(const BigInteger &a, const BigInteger &b);
    friend bool operator==(const BigInteger &a, const BigInteger &b);
    friend bool operator!=(const BigInteger &a, const BigInteger &b);

  public:
    BigInteger() : value(1), sign(false) { value[0] = 0; }

    BigInteger(const std::string &str) : sign(false) {
        if (str.empty())
            throw std::runtime_error("Empty std::string can not be BigInteger");

        if (sz(str) == 1 && !isdigit(str[0]))
            throw std::runtime_error("Invalid value for BigInteger: " + str);

        int bgn = 0;

        if (str.front() == '-' || str.front() == '+') {
            sign = str.front() == '-';
            bgn++;
        }

        while (str[bgn] == '0' && bgn < sz(str) - 1)
            bgn++;

        if (str == "0") sign = false;

        if (sz(str)) {
            for (int i = bgn; i < sz(str); i++) {
                value.push_back(str[i] - '0');
                if (value.back() > 9 || value.back() < 0)
                    throw std::runtime_error("Invalid value: " + str);
            }
        }
    }

    BigInteger(int64_t num) : BigInteger(std::to_string(num)) {}

    BigInteger operator-() const; // Unary -
    BigInteger operator+() const; // Unary +
    BigInteger &operator++();     // prefix increment
    BigInteger &operator--();     // prefix increment
    BigInteger operator++(int);   // postfix increment
    BigInteger operator--(int);   // postfix increment
    void operator+=(const BigInteger &b);
    void operator-=(const BigInteger &b);
    void operator*=(const BigInteger &b);
    void operator%=(const BigInteger &b);
    void operator/=(const BigInteger &b);

    static BigInteger abs(const BigInteger &a) {
        BigInteger temp = a;
        temp.sign = false;
        return temp;
    }

    static BigInteger add(const BigInteger &a, const BigInteger &b) {
        if (a.sign != b.sign) {
            if (a.sign) return sub(b, BigInteger(a.value, false));
            if (b.sign) return sub(a, BigInteger(b.value, false));
        }

        BigInteger res;
        res.value.clear();

        const std::vector<int> &x =
            sz(a.value) >= sz(b.value) ? a.value : b.value;
        const std::vector<int> &y =
            sz(a.value) < sz(b.value) ? a.value : b.value;

        int temp = 0, j = sz(y) - 1;

        for (int i = sz(x) - 1; i >= 0; i--, j--) {
            temp += (i >= 0 ? x[i] : 0) + (j >= 0 ? y[j] : 0);
            res.value.push_back(temp % 10);
            temp /= 10;
        }

        if (temp) res.value.push_back(temp);

        if (a.sign && b.sign) res.sign = true;

        reverse(res.value.begin(), res.value.end());

        return res;
    }

    static BigInteger sub(const BigInteger &a, const BigInteger &b) {
        if (a.sign != b.sign) {
            if (a.sign) return add(a, BigInteger(b.value, true));
            if (b.sign) return add(a, BigInteger(b.value, false));
        }

        BigInteger res;
        res.value.clear();

        if (a == b) return BigInteger("0");

        const std::vector<int> &x = (abs(a) > abs(b) ? a.value : b.value);
        const std::vector<int> &y = (abs(a) < abs(b) ? a.value : b.value);

        int temp = 0, j = sz(y) - 1, carry = 0;

        for (int i = sz(x) - 1; i >= 0; i--, j--) {
            int subed = (i >= 0 ? x[i] : 0);
            int suber = (j >= 0 ? y[j] : 0);
            if (subed == suber && carry == 0) {
                res.value.push_back(0);
                continue;
            }
            temp = subed + carry;
            subed = subed == 0 ? 10 : subed;
            temp -= suber;
            res.value.push_back(temp < 0 ? 10 + temp : temp);
            carry = (temp < 0 || subed == 10) ? -1 : 0;
            temp = 0;
        }

        while (res.value.back() == 0)
            res.value.pop_back();

        if (a < b) res.sign = true;

        reverse(res.value.begin(), res.value.end());

        return res;
    }

    static BigInteger mul(const BigInteger &a, const BigInteger &b) {
        BigInteger res;
        res.value.resize(sz(a.value) + sz(b.value));

        const std::vector<int> &x = (abs(a) > abs(b) ? a.value : b.value);
        const std::vector<int> &y = (abs(a) < abs(b) ? a.value : b.value);

        if (a == BigInteger() || b == BigInteger())
            return BigInteger(); // multiplication by zero

        int shift = 0, end = sz(res.value) - 1;
        for (int i = sz(y) - 1; i >= 0; i--, shift++) {
            int index = shift;
            for (int j = sz(x) - 1; j >= 0; j--, index++) {
                int multic = y[i] * x[j];
                res.value[end - index] += multic;
            }
        }

        for (int i = sz(res.value) - 1; i > 0; i--) {
            int carry = res.value[i] / 10;
            res.value[i] %= 10;
            res.value[i - 1] += carry;
        }

        if (res.value.front() == 0) res.value.erase(res.value.begin());

        if (a.sign != b.sign) res.sign = true;

        return res;
    }

    static BigInteger div(const BigInteger &a, const BigInteger &dv) {
        if (dv == BigInteger())
            throw std::runtime_error("BigInteger: can't divide by 0!");
        if (abs(a) < abs(dv) || a == 0) return 0;

        BigInteger res;

        if (abs(a) == abs(dv)) {
            res = 1;

            res.sign = a.sign != dv.sign;
            return res;
        }

        if (abs(dv) == 1) {
            res = a;
            res.sign = a.sign != dv.sign;
            return res;
        }

        res.value.clear();

        BigInteger x;
        int index = 0;

        x.value.clear();

        while (x < dv) {
            x.value.push_back(a.value[index++]);
        }
        int cnt = 0;
        while (x >= dv) {
            x -= dv;
            cnt++;
        }

        res.value.push_back(cnt);

        while (index < sz(a.value)) {
            if (x < dv) {
                if (x == BigInteger()) x.value.clear();
                x.value.push_back(a.value[index++]);
                while (x < dv && index < sz(a.value)) {
                    if (x == BigInteger()) x.value.clear();
                    x.value.push_back(a.value[index++]);
                    res.value.push_back(0);
                }
            }

            cnt = 0;

            while (x >= dv) {
                x -= dv;
                cnt++;
            }

            res.value.push_back(cnt);
        }

        res.sign = a.sign != dv.sign;

        return res;
    }

    static BigInteger mod(const BigInteger &a, const BigInteger &b) {
        if (b == BigInteger())
            throw std::runtime_error("BigInteger: can't divide by 0!");
        if (a == BigInteger()) {
            return BigInteger();
        }

        BigInteger res = abs(a);
        BigInteger y = abs(b);

        if (res < y) {
            res.sign = a.sign;
            return res;
        }

        if (y == BigInteger("1")) {
            return BigInteger();
        }

        do {
            y = abs(b);

            while (sz(res.value) > sz(y.value)) {
                y.value.push_back(0);
            }

            if (res < y) y.value.pop_back();

            while (res >= y) {
                res -= y;
            }
        } while (res >= abs(b));

        if (res != BigInteger()) res.sign = a.sign;

        return res;
    }

    void add(const BigInteger &b) { *this = add(*this, b); }
    void sub(const BigInteger &b) { *this = sub(*this, b); }
    void mul(const BigInteger &b) { *this = mul(*this, b); }
    void mod(const BigInteger &b) { *this = mod(*this, b); }
    void div(const BigInteger &b) { *this = div(*this, b); }

    static int compare(const BigInteger &a, const BigInteger &b) {
        if (a.isSigned() && !b.isSigned()) return -1;
        if (!a.isSigned() && b.isSigned()) return 1;

        if (!a.isSigned() && !b.isSigned()) {
            if (sz(a.value) > sz(b.value)) return 1;
            if (sz(a.value) < sz(b.value)) return -1;
            return cmp(a.value, b.value);
        }

        if (sz(a.value) < sz(b.value)) return 1;
        if (sz(a.value) > sz(b.value)) return -1;

        int res = cmp(a.value, b.value);
        return -res;
    }

    const std::vector<int> &getValue() const { return value; }
    const bool &isSigned() const { return sign; }
};

std::istream &operator>>(std::istream &in, BigInteger &r) {
    char ch;
    if (!in.get(ch)) return in;

    while (isspace(ch))
        in.get(ch);

    if (!isdigit(ch) && ch != '-' && ch != '+') {
        in.setstate(std::ios_base::failbit);
        return in;
    }

    std::string str;

    str += ch;

    while (in.get(ch)) {
        if (!isdigit(ch)) {
            if (!isspace(ch)) in.putback(ch);
            break;
        }
        str += ch;
    }

    if (str.size() == 1 && !isdigit(str[0])) {
        in.setstate(std::ios_base::failbit);
        return in;
    }

    r = BigInteger(str);

    return in;
}

std::ostream &operator<<(std::ostream &out, const BigInteger &r) {

    std::vector<int> temp = r.getValue();

    if (r.isSigned()) out << "-";

    for (auto &&i : temp) {
        out << i;
    }

    return out;
}

BigInteger operator+(const BigInteger &a, const BigInteger &b) {
    return BigInteger::add(a, b);
}

BigInteger operator-(const BigInteger &a, const BigInteger &b) {
    return BigInteger::sub(a, b);
}

BigInteger operator*(const BigInteger &a, const BigInteger &b) {
    return BigInteger::mul(a, b);
}

BigInteger operator%(const BigInteger &a, const BigInteger &b) {
    return BigInteger::mod(a, b);
}

BigInteger operator/(const BigInteger &a, const BigInteger &b) {
    return BigInteger::div(a, b);
}

BigInteger BigInteger::operator-() const {
    BigInteger res;
    res.value = value;
    res.sign = !sign;

    if (res.value[0] == 0) res.sign = false;

    return res;
}

BigInteger BigInteger::operator+() const { return *this; }

void BigInteger::operator+=(const BigInteger &b) { this->add(b); }
void BigInteger::operator-=(const BigInteger &b) { this->sub(b); }
void BigInteger::operator*=(const BigInteger &b) { this->mul(b); }
void BigInteger::operator%=(const BigInteger &b) { this->mod(b); }
void BigInteger::operator/=(const BigInteger &b) { this->div(b); }

BigInteger &BigInteger::operator++() {
    this->add(BigInteger("1"));
    return *this;
}

BigInteger &BigInteger::operator--() {
    this->sub(BigInteger("1"));
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger temp = *this;
    ++*this;
    return temp;
}

BigInteger BigInteger::operator--(int) {
    BigInteger temp = *this;
    --*this;
    return temp;
}

bool operator>(const BigInteger &a, const BigInteger &b) {
    int res = BigInteger::compare(a, b);
    return (res == 1);
}

bool operator<(const BigInteger &a, const BigInteger &b) {
    int res = BigInteger::compare(a, b);
    return (res == -1);
}

bool operator>=(const BigInteger &a, const BigInteger &b) {
    int res = BigInteger::compare(a, b);
    return (res != -1);
}

bool operator<=(const BigInteger &a, const BigInteger &b) {
    int res = BigInteger::compare(a, b);
    return (res != 1);
}

bool operator==(const BigInteger &a, const BigInteger &b) {
    int res = BigInteger::compare(a, b);
    return (res == 0);
}

bool operator!=(const BigInteger &a, const BigInteger &b) {
    int res = BigInteger::compare(a, b);
    return (res != 0);
}
