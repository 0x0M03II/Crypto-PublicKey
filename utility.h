#ifndef UTILITY_H
#define UTILITY_H

#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <NTL/vector.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

using namespace std;
using namespace NTL;

struct AESResult {
    unsigned char* iv;
    unsigned char* ciphertext;
    int length;

    AESResult() {};
    AESResult(unsigned char* ivPtr, 
    unsigned char* ciphertextPtr) : iv(ivPtr), ciphertext(ciphertextPtr) {}
};

class PrimeUtility {

    public:

        static ZZ modularExponentiation(const ZZ& base, const ZZ& exponent, const ZZ& modulus);
        static ZZ modMultiply(const ZZ& a, const ZZ& b, const ZZ& n);
        static bool primalityTest(const ZZ& prime);
        static ZZ pkgcd(ZZ a, ZZ b);
        
        PrimeUtility() {}

    private:
        ZZ c, b, e;
};

#endif