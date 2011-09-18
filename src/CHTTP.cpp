/*
 * CHTTP.cpp
 *
 *  Created on: 20.09.2009
 *      Author: svistunov
 */

#include "CHTTP.h"

CHTTP::CHTTP() {
	curl_global_init(CURL_GLOBAL_WIN32);
	_curl = curl_easy_init();
	curl_easy_setopt(_curl, CURLOPT_USERAGENT, "Yandex Fotki Screen Saver (Gnome)");
}

CHTTP::~CHTTP() {
	curl_easy_cleanup(_curl);
}

bool CHTTP::Get(const char * URL, const char * FileName) {
	CURLcode CURLRes = CURLE_WRITE_ERROR;

	FILE * Stream;
	Stream = fopen(FileName, "wb");
	if (!Stream)
		return false;

	char * proxy = getenv("http_proxy");
	if (proxy && strlen(proxy)) {
		curl_easy_setopt(_curl, CURLOPT_PROXY, proxy);
		if (CURLRes != CURLE_OK)
			return false;
	};

	CURLRes = curl_easy_setopt(_curl, CURLOPT_WRITEDATA, Stream);
	if (CURLRes != CURLE_OK)
		return false;

	CURLRes = curl_easy_setopt(_curl, CURLOPT_URL, URL);
	if (CURLRes != CURLE_OK)
		return false;

	CURLRes = curl_easy_perform(_curl);

	fclose(Stream);

	return CURLRes == CURLE_OK;
}
