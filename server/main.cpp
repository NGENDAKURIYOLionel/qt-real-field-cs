#include "imagerecognitionhelper.h"
#include <iostream>
#include <vector>
//#include <fstream>
#include <cstdio>
#include <ctime>
#define NAMESPACE_NAME "20110415"

void read_file(const char* src, jpeg_image_t& target) {
	FILE* f = fopen(src, "rb");
	if (f == NULL) throw;
#define BUFSIZE 4096
	unsigned char buf[BUFSIZE];
	while(1) {
		size_t read = fread(buf, 1, BUFSIZE, f);
		if (read > 0) target.append((const char*)buf, read);
		if (feof(f)) break;
	}
	fclose(f);
}

int main(int argc, char *argv[])
{
	clock_t start = clock();
	jpeg_image_t user1_reg;
	jpeg_image_t user1_detect;
	jpeg_image_t user2_reg;
	jpeg_image_t user2_detect;
	read_file("user1_reg.jpg", user1_reg);
	read_file("user1_detect.jpg", user1_detect);
	read_file("user2_reg.jpg", user2_reg);
	read_file("user2_detect.jpg", user2_detect);
	namespace_t current_namespace(NAMESPACE_NAME);
	ImageRecognitionHelper irh(current_namespace);
	jpeg_images_t players;
	players.push_back(user1_reg);
	players.push_back(user2_reg);
	uid_t user1("asdf");
	uid_t user2("qwe");
	irh.register_player(user1);
	irh.register_player(user2);
//	irh.account_limits();
//	irh.start_game(players);
	clock_t end = clock();
	std::cout << "whole program: "
			  << (end-start)/(CLOCKS_PER_SEC/1000)
			  << " ms"
			  << std::endl;
}
