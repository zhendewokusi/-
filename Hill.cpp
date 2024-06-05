#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

using namespace std;

const int MOD = 26;

// 求整数 a 在模 m 下的逆元
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    throw runtime_error("Modular inverse doesn't exist");
}

// 计算矩阵的行列式
int determinant(const vector<vector<int>>& matrix) {
    if (matrix.size() == 2) {
        return (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % MOD;
    } else if (matrix.size() == 3) {
        int det = 0;
        for (int i = 0; i < 3; i++) {
            det += matrix[0][i] * (matrix[1][(i+1) % 3] * matrix[2][(i+2) % 3] - matrix[1][(i+2) % 3] * matrix[2][(i+1) % 3]);
            det %= MOD;
        }
        return det;
    }
    throw runtime_error("Only 2x2 and 3x3 matrices are supported");
}

// 计算其伴随矩阵
vector<vector<int>> adjoint(const vector<vector<int>>& matrix) {
    if (matrix.size() == 2) {
        return {
            { matrix[1][1], -matrix[0][1] },
            { -matrix[1][0], matrix[0][0] }
        };
    } else if (matrix.size() == 3) {
        vector<vector<int>> adj(3, vector<int>(3));
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                adj[i][j] = (matrix[(j+1)%3][(i+1)%3] * matrix[(j+2)%3][(i+2)%3] - matrix[(j+1)%3][(i+2)%3] * matrix[(j+2)%3][(i+1)%3]) % MOD;
                if (adj[i][j] < 0) adj[i][j] += MOD;
            }
        }
        return adj;
    }
    throw runtime_error("Only 2x2 and 3x3 matrices are supported");
}

// 计算给定矩阵的逆矩阵
vector<vector<int>> inverseMatrix(const vector<vector<int>>& matrix) {
    int det = determinant(matrix);
    // 矩阵的行列式为 0 则报错
    if (det == 0) {
        throw runtime_error("Matrix is not invertible");
    }
    // 计算其逆元
    int det_inv = modInverse(det, MOD);

    // 计算矩阵的伴随矩阵
    // 伴随矩阵是原矩阵的转置矩阵的每个元素的代数余子式
    vector<vector<int>> adj = adjoint(matrix);

    vector<vector<int>> inv_matrix(matrix.size(), vector<int>(matrix.size()));
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            inv_matrix[i][j] = (adj[i][j] * det_inv) % MOD;
            if (inv_matrix[i][j] < 0) inv_matrix[i][j] += MOD;
        }
    }

    return inv_matrix;
}

// 将给定的矩阵 matrix 与向量 vec 相乘
vector<int> multiplyMatrix(const vector<vector<int>>& matrix, const vector<int>& vec) {
    vector<int> result(matrix.size());
    for (int i = 0; i < matrix.size(); ++i) {
        result[i] = 0;
        for (int j = 0; j < matrix.size(); ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
        result[i] %= MOD;
    }KL

// 加密
string encrypt(const string& plaintext, const vector<vector<int>>& key) {
    string ciphertext;
    for (size_t i = 0; i < plaintext.length(); i += key.size()) {
        vector<int> vec(key.size(), 0);
        for (size_t j = 0; j < key.size(); ++j) {
            if (i + j < plaintext.length()) {
                vec[j] = plaintext[i + j] - 'A';
            }
        }
        vector<int> result = multiplyMatrix(key, vec);
        for (int r : result) {
            ciphertext += (r + 'A');
        }
    }
    return ciphertext;
}

// 解密
string decrypt(const string& ciphertext, const vector<vector<int>>& key) {
    vector<vector<int>> inv_key = inverseMatrix(key);
    string plaintext;
    for (size_t i = 0; i < ciphertext.length(); i += key.size()) {
        vector<int> vec(key.size(), 0);
        for (size_t j = 0; j < key.size(); ++j) {
            if (i + j < ciphertext.length()) {
                vec[j] = ciphertext[i + j] - 'A';
            }
        }
        vector<int> result = multiplyMatrix(inv_key, vec);
        for (int r : result) {
            plaintext += (r + 'A');
        }
    }
    return plaintext;
}

// 两数是否互质
bool areCoprime(int a, int b) {
    while (b) {
        // 欧几里得算法
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a == 1;
}

// 生成可逆的矩阵（行列式不等于 0）
vector<vector<int>> generateRandomMatrix(int size) {
    vector<vector<int>> matrix(size, vector<int>(size));
    do {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                matrix[i][j] = rand() % MOD;
            }
        }
    // 直至矩阵的行列式计算结果不为 0，且和 MOD 互质
    // 目的：求逆元（给定一个正整数 a 和一个模数 m，如果存在一个整数 b，使得 (a * b) % m = 1，那么 b 就被称为 a 在模 m 下的逆元。）
    } while (!areCoprime(determinant(matrix), MOD)); 

    return matrix;
}

// 打印矩阵
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    // 明文
    string message = "26224012ZFY";
//     string message = "26224011LYT";

    // 调整明文长度
    while (message.length() % 2 != 0) {
        message += 'X';
    }

    while (message.length() % 3 != 0) {
        message += 'X';
    }

    // 生成符合条件的随机可逆矩阵
    vector<vector<int>> key2x2 = generateRandomMatrix(2);
    vector<vector<int>> key3x3 = generateRandomMatrix(3);

    // 打印生成的矩阵
    cout << "2x2 Key Matrix:" << endl;
    printMatrix(key2x2);
    cout << "3x3 Key Matrix:" << endl;
    printMatrix(key3x3);

    // 计算其逆矩阵
    vector<vector<int>> inv_key2x2 = inverseMatrix(key2x2);
    vector<vector<int>> inv_key3x3 = inverseMatrix(key3x3);

    // 打印其逆矩阵
    cout << "Inverse 2x2 Key Matrix:" << endl;
    printMatrix(inv_key2x2);
    cout << "Inverse 3x3 Key Matrix:" << endl;
    printMatrix(inv_key3x3);

    // 加密过程
    string ciphertext2x2 = encrypt(message, key2x2);
    string ciphertext3x3 = encrypt(message, key3x3);

    // 加密结果
    cout << "Ciphertext using 2x2 Key: " << ciphertext2x2 << endl;
    cout << "Ciphertext using 3x3 Key: " << ciphertext3x3 << endl;

    // 解密过程
    string decrypted2x2 = decrypt(ciphertext2x2, key2x2);
    string decrypted3x3 = decrypt(ciphertext3x3, key3x3);

    // 解密结果
    cout << "Decrypted using 2x2 Key: " << decrypted2x2 << endl;
    cout << "Decrypted using 3x3 Key: " << decrypted3x3 << endl;

    return 0;
}