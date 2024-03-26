#include "utility.h"
#include <NTL/lzz_pX.h>
#include <openssl/aes.h>

ZZ PrimeUtility::modularExponentiation(const ZZ& base, const ZZ& exponent, const ZZ& modulus)
{
    /* Using the Algorithm provided in class, this routine keeps a running product
       when performing the modular exponentiation.  Dr. Routi's algorithm was used
       here to perform modular exponentiation.
    
    *  Square and multiply.  Understanding why this works, say you have an exponent
       13 and a base b.  The number 13 represented as binary is 1101, so the following
       are equivelant: b^13 and b^(1101).  Changing the exponent to binary makes it 
       more clear what we are doing.  b^(1101) is the same as b^8 * b^4 * b^1.
    
    *  Algorithm:  Each iteration from i = 0 to l, the product is squared (mod m) to
       ensure that it represents the current bit value.  This means that when the e_i
       bit is set, and product is multiplied by base, the value represent that current
       bit.  In binary, each bit in a binary representation of an exponent is a power
       of 2.

    *  input: ZZ base, ZZ exponent, ZZ modulus

    *  return: ZZ exponentiation result */

    ZZ b = base;
    ZZ product = to_ZZ(1);
    long length = NumBits(exponent);

    for (long i = NumBits(exponent) - 1; i >= 0; i--) {

        // Update product to account for every bit shift/iteration
        product = (product * product) % modulus;
        if (bit(exponent, i) == 1) // set so multiply b
            product = modMultiply(product, b, modulus);
    }

    if (exponent < 0)
      return InvMod(product, modulus);

    return product;
}


ZZ PrimeUtility::modMultiply(const ZZ& a, const ZZ& b, const ZZ& n) {

    /* Simple Modular multiplicaiton has the commutative property, which
       means we can take each Multiplicand (mod n) and multiply that and 
       with the multiplier (mod n), making the answer the product (mod n).
    
    *  input: ZZ multiplicand, ZZ multiplier, ZZ modulus

    *  return: ZZ product (mod n) */

    ZZ result = (a % n);
    result *= (b % n);
    result %= n;

    // account for negative result
    if (result < 0) {
        result += n;
    }

    return result;
}

bool PrimeUtility::primalityTest(const ZZ& prime) {
    /* Miller-Rabin primality test following the algorithm discussed in
       the class lecture.  The NTL library generates a random number
       using RandomBnd(p) in the range [0, p-1].  Zero is an invalid num
       ber, so +1 is added to the result to ensure that it's valid.
    
    *  input: ZZ prime number

    *  return: boolean */


    int s = 0;
    int k = 50; // k = 50 for more confidence
    ZZ d = prime - 1;

    // factor out two from p - 1
    while (d % 2 == 0) {
        div(d, d, 2);
        s++;
    }

    while (k > 0) {

        ZZ check_a = RandomBnd(prime - 1) + 1; // no zero

        // Algorithm defines x = q^d (mod p)
        ZZ x = modularExponentiation(check_a, d, prime);
        if (x == 1 || x == prime - 1) {
            k--;
            continue;
        }

        bool composite = false;
        for (int i = 1; i < s; ++i) { // s - 1 times
            x = modularExponentiation(x, to_ZZ(2), prime);

            // x is already reduces modulus modularExponentiation()
            if (x == prime - 1) {
                composite = true;
                break;
            }
        }
        if (!composite) {
            return false;
        }

        --k;
    }

    return true;
}
