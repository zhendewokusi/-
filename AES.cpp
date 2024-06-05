#include "AES.h"

AES::AES(const AESKeyLength keyLength) {
  switch (keyLength) {
    case AESKeyLength::AES_128:
      this->Nk = 4;
      this->Nr = 10;
      break;
    case AESKeyLength::AES_192:
      this->Nk = 6;
      this->Nr = 12;
      break;
    case AESKeyLength::AES_256:
      this->Nk = 8;
      this->Nr = 14;
      break;
  }
}

unsigned char *AES::EncryptECB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[]) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    EncryptBlock(in + i, out + i, roundKeys);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::DecryptECB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[]) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    DecryptBlock(in + i, out + i, roundKeys);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::EncryptCBC(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char block[blockBytesLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    XorBlocks(block, in + i, block, blockBytesLen);
    EncryptBlock(block, out + i, roundKeys);
    memcpy(block, out + i, blockBytesLen);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::DecryptCBC(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char block[blockBytesLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    DecryptBlock(in + i, out + i, roundKeys);
    XorBlocks(block, out + i, out + i, blockBytesLen);
    memcpy(block, in + i, blockBytesLen);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::EncryptCFB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char block[blockBytesLen];
  unsigned char encryptedBlock[blockBytesLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    EncryptBlock(block, encryptedBlock, roundKeys);
    XorBlocks(in + i, encryptedBlock, out + i, blockBytesLen);
    memcpy(block, out + i, blockBytesLen);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::DecryptCFB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char block[blockBytesLen];
  unsigned char encryptedBlock[blockBytesLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    EncryptBlock(block, encryptedBlock, roundKeys);
    XorBlocks(in + i, encryptedBlock, out + i, blockBytesLen);
    memcpy(block, in + i, blockBytesLen);
  }

  delete[] roundKeys;

  return out;
}

void AES::CheckLength(unsigned int len) {
  if (len % blockBytesLen != 0) {
    throw std::length_error("Plaintext length must be divisible by " +
                            std::to_string(blockBytesLen));
  }
}

void AES::EncryptBlock(const unsigned char in[], unsigned char out[],
                       unsigned char *roundKeys) {
  unsigned char state[4][Nb];
  unsigned int i, j, round;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      state[i][j] = in[i + 4 * j];
    }
  }

  AddRoundKey(state, roundKeys);

  for (round = 1; round <= Nr - 1; round++) {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(state, roundKeys + round * 4 * Nb);
  }

  SubBytes(state);
  ShiftRows(state);
  AddRoundKey(state, roundKeys + Nr * 4 * Nb);

  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      out[i + 4 * j] = state[i][j];
    }
  }
}

void AES::DecryptBlock(const unsigned char in[], unsigned char out[],
                       unsigned char *roundKeys) {
  unsigned char state[4][Nb];
  unsigned int i, j, round;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      state[i][j] = in[i + 4 * j];
    }
  }

  AddRoundKey(state, roundKeys + Nr * 4 * Nb);

  for (round = Nr - 1; round >= 1; round--) {
    InvSubBytes(state);
    InvShiftRows(state);
    AddRoundKey(state, roundKeys + round * 4 * Nb);
    InvMixColumns(state);
  }

  InvSubBytes(state);
  InvShiftRows(state);
  AddRoundKey(state, roundKeys);

  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      out[i + 4 * j] = state[i][j];
    }
  }
}

void AES::SubBytes(unsigned char state[4][Nb]) {
  unsigned int i, j;
  unsigned char t;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      t = state[i][j];
      state[i][j] = sbox[t / 16][t % 16];
    }
  }
}

void AES::ShiftRow(unsigned char state[4][Nb], unsigned int i,
                   unsigned int n)  // shift row i on n positions
{
  unsigned char tmp[Nb];
  for (unsigned int j = 0; j < Nb; j++) {
    tmp[j] = state[i][(j + n) % Nb];
  }
  memcpy(state[i], tmp, Nb * sizeof(unsigned char));
}

void AES::ShiftRows(unsigned char state[4][Nb]) {
  ShiftRow(state, 1, 1);
  ShiftRow(state, 2, 2);
  ShiftRow(state, 3, 3);
}

unsigned char AES::xtime(unsigned char b)  // multiply on x
{
  return (b << 1) ^ (((b >> 7) & 1) * 0x1b);
}

void AES::MixColumns(unsigned char state[4][Nb]) {
  unsigned char temp_state[4][Nb];

  for (size_t i = 0; i < 4; ++i) {
    memset(temp_state[i], 0, 4);
  }

  for (size_t i = 0; i < 4; ++i) {
    for (size_t k = 0; k < 4; ++k) {
      for (size_t j = 0; j < 4; ++j) {
        if (CMDS[i][k] == 1)
          temp_state[i][j] ^= state[k][j];
        else
          temp_state[i][j] ^= GF_MUL_TABLE[CMDS[i][k]][state[k][j]];
      }
    }
  }

  for (size_t i = 0; i < 4; ++i) {
    memcpy(state[i], temp_state[i], 4);
  }
}

void AES::AddRoundKey(unsigned char state[4][Nb], unsigned char *key) {
  unsigned int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      state[i][j] = state[i][j] ^ key[i + 4 * j];
    }
  }
}

void AES::SubWord(unsigned char *a) {
  int i;
  for (i = 0; i < 4; i++) {
    a[i] = sbox[a[i] / 16][a[i] % 16];
  }
}

void AES::RotWord(unsigned char *a) {
  unsigned char c = a[0];
  a[0] = a[1];
  a[1] = a[2];
  a[2] = a[3];
  a[3] = c;
}

void AES::XorWords(unsigned char *a, unsigned char *b, unsigned char *c) {
  int i;
  for (i = 0; i < 4; i++) {
    c[i] = a[i] ^ b[i];
  }
}

void AES::Rcon(unsigned char *a, unsigned int n) {
  unsigned int i;
  unsigned char c = 1;
  for (i = 0; i < n - 1; i++) {
    c = xtime(c);
  }

  a[0] = c;
  a[1] = a[2] = a[3] = 0;
}

void AES::KeyExpansion(const unsigned char key[], unsigned char w[]) {
  unsigned char temp[4];
  unsigned char rcon[4];

  unsigned int i = 0;
  while (i < 4 * Nk) {
    w[i] = key[i];
    i++;
  }

  i = 4 * Nk;
  while (i < 4 * Nb * (Nr + 1)) {
    temp[0] = w[i - 4 + 0];
    temp[1] = w[i - 4 + 1];
    temp[2] = w[i - 4 + 2];
    temp[3] = w[i - 4 + 3];

    if (i / 4 % Nk == 0) {
      RotWord(temp);
      SubWord(temp);
      Rcon(rcon, i / (Nk * 4));
      XorWords(temp, rcon, temp);
    } else if (Nk > 6 && i / 4 % Nk == 4) {
      SubWord(temp);
    }

    w[i + 0] = w[i - 4 * Nk] ^ temp[0];
    w[i + 1] = w[i + 1 - 4 * Nk] ^ temp[1];
    w[i + 2] = w[i + 2 - 4 * Nk] ^ temp[2];
    w[i + 3] = w[i + 3 - 4 * Nk] ^ temp[3];
    i += 4;
  }
}

void AES::InvSubBytes(unsigned char state[4][Nb]) {
  unsigned int i, j;
  unsigned char t;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      t = state[i][j];
      state[i][j] = inv_sbox[t / 16][t % 16];
    }
  }
}

void AES::InvMixColumns(unsigned char state[4][Nb]) {
  unsigned char temp_state[4][Nb];

  for (size_t i = 0; i < 4; ++i) {
    memset(temp_state[i], 0, 4);
  }

  for (size_t i = 0; i < 4; ++i) {
    for (size_t k = 0; k < 4; ++k) {
      for (size_t j = 0; j < 4; ++j) {
        temp_state[i][j] ^= GF_MUL_TABLE[INV_CMDS[i][k]][state[k][j]];
      }
    }
  }

  for (size_t i = 0; i < 4; ++i) {
    memcpy(state[i], temp_state[i], 4);
  }
}

void AES::InvShiftRows(unsigned char state[4][Nb]) {
  ShiftRow(state, 1, Nb - 1);
  ShiftRow(state, 2, Nb - 2);
  ShiftRow(state, 3, Nb - 3);
}

void AES::XorBlocks(const unsigned char *a, const unsigned char *b,
                    unsigned char *c, unsigned int len) {
  for (unsigned int i = 0; i < len; i++) {
    c[i] = a[i] ^ b[i];
  }
}

void AES::printHexArray(unsigned char a[], unsigned int n) {
  for (unsigned int i = 0; i < n; i++) {
    printf("%02x ", a[i]);
  }
}

void AES::printHexVector(std::vector<unsigned char> a) {
  for (unsigned int i = 0; i < a.size(); i++) {
    printf("%02x ", a[i]);
  }
}

std::vector<unsigned char> AES::ArrayToVector(unsigned char *a,
                                              unsigned int len) {
  std::vector<unsigned char> v(a, a + len * sizeof(unsigned char));
  return v;
}

unsigned char *AES::VectorToArray(std::vector<unsigned char> &a) {
  return a.data();
}

std::vector<unsigned char> AES::EncryptECB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key) {
  unsigned char *out = EncryptECB(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key));
  std::vector<unsigned char> v = ArrayToVector(out, in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::DecryptECB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key) {
  unsigned char *out = DecryptECB(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key));
  std::vector<unsigned char> v = ArrayToVector(out, (unsigned int)in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::EncryptCBC(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv) {
  unsigned char *out = EncryptCBC(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key), VectorToArray(iv));
  std::vector<unsigned char> v = ArrayToVector(out, in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::DecryptCBC(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv) {
  unsigned char *out = DecryptCBC(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key), VectorToArray(iv));
  std::vector<unsigned char> v = ArrayToVector(out, (unsigned int)in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::EncryptCFB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv) {
  unsigned char *out = EncryptCFB(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key), VectorToArray(iv));
  std::vector<unsigned char> v = ArrayToVector(out, in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::DecryptCFB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv) {
  unsigned char *out = DecryptCFB(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key), VectorToArray(iv));
  std::vector<unsigned char> v = ArrayToVector(out, (unsigned int)in.size());
  delete[] out;
  return v;
}

void printHex(const std::vector<unsigned char>& data) {
    for (unsigned char byte : data) {
        printf("%02x ", byte);
    }
    printf("\n");
}

unsigned char* AES::EncryptOFB(const unsigned char in[], unsigned int inLen, const unsigned char key[], const unsigned char *iv) {
    unsigned char *out = new unsigned char[inLen];
    unsigned char block[blockBytesLen];
    unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
    KeyExpansion(key, roundKeys);
    memcpy(block, iv, blockBytesLen);
    for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
        EncryptBlock(block, block, roundKeys);
        unsigned int n = std::min(blockBytesLen, inLen - i);
        XorBlocks(in + i, block, out + i, n);
    }

    delete[] roundKeys;
    return out;
}

unsigned char* AES::DecryptOFB(const unsigned char in[], unsigned int inLen, const unsigned char key[], const unsigned char *iv) {
    return EncryptOFB(in, inLen, key, iv);  // OFB decryption is the same as encryption
}

std::vector<unsigned char> AES::EncryptOFB(std::vector<unsigned char> in, std::vector<unsigned char> key, std::vector<unsigned char> iv) {
    unsigned char *out = EncryptOFB(VectorToArray(in), (unsigned int)in.size(), VectorToArray(key), VectorToArray(iv));
    std::vector<unsigned char> v = ArrayToVector(out, in.size());
    delete[] out;
    return v;
}

std::vector<unsigned char> AES::DecryptOFB(std::vector<unsigned char> in, std::vector<unsigned char> key, std::vector<unsigned char> iv) {
    unsigned char *out = DecryptOFB(VectorToArray(in), (unsigned int)in.size(), VectorToArray(key), VectorToArray(iv));
    std::vector<unsigned char> v = ArrayToVector(out, (unsigned int)in.size());
    delete[] out;
    return v;
}


int main() {
    AES aes(AESKeyLength::AES_128);

    std::vector<unsigned char> key = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x7f, 0x67, 0x6f, 0xad, 0x74, 0x85
    };

    std::vector<unsigned char> iv = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };

    std::vector<unsigned char> plaintext = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
    };
// ********************** OFB **************************
    std::cout << "Plaintext: ";
    printHex(plaintext);

    std::vector<unsigned char> ciphertext = aes.EncryptOFB(plaintext, key, iv);

    std::cout << "Ciphertext (OFB): ";
    printHex(ciphertext);

    std::vector<unsigned char> decryptedtext = aes.DecryptOFB(ciphertext, key, iv);

    std::cout << "Decrypted text (OFB): ";
    printHex(decryptedtext);

    if (plaintext == decryptedtext) {
        std::cout << "OFB mode test passed!" << std::endl;
    } else {
        std::cout << "OFB mode test failed!" << std::endl;
    }

// ********************** CBC **************************

    ciphertext = aes.EncryptCBC(plaintext, key, iv);

    std::cout << "Ciphertext (CBC): ";
    printHex(ciphertext);

    decryptedtext = aes.DecryptCBC(ciphertext, key, iv);

    std::cout << "Decrypted text (CBC): ";
    printHex(decryptedtext);

    if (plaintext == decryptedtext) {
        std::cout << "CBC mode test passed!" << std::endl;
    } else {
        std::cout << "CBC mode test failed!" << std::endl;
    }


    return 0;
}