#ifndef DH_COSC583
#define DH_COSC583

#include "utility.h"
#include <NTL/ZZ.h>

class DiffieHellman {

    public:
        ZZ getQ() const;
        ZZ getG() const;
        ZZ getPrime() const;
        void getGenerator();
        void generateSafePrime(void);
        AESResult* simulateSymmetricEncryptionAES(const ZZ& pub);
        unsigned char* transformSharedKey(const ZZ& sharedKey);
        ZZ simulateDiffieHellmanExchange(const ZZ& p, const ZZ& g);
        unsigned char* encrypt(const std::string& plaintext, const unsigned char* key, unsigned char* iv);
        unsigned char* decrypt(const unsigned char* ciphertext, size_t ciphertextSize, const unsigned char* key, unsigned char* iv);

        /* constructor */
        DiffieHellman() {};

    private:
        ZZ p, q, g;
}; // DiffieHellman

#endif