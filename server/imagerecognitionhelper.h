#ifndef IMAGERECOGNITIONHELPER_H
#define IMAGERECOGNITIONHELPER_H
#include <string>
#include <vector>
#include "curl/curl.h"
#include "json/json.h"
#include <QString>
#include <QVector>
#define NAMESPACE_NAME "20110423_2"

typedef unsigned game_id_t;
typedef unsigned player_id_t;
typedef enum {
	IRH_ERROR_NOT_ENOUGH_PLAYERS, // deprecated
	IRH_ERROR_CURL,
	IRH_ERROR_JSON,
	IRH_ERROR_MALFORMED_RESPONSE, // deprecated
	IRH_ERROR_FACE_DOT_COM, // face.com api returned other than status: success
	IRH_ERROR_PHOTO_HAS_NO_FACES,
	IRH_ERROR_PHOTO_HAS_NO_RECOGNIZABLE_FACE, // face.com says the face isn't recognizable
} errors_e;

class ImageRecognitionHelper
{
public:
	ImageRecognitionHelper(std::string&);
	~ImageRecognitionHelper();

	// "the API"
	void register_player(std::string& uid, std::string& jpeg_image);
	// match puts the uid of the hit player into response
	// returns damage or -1 if no hit (also nothing is put into response if no hit)
	int match(std::string& response, std::string& jpeg_image, std::vector<std::string>& uids);
	int match_all(std::string& response, std::string& jpeg_image);
	void account_users(std::vector<std::string>& response);
	void account_limits();

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
	void faces_detect(std::string& jpeg_image, std::string& tid_response);
	void faces_recognize(std::vector<std::string>& uids, std::string jpeg_image, std::string& response);
	void faces_train(std::string&);
	void tags_remove(std::string&);
	void tags_save(std::string&, std::string&);
	game_id_t games;
};

#endif // IMAGERECOGNITIONHELPER_H
