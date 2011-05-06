#include <string>
#include <iostream>
#include <vector>
#include "imagerecognitionhelper.h"
#include "curl/curl.h"
#include "json/json.h"
#include <cstdio> // debug
#include <ctime> // debug
#include <QString>
#include <QVector>
#include <QByteArray>
#include <cmath>

#define API_KEY "5f7a9cbd46993f8672eab2f8e2eb516b"
#define API_SECRET "a88ae67f870e64c3e7abebee10585552"

// use compile time concatenation with these
#define API_URL "http://api.face.com"
#define ACCOUNT_LIMITS_URL "/account/limits.json"
#define ACCOUNT_USERS_URL "/account/users.json"
#define FACES_DETECT_URL "/faces/detect.json"
#define FACES_RECOGNIZE_URL "/faces/recognize.json"
#define FACES_STATUS_URL "/faces/status.json"
#define FACES_TRAIN_URL "/faces/train.json"
#define TAGS_REMOVE_URL "/tags/remove.json"
#define TAGS_SAVE_URL "/tags/save.json"

#define IMAGE_CENTER 50.0

ImageRecognitionHelper::ImageRecognitionHelper(std::string& api_namespace) {
	if (api_namespace.empty()) throw;
	current_namespace = std::string(api_namespace);
	curl_global_init(CURL_GLOBAL_ALL);
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

// TODO: increase tcp send window
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
//	clock_t start = clock();
	if (curl_easy_perform(easy_handle) != CURLE_OK) throw IRH_ERROR_CURL;
//	clock_t end = clock();
//	std::cout << "face.com response time: "
//	          << (end-start)/(CLOCKS_PER_SEC/1000)
//	          << " ms"
//	          << std::endl;
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
//	std::cout << "face.com response time: "
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
	//std::cout << decoded_response << std::endl;
	if (decoded_response["status"].asString().compare("success")) {
		throw IRH_ERROR_FACE_DOT_COM;
	}
	unsigned size = decoded_response["users"][NAMESPACE_NAME].size();
	for (unsigned i = 0; i < size; i++) {
		std::string temp_uid(decoded_response["users"][NAMESPACE_NAME][i].asString());
		unsigned index = temp_uid.find_first_of('@');
		std::string uid_without_namespace = temp_uid.substr(0, index);
		//std::cout << uid_without_namespace << std::endl;
		if (uid_without_namespace.size() <= 0) throw;
		response.push_back(uid_without_namespace.c_str());
	}
}

void ImageRecognitionHelper::tags_save(std::string& tid, std::string& uid) {
	std::string post_data("api_key=" API_KEY "&api_secret=" API_SECRET "&uid=");
	post_data += uid;
	post_data += "&tids=";
	post_data += tid;
	std::string post_url(API_URL TAGS_SAVE_URL);
	Json::Value decoded_response;
	post(post_data, post_url, decoded_response);
//	std::cout << decoded_response << std::endl;
	if (decoded_response["status"].asString().compare("success")) {
		std::cout << decoded_response << std::endl;
		throw IRH_ERROR_FACE_DOT_COM;
	}
}

unsigned ImageRecognitionHelper::select_face(Json::Value& decoded_response) {
//	std::cout << decoded_response["photos"][0u]["tags"] << std::endl; // DEBUG
	unsigned faces = decoded_response["photos"][0u]["tags"].size();
	if (faces < 1) {
                std::cout << "FYI: face.com found no face in the picture" << std::endl;
		throw IRH_ERROR_PHOTO_HAS_NO_FACES;
	}
	unsigned target_face = 0;
//	std::cout << decoded_response["photos"][0u]["tags"][target_face]["uids"] << std::endl; // DEBUG
	if (faces > 1) {
		double x = IMAGE_CENTER - decoded_response["photos"][0u]["tags"][0u]["center"]["x"].asDouble();
		double y = IMAGE_CENTER - decoded_response["photos"][0u]["tags"][0u]["center"]["y"].asDouble();
		double smallest_diff = x*x + y*y; // approx. without sqrt
//		std::cout << smallest_diff
//		          << " "
//		          << decoded_response["photos"][0u]["tags"][target_face]["uids"]
//		          << std::endl; // DEBUG
		for (unsigned i = 1; i < faces; i++) {
			x = IMAGE_CENTER - decoded_response["photos"][0u]["tags"][i]["center"]["x"].asDouble();
			y = IMAGE_CENTER - decoded_response["photos"][0u]["tags"][i]["center"]["x"].asDouble();
			double new_diff = x*x + y*y;
//			std::cout << new_diff
//			          << " "
//			          << decoded_response["photos"][0u]["tags"][i]["uids"]
//			          << std::endl; // DEBUG
			if (new_diff < smallest_diff) {
				smallest_diff = new_diff;
				target_face = i;
			}
		}
	}
	if (!decoded_response["photos"][0u]["tags"][target_face]["recognizable"].asBool()) {
		std::cout << "FYI: detected face not recognizable" << std::endl;
	}
	return target_face;
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
//	curl_formadd(&post_data, &last,
//	             CURLFORM_COPYNAME, "detector",
//	             CURLFORM_COPYCONTENTS, "Aggressive",
//	             CURLFORM_END);
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
//	std::cout << decoded_response << std::endl; // DEBUG
	if (decoded_response["status"].asString().compare("success")) {
		std::cout << decoded_response << std::endl;
		throw IRH_ERROR_FACE_DOT_COM;
	}

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
	// FIXME: check that the referenced object actually exists! (might return null)
//	std::cout << decoded_response["photos"][0u]["tags"].size() << std::endl;
//	std::cout << decoded_response << std::endl;
	unsigned target_face = select_face(decoded_response);

	if (!decoded_response["photos"][0u]["tags"][target_face]["recognizable"].asBool()) {
//		std::cout << decoded_response << std::endl;
		throw IRH_ERROR_PHOTO_HAS_NO_RECOGNIZABLE_FACE;
	}
	tid_response.assign(decoded_response["photos"][0u]["tags"][target_face]["tid"].asString());
}

void ImageRecognitionHelper::faces_recognize(std::vector<std::string>& uids,
                                             std::string jpeg_image,
                                             Json::Value& matched_uids,
                                             double& distance_from_center,
                                             double& face_tag_size) {
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
//	std::cout << "DEBUG: " << std::endl << decoded_response["photos"][0u]["tags"] << std::endl; // DEBUG
	if (decoded_response["status"].asString().compare("success")) {
		std::cout << "FYI: face.com status failure" << std::endl;
		throw IRH_ERROR_FACE_DOT_COM;
	}
	unsigned target_face = select_face(decoded_response);
//	face_response.assign(decoded_response["photos"][0u]["tags"][target_face].asString());
//	decoded_response["photos"][0u]["tags"][target_face]["uids"];
	matched_uids = decoded_response["photos"][0u]["tags"][target_face]["uids"];
	double x = IMAGE_CENTER - decoded_response["photos"][0u]["tags"][target_face]["center"]["x"].asDouble();
	double y = IMAGE_CENTER - decoded_response["photos"][0u]["tags"][target_face]["center"]["y"].asDouble();
	distance_from_center = sqrt(x*x + y*y);
	double width = decoded_response["photos"][0u]["tags"][target_face]["width"].asDouble();
	double height = decoded_response["photos"][0u]["tags"][target_face]["height"].asDouble();
	face_tag_size = sqrt(width * height);
}

void ImageRecognitionHelper::faces_train(std::string& uid) {
	std::string post_data("api_key=" API_KEY "&api_secret=" API_SECRET "&uids=");
	post_data += uid;
	std::string post_url(API_URL FACES_TRAIN_URL);
	Json::Value decoded_response;
	post(post_data, post_url, decoded_response);
//	std::cout << decoded_response << std::endl;
	if (decoded_response["status"].asString().compare("success")) {
		std::cout << decoded_response << std::endl;
		throw IRH_ERROR_FACE_DOT_COM;
	}
}

void ImageRecognitionHelper::register_player(std::string& uid, std::string& jpeg_image) {
	std::string new_face;
	std::string uid_with_namespace(uid);
	uid_with_namespace += "@" + current_namespace;
	faces_detect(jpeg_image, new_face);
	tags_save(new_face, uid_with_namespace);
	faces_train(uid_with_namespace);
}

// TODO: estimate distance from tag width & height
int ImageRecognitionHelper::match(std::string& response,
                                  std::string& jpeg_image,
                                  std::vector<std::string>& uids) {
	if (uids.size() < 1) throw;
	if (jpeg_image.size() < 1) throw;
	std::vector<std::string> uids_with_namespace(uids);
	for (unsigned i = 0; i < uids_with_namespace.size(); i++) {
		uids_with_namespace[i] += "@" + current_namespace;
	}
	Json::Value uids_response;
	try {
		double distance_from_center;
		double face_tag_size;
		faces_recognize(uids_with_namespace,
		                jpeg_image,
		                uids_response,
		                distance_from_center,
		                face_tag_size);
//		std::cout << uids_response << std::endl; // DEBUG
//		std::cout << "returned uids: " << uids_response << std::endl; // DEBUG
//		std::cout << face_tag_size << std::endl; // DEBUG
		if (distance_from_center > face_tag_size) {
			// closest face too far from center
			std::cout << "FYI: missed, distance "
			          << distance_from_center
			          << ", size "
			          << face_tag_size
			          << std::endl; // DEBUG
			return -1;
		}
		unsigned matched_uids = uids_response.size();
		if (matched_uids == 0) {
			std::cout << "FYI: detected face but matched no-one" << std::endl;
			return -1; // no matched UIDs
		}

		bool first_found = false;
		unsigned i = 0;
		std::string uid_part;
		while (i < matched_uids) {
			std::string temp_uid(uids_response[i]["uid"].asString());
			unsigned index = temp_uid.find_first_of('@');
			uid_part = temp_uid.substr(0, index);
			std::string namespace_part = temp_uid.substr(index + 1); // @ should never be at the end
//			std::cout << uid_part << " " << namespace_part << std::endl; // DEBUG
			if (!namespace_part.compare(current_namespace)) {
				// match_all
				if (uids.size() == 1 && !uids[0].compare("all")) {
					first_found = true;
					break;
				} else { // check that the uid is in the uids list
					for (unsigned k = 0; k < uids.size(); k++) {
						if (uid_part.compare(uids[k]) == 0) {
							first_found = true;
							break;
						}
					}
				}
			}
			i++;
		}
		if (!first_found) {
			std::cout << "FYI: matched UIDs had wrong namespace" << std::endl;
			return -1; // all matched UIDs have wrong namespace
		}

		std::string best_uid(uid_part);
		int best_confidence = uids_response[i]["confidence"].asInt();
//		std::cout << best_confidence << " " << best_uid << std::endl; // DEBUG
		for (unsigned j = i; j < matched_uids; j++) {
			std::string temp_uid(uids_response[j]["uid"].asString());
			unsigned index = temp_uid.find_first_of('@');
			uid_part = temp_uid.substr(0, index);
			std::string namespace_part = temp_uid.substr(index + 1);
//			std::cout << uid_part << " " << namespace_part << std::endl; // DEBUG
			if (namespace_part.compare(current_namespace)) continue; // wrong namespace
			if (uids.size() != 1 || uids[0].compare("all")) {
				bool uid_found = false;
				for (unsigned k = 0; k < uids.size(); k++) {
					if (uid_part.compare(uids[k]) == 0) {
						uid_found = true;
						break;
					}
				}
				if (!uid_found) continue; // uid wasn't in the uids list
			}
//			std::cout << uids_response[i]["confidence"].asInt()
//			          << " "
//			          << uids_response[i]["uid"].asString()
//			          << std::endl; // DEBUG
			int another_confidence = uids_response[j]["confidence"].asInt();
			if (another_confidence < best_confidence) continue;
			best_confidence = another_confidence;
			best_uid.assign(uid_part);
		}
		response.assign(best_uid);
		// TODO: calculate damage based on distance
		// returned damage range: 0...MAXIMUM_DAMAGE
		std::cout << "FYI: hit and returning damage" << std::endl;
		return (int)(MAXIMUM_DAMAGE*(face_tag_size - distance_from_center)/(face_tag_size));
	} catch (errors_e e) {
		switch (e) {
		case IRH_ERROR_PHOTO_HAS_NO_FACES: return -1;
		default: throw e;
		}
	}
	return -1; // shouldn't be reachable
}

int ImageRecognitionHelper::match_all(std::string& response,
                                      std::string& jpeg_image) {
	std::vector<std::string> temp_uids;
	std::string temp_uid("all");
	temp_uids.push_back(temp_uid);
	return match(response, jpeg_image, temp_uids);
}

void ImageRecognitionHelper::faces_status() {
//	std::string post_data("api_key=" API_KEY "&api_secret=" API_SECRET);
	std::string post_data("api_key=" API_KEY "&api_secret=" API_SECRET "&uids=all@" NAMESPACE_NAME);
	std::string post_url(API_URL FACES_STATUS_URL);
	Json::Value decoded_response;
	post(post_data, post_url, decoded_response);
	std::cout << decoded_response << std::endl;
}
