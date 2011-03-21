#ifndef IMAGERECOGNITIONHELPER_H
#define IMAGERECOGNITIONHELPER_H
#include "curl/curl.h"
#include <string>
#include <vector>

typedef unsigned game_id_t;
typedef unsigned player_id_t;
typedef std::string jpeg_image_t;

class ImageRecognitionHelper
{
public:
	ImageRecognitionHelper();
	~ImageRecognitionHelper();
	void post(); // will become private
	void post_multipart(); // will become private

	// "the API"
	game_id_t start_game(std::vector<jpeg_image_t>); // image index in vector = player_id
	player_id_t match(jpeg_image_t, game_id_t);
	void end_game(game_id_t);

private:
	CURL* easy_handle;
//	void decode_node(std::string, std::vector<std::string>&);
};

#endif // IMAGERECOGNITIONHELPER_H
