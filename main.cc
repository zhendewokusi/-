#include <iostream>
#include "实验1/AES.h"
#include "实验1/Hill.h"
#include "实验2/RSA.h"
#include "实验2/ElGamal.h"

int main() {
	std::cout << "---TEST 1-1---" << std::endl;
	test1_1();
	std::cout << "---TEST 1-3---" << std::endl;
	test1_3();
	std::cout << "---TEST 2-1---" << std::endl;
	test2_1();
	std::cout << "---TEST 2-2---" << std::endl;
	test2_2();
	std::cout << "---TEST 2-3---" << std::endl;
	test2_3();
	return 0;
}