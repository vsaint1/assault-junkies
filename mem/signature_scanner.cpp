#include "signature_scanner.h"

Signature::Signature(const char* pattern, unsigned int pBytesOffset) {
    this->pattern = pattern;
    this->addressByteOffset = pBytesOffset;

    for (unsigned int i = 0, charLength = 0; pattern[i] != '\0'; i++) {
        if (pattern[i] == ' ')
            continue;


        if (pattern[i] == '?') {
            addByte(0xFF, '?');
            continue;
        }

        if (++charLength == 2) {
            addByte(decode(pattern + i - 1), 'x');
            charLength = 0;
        }
    }
}

void Signature::addByte(unsigned int byte, char mask) {
    bytes[length] = byte;
    byteMask[length] = mask;
    length++;
}

unsigned int Signature::decode(const char* input) {
    unsigned int out, i, t, hn, ln;
    for (t = 0, i = 0; i < 2; i += 2, ++t) {
        hn = input[i] > '9' ? input[i] - 'A' + 10 : input[i] - '0';
        ln = input[i + 1] > '9' ? input[i + 1] - 'A' + 10 : input[i + 1] - '0';
        ((unsigned int*)&out)[t] = (hn << 4) | ln;
    }
    return out;
}

SignatureScanner::SignatureScanner(uintptr_t moduleBaseAddress, unsigned int moduleSize) {
    m_ModuleBaseAddress = moduleBaseAddress;
    m_ModuleSize = moduleSize;
}

bool SignatureScanner::scan(Signature* signature) {
    return scanMemory(m_ModuleBaseAddress, m_ModuleBaseAddress + m_ModuleSize, signature, 1);
}

bool SignatureScanner::scanAll(Signature signatures[], unsigned int signatureCount) {
    return scanMemory(m_ModuleBaseAddress, m_ModuleBaseAddress + m_ModuleSize, signatures, signatureCount);
}

bool SignatureScanner::scanMemory(uintptr_t fromAddress,uintptr_t toAddress, Signature signatures[], unsigned int signatureCount) {
    unsigned int searchRangeSize = toAddress - fromAddress;
    unsigned int signatureComplete = 0;

    for (unsigned int memoryIndex = 0; memoryIndex < searchRangeSize; memoryIndex++) {
        for (unsigned int signatureIndex = 0; signatureIndex < signatureCount; signatureIndex++) {
            if (signatures[signatureIndex].address != NULL) {
                continue;
            }

            if (checkSignature(fromAddress + memoryIndex, &signatures[signatureIndex])) {
                signatureComplete++;
            }
        }

        if (signatureComplete == signatureCount) {
            return true;
        }
    }
    return false;
}

bool SignatureScanner::checkSignature(uintptr_t address, Signature* signature) {
    for (unsigned int signatureIndex = 0; signatureIndex < signature->length; signatureIndex++) {
        if (signature->byteMask[signatureIndex] == '?') {
            continue;
        }
        if (signature->bytes[signatureIndex] != *(char*)(address + signatureIndex)) {
            return false;
        }
    }

    signature->address = address + signature->addressByteOffset;
    return true;
}
