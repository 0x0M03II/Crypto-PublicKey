#include "rsa.h"
#include <iomanip>
#include "utility.h"
#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <NTL/ZZ_p.h>
#include <NTL/vector.h>
#include "diffiehellman.h"

using namespace std;
using namespace NTL;

void DiffieHellmanAssignment() {

    // Diffie-Hellman Key Aggreement
    std::cout << "Beginning Diffie-Hellman Algorithm" << std::endl;

    // Instantiate Objects
    PrimeUtility* p = new PrimeUtility();
    DiffieHellman* dh = new DiffieHellman();

    // Begin Algorithm Routines

    // Generate Safe Prime
    std::cout << "Generating Safe Prime" << std::endl;
    dh->generateSafePrime();

    // Compute Valide Generator of Order Q
    std::cout << "Computing Valid Generator" << std::endl;
    dh->getGenerator();

    // Begin Symmetric Key Encryption Simulation

    // Compute Local Private Key
    std::cout << "Computing Local User Private Key" << std::endl;
    ZZ private_key = RandomBits_ZZ(1024);

    // Compute Local PUblic Key
    std::cout << "Computing Local User Public Key" << std::endl;
    ZZ public_key = PrimeUtility::modularExponentiation(
        dh->getG(), private_key, 
        dh->getPrime());
    
    // Compute Remote Public and Private Key
    std::cout << "Simulating Compute Remote User Public/Private Key" << std::endl;
    ZZ otherUsersPublicKey = dh->simulateDiffieHellmanExchange(
        dh->getPrime(), dh->getG());

    // Compute Shared Secret Using Remote Public Key
    std::cout << "Computing SharedKey From Remote Public Key" << std::endl;
    ZZ sharedKey = PrimeUtility::modularExponentiation(
        dh->getG(), 
        otherUsersPublicKey, 
        dh->getPrime());

    // Use Key Derivation Function (KDF) SHA256[1:128]
    std::cout << "Transforming Shared Key (After SHA256)" << std::endl;
    unsigned char* hash256 = dh->transformSharedKey(sharedKey);

    // Simulate Remote User Encrypts Message
    std::cout << "Simulating Message Encryption" << std::endl;
    AESResult* res = dh->simulateSymmetricEncryptionAES(public_key);

    // Local User Decrypts Message
    std::cout << "Decrypting Encrypted Message" << std::endl;
    unsigned char* message = dh->decrypt(res->ciphertext, 23, hash256, res->iv);
    std::string plaintextString(reinterpret_cast<char*>(message), res->length);

    // Verify Decrypted Message
    std::cout << "Decrypted Message: " << std::endl;
    std::cout << plaintextString << std::endl;

}

void RSAAssignment() {

    // RSA Project Assignment
    std::cout << "Beginning RSA Algorithm\n" << endl;

    // Instantiating Objects
    COSC583::RSA* rsa = new COSC583::RSA(1024);

    // Call Algoirthm routines
    rsa->relativePrimePrimers();
    rsa->computeEEC(rsa->getEValue(), rsa->getPhiNValue());

    // Print Values
    rsa->printAllValues();

    // RSA Encryption

    // Generate Randome Value of size 1 < m < n
    ZZ message =  RandomBnd(rsa->getNValue() - 1);
    std::cout << "Message Before Encryption: " << std::endl;
    std::cout << message << "\n" << std::endl;

    // Encrypt
    ZZ postEncrypt = rsa->encryptRSA(message);
    std::cout << "Message After Encryption " << std::endl;
    std::cout << postEncrypt << "\n" << std::endl;

    // Decrypt
    std::cout << "Decrypting Message" << std::endl;
    ZZ postDecrypt = rsa->decryptRSA(postEncrypt);
    std::cout << "Message After Decryption " << std::endl;
    std::cout << postDecrypt << "\n" <<  std::endl;
}

int main()
{
    // Below 
    // Testing using Dr. Routi's example from the slides
    // ZZ_p e = to_ZZ_p(131);
    // ZZ_p b = ZZ_p(4235880211405804673);
    // ZZ_p n = ZZ_p(12855544647099734480);

    //DiffieHellmanAssignment();

    RSAAssignment();

    return 0;
}