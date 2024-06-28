#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// 埃拉托色尼筛法函数
vector<int> sieveOfEratosthenes(int n) {
    vector<bool> isPrime(n, true);
    vector<int> primes;
    
    // 0 和 1 不是素数
    isPrime[0] = isPrime[1] = false;

    for (int i = 2; i <= sqrt(n); ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j < n; j += i) {
                isPrime[j] = false;
            }
        }
    }

    for (int i = 2; i < n; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}

int main() {
	int n;
	cout << "Enter a positive integer: ";
	cin >> n;

	vector<int> primes = sieveOfEratosthenes(n);

	cout << "Prime numbers less than " << n << " are: ";
	for (int prime : primes) {
		cout << prime << " ";
	}
	cout << endl;

	return 0;
}
