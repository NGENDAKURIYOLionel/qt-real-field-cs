#include <string>
#include <iostream>
#include <vector>
#include "imagerecognitionhelper.h"
#include "curl/curl.h"
#include "json/json.h"
#include <cstdio> // debug
#include <ctime> // debug

#define API_KEY "5f7a9cbd46993f8672eab2f8e2eb516b"
#define API_SECRET "a88ae67f870e64c3e7abebee10585552"

// use compile time concatenation with these
#define API_URL "http://api.face.com"
#define ACCOUNT_LIMITS_URL "/account/limits.json"
#define ACCOUNT_USERS_URL "/account/users.json"
#define FACES_DETECT_URL "/faces/detect.json"
#define FACES_RECOGNIZE_URL "/faces/recognize.json"
#define FACES_TRAIN_URL "/faces/train.json"
#define TAGS_REMOVE_URL "/tags/remove.json"
#define TAGS_SAVE_URL "/tags/save.json"

ImageRecognitionHelper::ImageRecognitionHelper(std::string& api_namespace) {
	if (api_namespace.empty()) throw;
	current_namespace = std::string(api_namespace);
	curl_global_init(CURL_GLOBAL_ALL);
	games = 0;
}

ImageRecognitionHelper::~ImageRecognitionHelper() {
}

// libcurl demands this not be a member function
static size_t response_callback(void* buffer, size_t size, size_t nmemb, void *userp) {
//	clock_t start = clock();
	((std::string*)userp)->append((const char*)buffer, size*nmemb);
//	clock_t end = clock();
//	std::cout << "callback overhead: " << (end-start)/(CLOCKS_PER_SEC/1000) << std::endl;
	return size*nmemb;
}

void ImageRecognitionHelper::post(std::string& post_data,
								  std::string& post_url,
								  Json::Value& decoded_response) {
	CURL* easy_handle = curl_easy_init();
	if (!easy_handle) throw IRH_ERROR_CURL;
	std::string response;
	// dunno why qtcreator sees an error here
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post_data.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, response_callback);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(easy_handle, CURLOPT_URL, post_url.c_str());
	if (curl_easy_perform(easy_handle) != CURLE_OK) throw IRH_ERROR_CURL;
	if (easy_handle) curl_easy_cleanup(easy_handle);
	Json::Reader json_reader;
	if (!json_reader.parse(response, decoded_response, false)) throw IRH_ERROR_JSON;
}

void ImageRecognitionHelper::post_multipart(curl_httppost* post_data,
											std::string& post_url,
											Json::Value& decoded_response) {
	CURL* easy_handle = curl_easy_init();
	if (!easy_handle) throw IRH_ERROR_CURL;
	curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Expect:"); // disable "expect" header
	curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, post_data);
	curl_easy_setopt(easy_handle, CURLOPT_URL, post_url.c_str());
	std::string response;
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, response_callback);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);
//	clock_t start = clock();
	if (curl_easy_perform(easy_handle) != CURLE_OK) throw IRH_ERROR_CURL;
//	clock_t end = clock();
//	std::cout << "request: "
//	          << (end-start)/(CLOCKS_PER_SEC/1000)
//	          << " ms"
//	          << std::endl;
	if (easy_handle) curl_easy_cleanup(easy_handle);
	curl_formfree(post_data);
	curl_slist_free_all(headers);
	Json::Reader json_reader;
	if (!json_reader.parse(response, decoded_response, false)) throw IRH_ERROR_JSON;
}

void ImageRecognitionHelper::account_limits() {
	std::string post_data("api_key=" API_KEY "&api_secret=" API_SECRET);
	std::string post_url(API_URL ACCOUNT_LIMITS_URL);
	Json::Value decoded_response;
	post(post_data, post_url, decoded_response);
	std::cout << decoded_response << std::endl;
}

void ImageRecognitionHelper::account_users(std::vector<std::string>& response) {
	std::string post_data("api_key=" API_KEY "&api_secret=" API_SECRET "&namespaces=");
	post_data.append(current_namespace);
	std::string post_url(API_URL ACCOUNT_USERS_URL);
	Json::Value decoded_response;
	post(post_data, post_url, decoded_response);
	if (decoded_response["status"].asString().compare("success"))
		throw IRH_ERROR_FACE_DOT_COM;
	else std::cout << decoded_response;
	//for (unsigned i = 0; i < decoded_response.)
}

void ImageRecognitionHelper::tags_save(std::string& tid, std::string& uid) {
	std::string post_data("api_key=" API_KEY "&api_secret=" API_SECRET "&uid=");
	post_data += uid;
	post_data += "&tids=";
	post_data += tid;
	std::string post_url(API_URL TAGS_SAVE_URL);
	Json::Value decoded_response;
	post(post_data, post_url, decoded_response);
	std::cout << decoded_response << std::endl;
	if (decoded_response["status"].asString().compare("success"))
		throw IRH_ERROR_FACE_DOT_COM;
}

void ImageRecognitionHelper::faces_detect(std::string& jpeg_image, std::string& tid_response) {
	curl_httppost* post_data = NULL; // gets freed by post_multipart
	curl_httppost* last = NULL;
	curl_formadd(&post_data, &last,
	             CURLFORM_COPYNAME, "api_key",
	             CURLFORM_COPYCONTENTS, API_KEY,
	             CURLFORM_END);
	curl_formadd(&post_data, &last,
	             CURLFORM_COPYNAME, "api_secret",
	             CURLFORM_COPYCONTENTS, API_SECRET,
	             CURLFORM_END);
	curl_formadd(&post_data, &last,
	             CURLFORM_COPYNAME, "file",
	             CURLFORM_CONTENTTYPE, "image/jpeg",
	             CURLFORM_BUFFER, "file",
	             CURLFORM_BUFFERPTR, jpeg_image.data(),
	             CURLFORM_BUFFERLENGTH, jpeg_image.size(),
	             CURLFORM_END);
	std::string post_url(API_URL FACES_DETECT_URL);
	Json::Value decoded_response;
	post_multipart(post_data, post_url, decoded_response);
	if (decoded_response["status"].asString().compare("success"))
		throw IRH_ERROR_FACE_DOT_COM;

	// are values returned by reference supposed to be freed at some point?
//	Json::Value& photos_array = decoded_response["photos"];
//	if (photos_array.isNull()) throw MALFORMED_RESPONSE;
//	if (!photos_array.isArray()) throw MALFORMED_RESPONSE;
//	if (photos_array.size() < 1) throw MALFORMED_RESPONSE;
//	if (!photos_array.isObject()) throw MALFORMED_RESPONSE;
//	if (photo_object["tags"].isNull()) throw MALFORMED_RESPONSE;

	// pick one face
//	Json::Value& tags_array = decoded_response["photos"][0u]["tags"];
//	if (!tags_array.isArray()) throw MALFORMED_RESPONSE;
//	if (tags_array.size() < 1) throw MALFORMED_RESPONSE;
//	for (unsigned i = 0; i < tags_array.size(); i++) {
//		std::cout << decoded_response["photos"][0u]["tags"][0u]["recognizable"].asBool() << std::endl;
//	}

	// pick first face found in photo
	// TODO: check if face recognizable
	// FIXME: check that the referenced object actually exists! (might return null)
//	std::cout << decoded_response["photos"][0u]["tags"].size() << std::endl;
	std::cout << decoded_response << std::endl;
	if (decoded_response["photos"][0u]["tags"].size() < 1) throw IRH_ERROR_PHOTO_HAS_NO_FACES;
	// TODO: if more than one face, pick the most likely (center of picture, best confidence, etc)
	if (!decoded_response["photos"][0u]["tags"][0u]["recognizable"].asBoolean())
		throw IRH_ERROR_PHOTO_HAS_NO_RECOGNIZABLE_FACE;
	tid_response.assign(decoded_response["photos"][0u]["tags"][0u]["tid"].asString());
}

void ImageRecognitionHelper::faces_recognize(std::vector<std::string>& uids,
                                             std::string jpeg_image,
                                             std::string& response) {
	std::string uids_comma_separated;
	if (uids.size() < 1) throw;
	uids_comma_separated += uids[0];
	if (uids.size() > 1) {
		for (unsigned i = 1; i < uids.size(); i++) {
			uids_comma_separated += ',';
			uids_comma_separated += uids[i];
		}
	}
	curl_httppost* post_data = NULL; // gets freed by post_multipart
	curl_httppost* last = NULL;
	curl_formadd(&post_data, &last,
	             CURLFORM_COPYNAME, "api_key",
	             CURLFORM_COPYCONTENTS, API_KEY,
	             CURLFORM_END);
	curl_formadd(&post_data, &last,
	             CURLFORM_COPYNAME, "api_secret",
	             CURLFORM_COPYCONTENTS, API_SECRET,
	             CURLFORM_END);
	curl_formadd(&post_data, &last,
	             CURLFORM_COPYNAME, "uids",
	             CURLFORM_COPYCONTENTS, uids_comma_separated.c_str(),
	             CURLFORM_END);
	curl_formadd(&post_data, &last,
	             CURLFORM_COPYNAME, "file",
	             CURLFORM_CONTENTTYPE, "image/jpeg",
	             CURLFORM_BUFFER, "file",
	             CURLFORM_BUFFERPTR, jpeg_image.data(),
	             CURLFORM_BUFFERLENGTH, jpeg_image.size(),
	             CURLFORM_END);
	std::string post_url(API_URL FACES_RECOGNIZE_URL);
	Json::Value decoded_response;
	post_multipart(post_data, post_url, decoded_response);
	std::cout << decoded_response << std::endl;
}

void ImageRecognitionHelper::faces_train(std::string& uid) {
	std::string post_data("api_key=" API_KEY "&api_secret=" API_SECRET "&uids=");
	post_data += uid;
	std::string post_url(API_URL FACES_TRAIN_URL);
	Json::Value decoded_response;
	post(post_data, post_url, decoded_response);
	std::cout << decoded_response << std::endl;
	if (decoded_response["status"].asString().compare("success"))
		throw IRH_ERROR_FACE_DOT_COM;
}

void ImageRecognitionHelper::register_player(std::string& uid, std::string& jpeg_image) {
	std::string new_face;
	std::string uid_with_namespace(uid);
	uid_with_namespace += "@" + current_namespace;
	faces_detect(jpeg_image, new_face);
	tags_save(new_face, uid_with_namespace);
	faces_train(uid_with_namespace);
}

// TODO: ignore users from other namespaces
void ImageRecognitionHelper::match(std::string& response,
                                   std::string& jpeg_image,
                                   std::vector<std::string>& uids) {
	if (uids.size() < 1) throw;
	if (jpeg_image.size() < 1) throw;
	std::vector<std::string> uids_with_namespace(uids);
	for (unsigned i = 0; i < uids_with_namespace.size(); i++) {
		uids_with_namespace[i] += "@" + current_namespace;
	}
	std::string temp_response;
	faces_recognize(uids_with_namespace, jpeg_image, temp_response);
}

// deprecated stuff

game_id_t ImageRecognitionHelper::start_game(std::vector<std::string>& x) {
//	std::cout << x.size() << std::endl;
	if (x.size() < 1) throw IRH_ERROR_NOT_ENOUGH_PLAYERS;
	// detect all images
	game_id_t g = games;
	games++;
	for (unsigned i = 0; i < x.size(); i++) {
//		std::cout << i->size() << std::endl;
		// detect
		std::string current_tid;
		clock_t start = clock();
		faces_detect(x[i], current_tid);
		clock_t end = clock();
		std::cout << "faces_detect: "
		          << (end-start)/(CLOCKS_PER_SEC/1000)
		          << " ms"
		          << std::endl;
		// create temp uid for player
		std::string player_uid;
#define LONG_STR_SIZE 128
		char long_str[LONG_STR_SIZE]; // should be big enough for the decimal representation of time
		snprintf(long_str, LONG_STR_SIZE, "%lu", time(NULL)); // timestamp
		player_uid += long_str;
		player_uid += '_';
		snprintf(long_str, LONG_STR_SIZE, "%lu", clock()); // timestamp pt2
		player_uid += long_str;
		player_uid += '_';
		snprintf(long_str, LONG_STR_SIZE, "%lu", (unsigned long)g); // game id
		player_uid += long_str;
		player_uid += '_';
		snprintf(long_str, LONG_STR_SIZE, "%u", i); // player id
		player_uid += long_str;
		std::cout << player_uid << " " << current_tid << std::endl;
		player_uid += '@';
		player_uid += current_namespace;
		// associate face with player
		start = clock();
		tags_save(current_tid, player_uid);
		end = clock();
		std::cout << "tags_save: "
		          << (end-start)/(CLOCKS_PER_SEC/1000)
		          << " ms"
		          << std::endl;
		// train index
		start = clock();
		faces_train(player_uid);
		end = clock();
		std::cout << "faces_train: "
		          << (end-start)/(CLOCKS_PER_SEC/1000)
		          << " ms"
		          << std::endl;
	}
	return g; // placeholder
}

player_id_t ImageRecognitionHelper::match(std::string& x, game_id_t) {

	return (player_id_t)0; // placeholder
}

void ImageRecognitionHelper::end_game(game_id_t) {
	// placeholder
}
