#include "rsa.h"

// RSA Algorithm Implemenation

// BEGIN: RSA Print Statements
void COSC583::RSA::printAllValues() const {
    std::cout << "E Value: " << endl;
    std::cout << e << "\n" << std::endl;
    std::cout << "Prime P: " << endl;
    std::cout << p << "\n" << std::endl;
    std::cout << "Prime Q: " << endl;
    std::cout << q << "\n" << std::endl;
    std::cout << "Prime N: " << endl;
    std::cout << n << "\n" << std::endl;
    std::cout << "Prime Phi(n): " << endl;
    std::cout << phi_n << "\n" << std::endl;
    std::cout << "D Value: " << endl;
    std::cout << d << "\n" << std::endl;
}

// BEGIN: RSA Accesor Methods
ZZ COSC583::RSA::getNValue(void) const {
    return this->n;
}

ZZ COSC583::RSA::getDValue(void) const {
    return this->d;
}

ZZ COSC583::RSA::getPhiNValue(void) const {
    return this->phi_n;
}

ZZ COSC583::RSA::getEValue(void) const {
    return this->e;
}


// BEGIN: RSA Encryption & Decryption
ZZ COSC583::RSA::encryptRSA(ZZ message) {
    return PrimeUtility::modularExponentiation(message, this->e, this->n);
}

ZZ COSC583::RSA::decryptRSA(ZZ cipherText) {
    return PrimeUtility::modularExponentiation(cipherText, this->d, this->n);
}


// BEGIN: RSA Algorithm Routines 

// Stare the EEC Algorithm
void COSC583::RSA::computeEEC(ZZ a, ZZ m) {

    ZZ x, y, gcd;
    calculateExtendedGCD(a, m, x, y, gcd);
    
    // account for negavtive values
    this->d = x % m;
    if (this->d < to_ZZ(0)) {
        this->d += m;
    }
}


// Compute Relative Prime e and phi_n
void COSC583::RSA::relativePrimePrimers() {
    /* This implementation of RSA uses 1024-bit prime values for p and q,
       which are then used to compute n and phi(n), and phi(n) be coprime
       with e, which is 56537 
    
    *  Without using a prime number generator (which is provide by the library
       that I used for this project, NTL), random 1024-bit ZZ numbers are 
       denerated and prime tested using my static prime utility method 
       PrimeUtility::primalityTest()
    
    *  Note: I did use the built-in method GCD() since the directions did not
       specify GCD as one of the routines that cannot be implemented using a 
       library.  This built in is use nowhere else */
    

    ZZ p, q, n, phi_n;

    // keep trying until phi(n) and e are coprime
    while (true) {

        // Generate random 1024-bit numbers for p and q
        do {
            p = RandomBits_ZZ(keysize);
        } while(!PrimeUtility::primalityTest(p));

        do {
            q = RandomBits_ZZ(keysize);
        } while (!PrimeUtility::primalityTest(q));

        // Calculate n and phi(n)
        n = p * q;
        phi_n = (p - 1) * (q - 1);

        // Check if relatively prime
        if (GCD(phi_n, e) == 1) {
            break;
        }
    }

    this->p = p; this->q = q; this->n = n; this->phi_n = phi_n;

}

// Extended Euclidean Algorithm
void COSC583::RSA::calculateExtendedGCD(ZZ num1, ZZ num2, ZZ& coeff1, ZZ& coeff2, ZZ& greatestCommonDivisor) {
    /* As described in the lecture the algorithm works repeatedly swaps a with b, b with a%b, and gets the 
       new remainder until we reach a remainder of 0.  At that point, we must
       work backwards.

       ax + by = GCD(a, b) is the formula
    
    *  Substitution after Base case is reached, work backwards to substitute,
       rearrange, and ditribute to get the ax + by = GCD(a,b) structure until
       there are no moves left.  Then the d value.

    *  This shows the structure of the problem at every iteration i and how
       working backwards after reaching 0 will allow me to substitute. As discussed
       in COSC581 
    
    */

    ZZ rPrev = num1, rCurrent = num2;
    ZZ sPrev = to_ZZ(1), sCurrent = to_ZZ(0);
    ZZ tPrev = to_ZZ(0), tCurrent = to_ZZ(1);
    ZZ quotient;

    do {
        quotient = rPrev / rCurrent;
        updateCoefficients(rPrev, rCurrent, quotient);
        updateCoefficients(sPrev, sCurrent, quotient);
        updateCoefficients(tPrev, tCurrent, quotient);
    } while (rCurrent != 0);

    greatestCommonDivisor = rPrev; coeff1 = sPrev; coeff2 = tPrev;
}