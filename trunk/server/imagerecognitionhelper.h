#ifndef IMAGERECOGNITIONHELPER_H
#define IMAGERECOGNITIONHELPER_H
#include <string>
#include <vector>
#include "curl/curl.h"
#include "json/json.h"
#include <QString>
#include <QVector>
#define NAMESPACE_NAME "20110509"
#define MAXIMUM_DAMAGE 100.0

typedef enum {
	IRH_ERROR_CURL,
	IRH_ERROR_JSON,
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
	void account_users(std::vector<std::string>& response); // doesn't put anything in response at the moment
	void account_limits(void);
	void faces_status(void);

private:
//	CURL* easy_handle;
	// needs to match the one created manually at developer.face.com
	std::string current_namespace;

	void post(std::string& post_data, std::string& post_url, Json::Value& decoded_response);
	void post_multipart(curl_httppost* post_data, std::string& post_url, Json::Value& decoded_response);
	void faces_detect(std::string& jpeg_image, std::string& tid_response);
	void faces_recognize(
	    std::vector<std::string>& uids,
	    std::string jpeg_image,
	    Json::Value& matched_uids,
	    double& distance_from_center,
	    double& face_tag_size);
	void faces_train(std::string&);
	void tags_remove(std::string&);
	void tags_save(std::string&, std::string&);
	unsigned select_face(Json::Value& decoded_response);
};

#endif // IMAGERECOGNITIONHELPER_H
