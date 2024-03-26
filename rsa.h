#ifndef RSA_COSC583
#define RSA_COSC583
#include "utility.h"

namespace COSC583 { // openssl has an RSA library


    class RSA {

        public:
            
            ZZ getNValue(void) const;
            ZZ getEValue(void) const;
            ZZ getDValue(void) const;
            ZZ encryptRSA(ZZ message);
            ZZ getPhiNValue(void) const;
            void computeEEC(ZZ a, ZZ m);
            ZZ decryptRSA(ZZ cipherText);
            void relativePrimePrimers(void);
            void printAllValues(void) const;
            void calculateExtendedGCD(ZZ num1, ZZ num2, ZZ& coeff1, ZZ& coeff2, ZZ& greatestCommonDivisor);

            RSA() {};
            
            RSA(int keysize) {
                this->keysize = keysize;
            }

        private:
            void updateCoefficients(ZZ& prev, ZZ& current, ZZ quotient) {
                ZZ temp = prev - quotient * current;
                prev = current;
                current = temp;
            }
            size_t keysize;
            ZZ e = to_ZZ(65537);
            ZZ d, p, q, n, phi_n;
    }; // class RSA

}; // namespace COSC583

#endif