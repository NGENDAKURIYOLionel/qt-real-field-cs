#include <string>
#include <iostream>
#include <vector>
#include "imagerecognitionhelper.h"
#include "curl/curl.h"

#define API_KEY "5f7a9cbd46993f8672eab2f8e2eb516b"
#define API_SECRET "a88ae67f870e64c3e7abebee10585552"
#define API_URL "http://api.face.com"
#define ACCOUNT_LIMITS_URL "/account/limits.json"

ImageRecognitionHelper::ImageRecognitionHelper() {
	easy_handle = curl_easy_init();
	if (!easy_handle) {
		// throw a tantrum
	}
}

ImageRecognitionHelper::~ImageRecognitionHelper() {
	if (easy_handle)
		curl_easy_cleanup(easy_handle);
}

// libcurl demands this not be a member function
static size_t response_callback(void* buffer, size_t size, size_t nmemb, void *userp) {
	((std::string*)userp)->append((const char*)buffer, size*nmemb);
	return size*nmemb;
}

void ImageRecognitionHelper::post() {
	std::cout << "testing" << std::endl;
	char post_data[] = "api_key=" API_KEY "&api_secret=" API_SECRET;
	std::string response;
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post_data); // dunno why qtcreator sees an error here
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, response_callback);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(easy_handle, CURLOPT_URL, API_URL ACCOUNT_LIMITS_URL);
	if (curl_easy_perform(easy_handle) != CURLE_OK) {
		// throw another tantrum
	} else {
		std::cout << response << std::endl;
	}
}

void ImageRecognitionHelper::post_multipart() {
}

game_id_t ImageRecognitionHelper::start_game(std::vector<jpeg_image_t>) {
	return (game_id_t)0; // placeholder
}

player_id_t ImageRecognitionHelper::match(jpeg_image_t, game_id_t) {
	return (player_id_t)0; // placeholder
}

void ImageRecognitionHelper::end_game(game_id_t) {
	// placeholder
}
