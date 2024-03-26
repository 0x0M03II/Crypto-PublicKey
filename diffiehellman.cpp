#include "utility.h"
#include "diffiehellman.h"
#include <NTL/ZZ.h>

// BEGIN Diffie-Hellman  Algorithm

// Accessor Methods
ZZ DiffieHellman::getQ() const {
    return this->q;
}

ZZ DiffieHellman::getG() const {
    return this->g;
}

ZZ DiffieHellman::getPrime() const {
    return this->p;
}

// BEGIN: utility function zzToBytes()
unsigned char* zzToBytes(const NTL::ZZ& num) {
    unsigned char* bytes = new unsigned char[NTL::NumBytes(num)];
    
    NTL::BytesFromZZ(bytes, num, NTL::NumBytes(num));
    return bytes;
}

// BEGIN: Diffie-Hellman Algorithm Routines
void DiffieHellman::generateSafePrime(void) {

    ZZ p, q;
    while (true) {

        // find safe prime
        q = RandomBits_ZZ(1024);

        // is q prime?
        if (!PrimeUtility::primalityTest(q)) {
            continue;
        }

        mul(q, q, to_ZZ(2));
        add(p, q, to_ZZ(1));

        if (PrimeUtility::primalityTest(p)) {
            std::cout << "Found Safe Prime P " << p << std::endl;
            this->q = q;
            this->p = p;
            break;
        }
    } // found safe prime
}

void DiffieHellman::getGenerator() {
    /* Generator is a member of a subgroup that produces a set of integers n
       when raised to the power of i (mod n) from i = 1 to m, terminating on 
       the identity element, produces all of the members of the subgroup. For
       Diffie-Hellman, the generator must be of order q.

    *  Algorithm: a random h is selected from the range [2, p-2] and squared.
       After being squared, the following must be true for a successful generator
       to be identified: h^2 != 1 and g^q = 1.
    
    *  Input: void

    *  return: void */



    // choose h b/t [2, p-2]
    ZZ h, g;
    ZZ q = this->q;

    while (true) {
        h = RandomBnd(this->p) + 2;
        g = PrimeUtility::modularExponentiation(h, to_ZZ(2), p);

        // cannot equal 1
        if (g == to_ZZ(1)) {continue;}

        if ((PrimeUtility::modularExponentiation(g, q, p) == 1)) {
            this->g = g; // valid generator found
            break;
        }

    }
    this->g = g; // generator
}

// BEGIN: Transform Shared Key
unsigned char* DiffieHellman::transformSharedKey(const ZZ& sharedKey) {
    unsigned char* hash = new unsigned char[16];

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    unsigned char* data = zzToBytes(sharedKey);
    SHA256_Update(&sha256, data, NTL::NumBytes(sharedKey));
    SHA256_Final(hash, &sha256);

    return hash;
}

// BEGIN: Simulation with Remote users

// Generate remote public and private key
ZZ DiffieHellman::simulateDiffieHellmanExchange(const ZZ& p, const ZZ& g) {

    // p and g are passed, simulating agreement.
    ZZ private_key = RandomBits_ZZ(1024);
    ZZ public_key = PrimeUtility::modularExponentiation(g, private_key,p);

    return public_key;
}

// Encrypt remote message and send IV, Ciphertext
AESResult* DiffieHellman::simulateSymmetricEncryptionAES(const ZZ& pub) {
    // pub == local users public key

    // Remote User generating shared key
    ZZ sharedKey = PrimeUtility::modularExponentiation(
        this->g, 
        pub, 
        this->p);

    std::string testMessage = "This was a fun project!";

    // Key Derivation Function SHA256 first 128
    unsigned char* encryptedKey = transformSharedKey(sharedKey);

    // Encrypt message
    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);

    // Initialize OpenSSL context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, encryptedKey, iv);

    int outLen1 = 0, outLen2 = 0;

    int ciphertextLen = testMessage.length() + AES_BLOCK_SIZE;

    //unsigned char* ciphertext = encrypt(testMessage, encryptedKey, iv);
    unsigned char* ciphertext = new unsigned char[ciphertextLen];

    EVP_EncryptUpdate(ctx, ciphertext, &outLen1, (const unsigned char*)testMessage.c_str(), testMessage.length());
    EVP_EncryptFinal_ex(ctx, ciphertext + outLen1, &outLen2);

    EVP_CIPHER_CTX_free(ctx);

    ciphertextLen = outLen1 + outLen2;

    AESResult* result = new AESResult;
    result->iv = iv;
    result->ciphertext = ciphertext;
    result->length = ciphertextLen;

    return result;
}

// Decrypt Ciphertext
unsigned char* DiffieHellman::decrypt(const unsigned char* ciphertext, size_t ciphertextSize, const unsigned char* key, unsigned char* iv) {
    /* OpenSSL is a pretty complicated library.  I wasn't 100% sure how the simulation
       was supposed to work, so I created a basic encrypt and decrypt method to facilitate
       the exercise.
    
    * input: unsinged char* ciphertext, size_t ciphertext size, const unsigned char* key, unsigned char* iv

    * return: plaintext bytes */


    // Start an OpenSSL Context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    // Use AES-CBC-128 with Padding
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_CIPHER_CTX_set_padding(ctx, 0);
    
    int outLen1 = 0, outLen2 = 0;
    int plaintextLen = ciphertextSize;

    unsigned char* plaintext = new unsigned char[plaintextLen];

    // Some methods that I tried were deprecated, so I had to use this 
    EVP_DecryptUpdate(ctx, plaintext, &outLen1, ciphertext, ciphertextSize);
    EVP_DecryptFinal_ex(ctx, plaintext + outLen1, &outLen2);

    EVP_CIPHER_CTX_free(ctx);

    // Adjust plaintext length
    plaintextLen = outLen1 + outLen2;

    return plaintext;
}