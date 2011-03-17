import httplib
import json
import urllib
import sys

CRLF                  = '\r\n'
BOUNDARY_STRING       = 'nonRelevantString'
BOUNDARY              = '--' + BOUNDARY_STRING + CRLF
BOUNDARY_END          = '--' + BOUNDARY_STRING + '--' + CRLF

API_DOMAIN_NAME       = 'api.face.com'
API_KEY               = '5f7a9cbd46993f8672eab2f8e2eb516b'
API_SECRET            = 'a88ae67f870e64c3e7abebee10585552'
NAMESPACE             = 'T1105130'
TEST_UID              = 'spede' + '@' + NAMESPACE

POST_HEADERS          = {'Content-type': 'application/x-www-form-urlencoded'}
MULTIPART_HEADERS     = {'Content-Type' : 'multipart/form-data; boundary=' + BOUNDARY_STRING}


# faces.recognize
FACES_RECOGNIZE_URL = '/faces/recognize.json'

FACES_RECOGNIZE_DATA  = ''

FACES_RECOGNIZE_DATA += BOUNDARY
FACES_RECOGNIZE_DATA += 'Content-Disposition: form-data; name="api_key"'
FACES_RECOGNIZE_DATA += CRLF
FACES_RECOGNIZE_DATA += CRLF
FACES_RECOGNIZE_DATA += API_KEY
FACES_RECOGNIZE_DATA += CRLF

FACES_RECOGNIZE_DATA += BOUNDARY
FACES_RECOGNIZE_DATA += 'Content-Disposition: form-data; name="api_secret"'
FACES_RECOGNIZE_DATA += CRLF
FACES_RECOGNIZE_DATA += CRLF
FACES_RECOGNIZE_DATA += API_SECRET
FACES_RECOGNIZE_DATA += CRLF

FACES_RECOGNIZE_DATA += BOUNDARY
FACES_RECOGNIZE_DATA += 'Content-Disposition: form-data; name="uids"'
FACES_RECOGNIZE_DATA += CRLF
FACES_RECOGNIZE_DATA += CRLF
FACES_RECOGNIZE_DATA += TEST_UID
FACES_RECOGNIZE_DATA += CRLF

FACES_RECOGNIZE_DATA += BOUNDARY
FACES_RECOGNIZE_DATA += 'Content-Disposition: form-data; filename="file"'
FACES_RECOGNIZE_DATA += CRLF
FACES_RECOGNIZE_DATA += 'Content-Type: image/jpeg'
FACES_RECOGNIZE_DATA += CRLF
FACES_RECOGNIZE_DATA += CRLF
f = open('diagonal.jpg', 'rb')
FACES_RECOGNIZE_DATA += f.read()
f.close()
FACES_RECOGNIZE_DATA += CRLF
FACES_RECOGNIZE_DATA += BOUNDARY_END

def recognize():
	connection = httplib.HTTPConnection(API_DOMAIN_NAME)
	connection.connect()
	connection.request('POST', FACES_RECOGNIZE_URL, FACES_RECOGNIZE_DATA, MULTIPART_HEADERS)
	response = connection.getresponse()
	response_data_string = response.read()
	response_data = json.loads(response_data_string)
	print json.dumps(response_data, sort_keys=True, indent=4)
	connection.close()


# faces.train
FACES_TRAIN_URL    = '/faces/train.json'
FACES_TRAIN_PARAMS = {'api_key' : API_KEY, 'api_secret' : API_SECRET, 'uids' : TEST_UID}
def train():
	connection = httplib.HTTPConnection(API_DOMAIN_NAME) # creates object, doesn't actually connect yet
	connection.connect() # opens tcp connection
	url_params = urllib.urlencode(FACES_TRAIN_PARAMS)
	connection.request('POST', FACES_TRAIN_URL, url_params, POST_HEADERS)
	response = connection.getresponse()
	response_data_string = response.read()
	response_data = json.loads(response_data_string) # put the response string into a data structure
	print json.dumps(response_data, sort_keys=True, indent=4)
	connection.close() # closes tcp connection


# faces.status
#FACES_STATUS_URL    = '/account/limits.format'
FACES_STATUS_URL    = '/account/limits.json'
FACES_STATUS_PARAMS = {'api_key' : API_KEY, 'api_secret' : API_SECRET}
def status():
	connection = httplib.HTTPConnection(API_DOMAIN_NAME) # creates object, doesn't actually connect yet
	connection.connect() # opens tcp connection
	url_params = urllib.urlencode(ACCOUNT_LIMITS_PARAMS)
	connection.request('POST', ACCOUNT_LIMITS_URL, url_params, POST_HEADERS)
	response = connection.getresponse()
	response_data_string = response.read()
	response_data = json.loads(response_data_string) # put the response string into a data structure
	print json.dumps(response_data, sort_keys=True, indent=4)
	connection.close() # closes tcp connection


# tags.get irrelevant
# tags.add irrelevant
# tags.remove

# account.limits
#ACCOUNT_LIMITS_URL    = '/account/limits.format'
#ACCOUNT_LIMITS_URL    = '/account/limits'
ACCOUNT_LIMITS_URL    = '/account/limits.json'
ACCOUNT_LIMITS_PARAMS = {'api_key' : API_KEY, 'api_secret' : API_SECRET}
def limits():
	connection = httplib.HTTPConnection(API_DOMAIN_NAME) # creates object, doesn't actually connect yet
	connection.connect() # opens tcp connection
	url_params = urllib.urlencode(ACCOUNT_LIMITS_PARAMS)
	connection.request('POST', ACCOUNT_LIMITS_URL, url_params, POST_HEADERS)
	response = connection.getresponse()
	response_data_string = response.read()
	response_data = json.loads(response_data_string) # put the response string into a data structure
	print json.dumps(response_data, sort_keys=True, indent=4)
	connection.close() # closes tcp connection

# account.users
'''
Required api_key      your face.com API Key
         api_secret   your face.com API Secret
         namespaces   comma separated list of one or more private namespaces
'''
#ACCOUNT_USERS_URL     = '/account/users.format'
#ACCOUNT_USERS_URL     = '/account/users'
ACCOUNT_USERS_URL     = '/account/users.json'
ACCOUNT_USERS_PARAMS  = {'api_key' : API_KEY, 'api_secret' : API_SECRET, 'namespaces' : NAMESPACE}

def users():
	connection = httplib.HTTPConnection(API_DOMAIN_NAME)
	connection.connect()
	url_params = urllib.urlencode(ACCOUNT_USERS_PARAMS)
	connection.request('POST', ACCOUNT_USERS_URL, url_params, POST_HEADERS)
	response = connection.getresponse()
	response_data_string = response.read()
	response_data = json.loads(response_data_string)
	print json.dumps(response_data, sort_keys=True, indent=4)
	connection.close()


# faces.detect
#FACES_DETECT_URL    = '/faces/detect.format'
FACES_DETECT_URL    = '/faces/detect.json'
#FACES_DETECT_URL     = '/faces/detect'

FACES_DETECT_DATA  = ''

FACES_DETECT_DATA += BOUNDARY
FACES_DETECT_DATA += 'Content-Disposition: form-data; name="api_key"'
FACES_DETECT_DATA += CRLF
FACES_DETECT_DATA += CRLF
FACES_DETECT_DATA += API_KEY
FACES_DETECT_DATA += CRLF

FACES_DETECT_DATA += BOUNDARY
FACES_DETECT_DATA += 'Content-Disposition: form-data; name="api_secret"'
FACES_DETECT_DATA += CRLF
FACES_DETECT_DATA += CRLF
FACES_DETECT_DATA += API_SECRET
FACES_DETECT_DATA += CRLF

FACES_DETECT_DATA += BOUNDARY
FACES_DETECT_DATA += 'Content-Disposition: form-data; filename="file"'
FACES_DETECT_DATA += CRLF
FACES_DETECT_DATA += 'Content-Type: image/jpeg'
FACES_DETECT_DATA += CRLF
FACES_DETECT_DATA += CRLF
f = open('front.jpg', 'rb')
FACES_DETECT_DATA += f.read()
f.close()
FACES_DETECT_DATA += CRLF
FACES_DETECT_DATA += BOUNDARY_END

def detect(): # should return TID
	connection = httplib.HTTPConnection(API_DOMAIN_NAME)
	connection.connect()
	connection.request('POST', FACES_DETECT_URL, FACES_DETECT_DATA, MULTIPART_HEADERS)
	response = connection.getresponse()
	response_data_string = response.read()
	response_data = json.loads(response_data_string)
	print json.dumps(response_data, sort_keys=True, indent=4)
	connection.close()

def namespaces():
	connection = httplib.HTTPConnection(API_DOMAIN_NAME)
	connection.connect()
	url_params = urllib.urlencode(ACCOUNT_LIMITS_PARAMS)
	connection.request('POST', '/account/namespaces.json', url_params, POST_HEADERS)
	response = connection.getresponse()
	response_data_string = response.read()
	response_data = json.loads(response_data_string)
	print json.dumps(response_data, sort_keys=True, indent=4)
	connection.close()

# tags.save
#TAGS_SAVE_URL = '/tags/save.format'
#TAGS_SAVE_URL = '/tags/save'
TAGS_SAVE_URL  = '/tags/save.json'
def save():
	connection = httplib.HTTPConnection(API_DOMAIN_NAME)
	connection.connect()
	# detect
	connection.request('POST', FACES_DETECT_URL, FACES_DETECT_DATA, MULTIPART_HEADERS)
	response = connection.getresponse()
	response_data_string = response.read()
	response_data = json.loads(response_data_string)
	#print json.dumps(response_data, sort_keys=True, indent=4)
	#end detect
	tid = response_data['photos'][0]['tags'][0]['tid'] # zeros = assume there is only one photo and one tag
	print tid
	TAGS_SAVE_PARAMS  = {'api_key' : API_KEY, 'api_secret' : API_SECRET, 'tids' : tid, 'uid' : TEST_UID}
	url_params = urllib.urlencode(TAGS_SAVE_PARAMS)
	connection.request('POST', TAGS_SAVE_URL, url_params, POST_HEADERS)
	response = connection.getresponse()
	response_data_string = response.read()
	response_data = json.loads(response_data_string)
	print json.dumps(response_data, sort_keys=True, indent=4)
	connection.close()

#print sys.argv
#detect()
#limits()
#users()
#namespaces()
#save()
recognize()
#train()