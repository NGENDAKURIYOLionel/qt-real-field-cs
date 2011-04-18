#ifndef IMAGERECOGNITIONHELPER_H
#define IMAGERECOGNITIONHELPER_H
#include <string>
#include <vector>
#include "curl/curl.h"
#include "json/json.h"

typedef unsigned game_id_t;
typedef unsigned player_id_t;
typedef enum {
	NOT_ENOUGH_PLAYERS,
	CURL_ERROR,
	JSON_ERROR,
	MALFORMED_RESPONSE
} errors_e;

class ImageRecognitionHelper
{
public:
	ImageRecognitionHelper(std::string&);
	~ImageRecognitionHelper();

	// "the API"
	void register_player(std::string& uid, std::string& jpeg_image);
	void match(std::string& response, std::string& jpeg_image, std::vector<std::string>& uids);
	void account_users(std::vector<std::string>& response);

	// deprecated
	game_id_t start_game(std::vector<std::string>&); // image index in vector = player_id
	player_id_t match(std::string&, game_id_t);
	void end_game(game_id_t);

private:
//	CURL* easy_handle;
	// needs to match the one created manually at developer.face.com
	std::string current_namespace;

	void post(std::string& post_data, std::string& post_url, Json::Value& decoded_response);
	void post_multipart(curl_httppost* post_data, std::string& post_url, Json::Value& decoded_response);
	void account_limits();
	void faces_detect(std::string& jpeg_image, std::string& tid_response);
	void faces_recognize(std::vector<std::string>& uids, std::string jpeg_image, std::string& response);
	void faces_train(std::string&);
	void tags_remove(std::string&);
	void tags_save(std::string&, std::string&);
	game_id_t games;
};

#endif // IMAGERECOGNITIONHELPER_H
