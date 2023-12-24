#pragma once
#include <iostream>


class Signature {
public:
    const char* pattern;
    unsigned int addressByteOffset;
    uintptr_t address = 0;

    unsigned int length = 0;
    char bytes[255] = {};
    char byteMask[255] = {};

    explicit Signature(const char* pattern) : Signature(pattern, 0) {
    };

    Signature(const char* pattern, unsigned int pBytesOffset);

private:
    void addByte(unsigned int byte, char mask);

    unsigned int decode(const char* input);
};

class SignatureScanner {
public:
    SignatureScanner(uintptr_t moduleBaseAddress, unsigned int moduleSize);


    bool scan(Signature* signature);

    bool scanAll(Signature signatures[], unsigned int signatureCount);

private:
    bool scanMemory(uintptr_t from, uintptr_t to, Signature signature[], unsigned int signatureCount);

    bool checkSignature(uintptr_t address, Signature* signature);

    uintptr_t m_ModuleBaseAddress;
    unsigned int m_ModuleSize;
};
