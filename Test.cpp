#include <cstdio>
#include <cstring>
#include "AES.h"

// 在CBC模式中，每个明文块先与前一个密文块进行异或后，再进行加密。
// 输出反馈模式（Output feedback, OFB）可以将块密码变成同步的流密码。它产生密钥流的块，然后将其与明文块进行异或，得到密文。与其它流密码一样，密文中一个位的翻转会使明文中同样位置的位也产生翻转。

// https://zh.wikipedia.org/wiki/%E9%AB%98%E7%BA%A7%E5%8A%A0%E5%AF%86%E6%A0%87%E5%87%86

void print(unsigned char* state, int len);

int main(int argc, char* argv[])
{
	unsigned char input[] = "hello world";
	unsigned char iv[]    = {103,35,148,239,76,213,47,118,255,222,123,176,106,134,98,92};
	unsigned char key[]   = {143,194,34,208,145,203,230,143,177,246,97,206,145,92,255,84};
	unsigned char output[100] ={0};
	unsigned char temp[100] = {0};
	AESModeOfOperation moo;
	moo.set_key(key);
	moo.set_mode(MODE_OFB);
	moo.set_iv(iv);
	int olen = sizeof input;

	memcpy(temp, input, sizeof input);
	int len = moo.Encrypt(temp, olen, output);
	printf("len = %d\n", len);
	printf("output");
	print(output, len);
	printf("\n\nDecrypt----------\n");
	len = moo.Decrypt(output, len, input);
	printf("len = %d\n", len);
	printf("input");
	// print(input, len);
	printf("%s\n",output);
	
	return 0;
}

void print(unsigned char* state, int len)
{
	int i;
	for(i=0; i<len; i++)
	{
	if ( i%16 == 0) printf("\n");
	// printf("%s%X ",state[i]>15 ? "" : "0", state[i]);
	// printf("%d  ", (int)(state[i] & 0xff));
	}
	printf("\n");
}

