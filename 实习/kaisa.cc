#include <iostream>
#include <string>

using namespace std;

// 凯撒加密函数
string caesarEncrypt(const string& text, int shift) {
    string result = "";
    for (char ch : text) {
        if (isalpha(ch)) {
            char offset = isupper(ch) ? 'A' : 'a';
            ch = (ch - offset + shift) % 26 + offset;
        }
        result += ch;
    }
    return result;
}

// 凯撒解密函数
string caesarDecrypt(const string& text, int shift) {
    return caesarEncrypt(text, 26 - shift);
}

// 凯撒暴力破解函数
void caesarBruteForce(const string& text) {
    for (int shift = 1; shift < 26; ++shift) {
        string decrypted = caesarDecrypt(text, shift);
        cout << "Shift " << shift << ": " << decrypted << endl;
    }
}

int main() {
    string text;
    int shift;
    
    cout << "Enter text: ";
    getline(cin, text);
    
    cout << "Enter shift (1-25): ";
    cin >> shift;

    string encrypted = caesarEncrypt(text, shift);
    cout << "Encrypted text: " << encrypted << endl;

    string decrypted = caesarDecrypt(encrypted, shift);
    cout << "Decrypted text: " << decrypted << endl;

    cout << "Brute force results:" << endl;
    caesarBruteForce(encrypted);

    return 0;
}
// OmqemdOubtqdUeNdawqz