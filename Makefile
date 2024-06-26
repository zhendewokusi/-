all:
	g++ main.cc 实验1/AES.cpp 实验1/Hill.cpp 实验2/ElGamal.cc 实验2/RSA.cc 实验3/hmac.cc -g -o 实验 -lssl -lcrypto