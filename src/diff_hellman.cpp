#include"diff_hellman.h"


DifHel::DifHel()
{

}
DifHel::~DifHel()
{

}

// 生成指定位数的随机数
ull DifHel::generateRandomNumber(int digits) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<ull> dis(pow(10, digits - 1), pow(10, digits) - 1);
    return dis(gen);
}

// 快速幂取模
ull DifHel::powerMod(ull base, ull exp, ull mod) {
    ull result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// 米勒-罗宾素性检测
bool DifHel::millerRabinTest(ull d, ull n) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<ull> dis(2, n - 2);
    ull a = dis(gen);
    ull x = powerMod(a, d, n);
    if (x == 1 || x == n - 1)
        return true;
    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;
        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

bool DifHel::isPrime(ull n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;
    ull d = n - 1;
    while (d % 2 == 0)
        d /= 2;
    for (int i = 0; i < k; i++)
        if (!millerRabinTest(d, n))
            return false;
    return true;
}

// 生成指定位数的素数
ull DifHel::generatePrime(int digits) {
    ull num;
    do {
        num = generateRandomNumber(digits);
    } while (!isPrime(num, 4));
    return num;
}

// 判断 g 是否是 q 的原根
bool DifHel::isPrimitiveRoot(ull g, ull q) {
    if (powerMod(g, q - 1, q) != 1)
        return false;
    for (ull i = 2; i * i <= q - 1; i++) {
        if ((q - 1) % i == 0) {
            if (powerMod(g, i, q) == 1 || powerMod(g, (q - 1) / i, q) == 1)
                return false;
        }
    }
    return true;
}

// 求 q 的一个原根
ull DifHel::findPrimitiveRoot(ull q) {
    for (ull g = 2; g < q; g++) {
        if (isPrimitiveRoot(g, q))
            return g;
    }
    return -1; // 没有找到原根
}