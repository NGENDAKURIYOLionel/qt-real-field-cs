#include "imagerecognitionhelper.h"
#include <iostream>
#include <vector>
//#include <fstream>
#include <cstdio>
#include <ctime>

static std::string test_namespace(NAMESPACE_NAME);
static ImageRecognitionHelper test_irh(test_namespace);

typedef enum {
	FILE_NOT_FOUND
} main_errors;

static void read_file(const char* src, std::string& target) {
	FILE* f = fopen(src, "rb");
	if (f == NULL) throw FILE_NOT_FOUND;
#define BUFSIZE 4096
	unsigned char buf[BUFSIZE];
	while(1) {
		size_t read = fread(buf, 1, BUFSIZE, f);
		if (read > 0) target.append((const char*)buf, read);
		if (feof(f)) break;
	}
	fclose(f);
}

static void register_test(std::string& uid, std::string& picture) {
	clock_t start = clock();
	std::cout << "registering user " << uid << std::endl;
	std::string jpeg_picture_data;
	try {
		read_file(picture.c_str(), jpeg_picture_data);
	} catch(...) {
		std::cout << "error reading image" << std::endl;
		return;
	}
	try {
		test_irh.register_player(uid, jpeg_picture_data);
	} catch(...) {
		std::cout << "error registering player" << std::endl;
	}

	clock_t end = clock();
	std::cout << "register: "
			  << (end-start)/(CLOCKS_PER_SEC/1000)
			  << " ms"
			  << std::endl;
}

static void users_test() {
	clock_t start = clock();
	std::vector<std::string> r;
	test_irh.account_users(r);
	for (unsigned j = 0; j < r.size(); j++) {
		std::cout << r[j] << std::endl;
	}
	clock_t end = clock();
	std::cout << "users: "
			  << (end-start)/(CLOCKS_PER_SEC/1000)
			  << " ms"
			  << std::endl;
}

static void status_test() {
	clock_t start = clock();
	test_irh.faces_status();
	clock_t end = clock();
	std::cout << "status: "
			  << (end-start)/(CLOCKS_PER_SEC/1000)
			  << " ms"
			  << std::endl;
}

static void match_test(std::string& picture, std::vector<std::string>& uids) {
	clock_t start = clock();
	std::cout << "matching picture to users" << std::endl;
//	for (unsigned i = 0; i < uids.size(); i++) {
//		std::cout << uids[i] << std::endl;
//	}
	std::string jpeg_picture_data;
	try {
		read_file(picture.c_str(), jpeg_picture_data);
	} catch(...) {
		std::cout << "error reading image" << std::endl;
		return;
	}
	std::string response;
	int damage = test_irh.match(response, jpeg_picture_data, uids);
	if (damage < 0) std::cout << "miss or no match" << std::endl;
	else {
		std::cout << "matched user \'" << response << "\' with damage " << damage << std::endl;
	}
	clock_t end = clock();
	std::cout << "match test: "
			  << (end-start)/(CLOCKS_PER_SEC/1000)
			  << " ms"
			  << std::endl;
}

int main(int argc, char *argv[])
{
	std::cout << "testing face.com..." << std::endl;
	try {
		std::vector<std::string> users;
		test_irh.account_users(users);
//		for (unsigned j = 0; j < users.size(); j++) {
//			std::cout << users[j] << std::endl;
//		}
	} catch (...) {
		std::cout << "error using face.com - is your namespace set correctly?" << std::endl;
		return 1;
	}
	std::cout << "test successful!" << std::endl;
	if (argc > 1) {
		std::string argv1(argv[1]);
		if (!argv1.compare("register")) {
			if (argc != 4) {
				std::cout << "usage: register uid face.jpg" << std::endl;
				return 0;
			}
			std::string uid(argv[2]);
			std::string picture(argv[3]);
			register_test(uid, picture);
			return 0;
		} else if (!argv1.compare("match")) {
			if (argc < 4) {
				std::cout << "usage: match face.jpg uid1 uid2 ..." << std::endl;
				return 0;
			}
			std::string picture(argv[2]);
			if (picture.size() < 1) {
				std::cout << "invalid picture" << std::endl;
				return 0;
			}
			std::vector<std::string> uids;
			unsigned players = argc - 3;
			for (unsigned i = 0; i < players; i++) {
//				std::cout << "player1: " << argv[i + 3] << std::endl;
				std::string temp_uid(argv[i + 3]);
				if (temp_uid.size() < 1) {
					std::cout << "error in uid" << std::endl;
					return 0;
				}
				uids.push_back(temp_uid);
			}
			match_test(picture, uids);
			return 0;
		} else if (!argv1.compare("users")) {
			users_test();
			return 0;
		} else if (!argv1.compare("status")) {
			status_test();
			return 0;
		}
	}
	std::cout << "valid command line arguments: register, match, users, status" << std::endl;
	return 0;
}
