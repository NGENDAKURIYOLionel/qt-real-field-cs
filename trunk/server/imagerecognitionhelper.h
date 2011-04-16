#ifndef IMAGERECOGNITIONHELPER_H
#define IMAGERECOGNITIONHELPER_H
#include <string>
#include <vector>
#include "curl/curl.h"
#include "json/json.h"

typedef unsigned game_id_t;
typedef unsigned player_id_t;
typedef std::string jpeg_image_t;
typedef std::vector<jpeg_image_t> jpeg_images_t;
typedef std::string tid_t;
typedef std::string uid_t;
typedef std::vector<uid_t> uids_t;
typedef std::string namespace_t;
typedef enum {
	NOT_ENOUGH_PLAYERS,
	CURL_ERROR,
	JSON_ERROR,
	MALFORMED_RESPONSE
} errors_e;

class ImageRecognitionHelper
{
public:
	ImageRecognitionHelper(namespace_t&);
	~ImageRecognitionHelper();

	// "the API"
	void register_player(uid_t&, jpeg_image_t&);
	void match(uid_t& response, jpeg_image_t&, uids_t&);

	// deprecated
	game_id_t start_game(jpeg_images_t&); // image index in vector = player_id
	player_id_t match(jpeg_image_t&, game_id_t);
	void end_game(game_id_t);

private:
//	CURL* easy_handle;
	// needs to match the one created manually at developer.face.com
	namespace_t current_namespace;

	void post(std::string&, std::string&, Json::Value&);
	void post_multipart(curl_httppost*, std::string&, Json::Value&);
	void account_limits();
	void faces_detect(jpeg_image_t&, tid_t&);
	void faces_recognize(uids_t&, jpeg_image_t);
	void faces_train(uid_t&);
	void tags_remove(tid_t&);
	void tags_save(tid_t&, uid_t&);
	game_id_t games;
};

#endif // IMAGERECOGNITIONHELPER_H
