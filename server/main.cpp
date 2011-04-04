#include "imagerecognitionhelper.h"
#include <iostream>
#include <vector>
//#include <fstream>
#include <cstdio>
#include <ctime>
#define BUFSIZE 4096
#define NAMESPACE_NAME "T1105130_2"

int main(int argc, char *argv[])
{
	clock_t start = clock();
	FILE* file1 = fopen("front.jpg", "rb");
	if (file1 == NULL) throw;
	FILE* file2 = fopen("diagonal.jpg", "rb");
	if (file2 == NULL) throw;
	jpeg_image_t player1;
	unsigned char buf[BUFSIZE];
	while(1) {
		size_t read = fread(buf, 1, BUFSIZE, file1);
		player1.append((const char*)buf, read);
		if (feof(file1)) break;
	}
	fclose(file1);
	jpeg_image_t player2;
	while(1) {
		size_t read = fread(buf, 1, BUFSIZE, file2);
		player2.append((const char*)buf, read);
		if (feof(file2)) break;
	}
	fclose(file2);
	namespace_t current_namespace(NAMESPACE_NAME);
	ImageRecognitionHelper irh(current_namespace);
	jpeg_images_t players;
	players.push_back(player1);
	players.push_back(player2);
//	irh.account_limits();
	irh.start_game(players);
	clock_t end = clock();
	std::cout
			<< "whole program: "
			<< (end-start)/(CLOCKS_PER_SEC/1000)
			<< " ms"
			<< std::endl;
}
