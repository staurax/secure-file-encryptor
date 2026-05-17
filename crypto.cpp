#include "crypto.h"

#include <fstream>
#include <iostream>

using namespace std;

const string MAGIC_HEADER = "ENC_V2";
const int    KEY_STRETCH   = 1000;

uint32_t passwordChecksum(const string& password) {
    uint32_t hash = 0x12345678;

    for (char c : password) {
        hash ^= (unsigned char)c;
        hash  = (hash << 7) | (hash >> 25);
        hash += 0xDEADBEEF;
    }

    for (char c : password) {
        hash *= 0x9E3779B9;
        hash ^= (unsigned char)c;
    }

    return hash;
}

vector<unsigned char> stretchKey(const string& password, size_t length) {
    vector<unsigned char> key(length);

    for (size_t i = 0; i < length; i++)
        key[i] = (unsigned char)password[i % password.size()];

    for (int round = 0; round < KEY_STRETCH; round++) {
        unsigned char carry = (unsigned char)(round & 0xFF);

        for (size_t i = 0; i < length; i++) {
            key[i] ^= carry;
            key[i]  = (key[i] << 3) | (key[i] >> 5);
            carry   = key[i];
        }
    }

    return key;
}

vector<unsigned char> xorCrypt(const vector<unsigned char>& data,
                                const string& password) {
    vector<unsigned char> key    = stretchKey(password, data.size());
    vector<unsigned char> result(data.size());

    for (size_t i = 0; i < data.size(); i++)
        result[i] = data[i] ^ key[i];

    return result;
}

bool encryptFile(const string& inputPath,
                 const string& outputPath,
                 const string& password) {
    ifstream inFile(inputPath, ios::binary);

    if (!inFile.is_open()) {
        cerr << "  [ERROR] Cannot open input file: " << inputPath << endl;
        return false;
    }

    vector<unsigned char> plainData(
        (istreambuf_iterator<char>(inFile)),
         istreambuf_iterator<char>()
    );

    inFile.close();

    if (plainData.empty()) {
        cerr << "  [ERROR] File is empty." << endl;
        return false;
    }

    vector<unsigned char> cipherData = xorCrypt(plainData, password);

    ofstream outFile(outputPath, ios::binary);

    if (!outFile.is_open()) {
        cerr << "  [ERROR] Cannot create output file: " << outputPath << endl;
        return false;
    }

    outFile.write(MAGIC_HEADER.c_str(), MAGIC_HEADER.size());

    uint32_t checksum = passwordChecksum(password);

    outFile.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));

    uint64_t fileSize = (uint64_t)plainData.size();

    outFile.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));

    outFile.write(reinterpret_cast<const char*>(cipherData.data()), cipherData.size());

    outFile.close();

    cout << "  [OK] Encrypted : " << inputPath << "  -->  " << outputPath << endl;
    cout << "  [OK] File size : " << plainData.size() << " bytes" << endl;

    return true;
}

bool decryptFile(const string& inputPath,
                 const string& outputPath,
                 const string& password) {
    ifstream inFile(inputPath, ios::binary);

    if (!inFile.is_open()) {
        cerr << "  [ERROR] Cannot open file: " << inputPath << endl;
        return false;
    }

    char headerBuf[7] = {0};

    inFile.read(headerBuf, MAGIC_HEADER.size());

    if (string(headerBuf) != MAGIC_HEADER) {
        cerr << "  [ERROR] Not a valid encrypted file (wrong format or old V1 file)." << endl;
        inFile.close();
        return false;
    }

    uint32_t savedChecksum = 0;

    inFile.read(reinterpret_cast<char*>(&savedChecksum), sizeof(savedChecksum));

    uint32_t inputChecksum = passwordChecksum(password);

    if (inputChecksum != savedChecksum) {
        cerr << "  [ERROR] Wrong password! Decryption aborted." << endl;
        inFile.close();
        return false;
    }

    uint64_t originalSize = 0;

    inFile.read(reinterpret_cast<char*>(&originalSize), sizeof(originalSize));

    vector<unsigned char> cipherData(
        (istreambuf_iterator<char>(inFile)),
         istreambuf_iterator<char>()
    );

    inFile.close();

    vector<unsigned char> plainData = xorCrypt(cipherData, password);

    ofstream outFile(outputPath, ios::binary);

    if (!outFile.is_open()) {
        cerr << "  [ERROR] Cannot create output file: " << outputPath << endl;
        return false;
    }

    outFile.write(reinterpret_cast<const char*>(plainData.data()), originalSize);

    outFile.close();

    cout << "  [OK] Decrypted : " << inputPath << "  -->  " << outputPath << endl;
    cout << "  [OK] Restored  : " << originalSize << " bytes" << endl;

    return true;
}
