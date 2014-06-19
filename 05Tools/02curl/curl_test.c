#include <curl/curl.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *dest)
{
	memcpy(dest, ptr, strlen((char*)ptr));
	return strlen((char*)ptr); 
}

// --------------------------------------------- POST --------------------------------------------------------
char sig_buf[10240];
snprintf(sig_buf, 10240, "%dsid=%s%s", .....); 

string sign = md5sum(sig_buf, strlen(sig_buf)); 

char request[10240];
snprintf(request, 10240, 
	"{\"id\":1403055383942,\"service\":\"ucid.user.sidInfo\", \"data\":{\"sid\":\"%s\"},\"game\":{\"cpId\":%d,\"gameId\":%d,\"channelId\":\"2\",\"serverId\":3154},\"sign\":\"%s\"}", ...); 	

CURL *m_curl = NULL;  
m_curl = ::curl_easy_init();    
if (NULL == m_curl){ 
    return -1;  
}  

char http_recv[10240];
memset(http_recv, 0, 10240);

::curl_easy_setopt(m_curl, CURLOPT_URL, url);  
::curl_easy_setopt(m_curl, CURLOPT_POST, 1);  
::curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_data);  
::curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, http_recv);  
::curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, request);  
::curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, strlen(request));  

curl_slist *http_headers = NULL; 
http_headers = curl_slist_append(http_headers, "Content-Type: application/json");
::curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, http_headers);

CURLcode ret = curl_easy_perform(m_curl);   

::curl_slist_free_all(http_headers); 
::curl_easy_cleanup(m_curl); 
::curl_global_cleanup();

ptree pt; // http return json  boost ptree
bool parse;
try{
	std::stringstream ss(http_recv);
	boost::property_tree::read_json(ss, pt);
	VLOG(L_DT) << "json parse  success";
	parse = true;
}
catch(...){
	VLOG(L_DT) << "json parse  failed";
	parse = false;
}

if(!parse){
	.....
}

int id		= pt.get<int>("id",			 -10086);
ptree state = pt.get_child("state");
ptree data  = pt.get_child("data");
code		= state.get<int>("code",	 -10086);
string msg	= state.get<string>("msg",	 "amd");
if(UC_SUCCESS == code){
	ucid	  = data.get<string>("ucid",		"amd");
	nickName  = data.get<string>("nickName",	"amd");
}

// -------------------------------------------------- GET -------------------------------------------------------------------------

char sig_buf[10240];
snprintf(sig_buf, 10240, "appId=%d&session=%s&uid=%s", .....); 

string signature = cryptlite::hmac<cryptlite::sha1>::calc_hex(sig_buf, key); // hmac_sha1 algo 

char http_buf[10240];
snprintf(http_buf, 10240, "%s?appId=%d&session=%s&uid=%s&signature=%s", ....);

VLOG(L_DT) << "http : " << http_buf;

CURL *curl = curl_easy_init(); // use curl lib for http
CURLcode res;

char recv_buf[10240];
memset(recv_buf, 0, 10240);
curl_easy_setopt(curl, CURLOPT_URL, http_buf);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buf);  // set recv buf
res = curl_easy_perform(curl);
curl_easy_cleanup(curl);

ptree pt; // http return json  boost json
bool parse;
try{
	std::stringstream ss(recv_buf);
	boost::property_tree::read_json(ss, pt);
	parse = true;
}
catch(...){
	parse = false;
}

if(!parse){
	return -1;
}

int errcode = pt.get<int>("errcode");