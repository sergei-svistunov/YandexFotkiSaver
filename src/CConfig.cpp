/*
 * CConfig.cpp
 *
 *  Created on: 05.01.2010
 *      Author: svistunov
 */

#include "CConfig.h"
#include "regex.h"

CConfig::CConfig() {

}

CConfig::~CConfig() {

}

const char * CConfig::_Get(const char * Name) {
	return _Values[Name];
}

void CConfig::Load(const char * FileName) {
	FILE * Stream = fopen(FileName, "r");
	if (Stream == NULL)
		return;

	regex_t RegExp;
	regmatch_t RegMatch[3];
	if (regcomp(&RegExp, "([a-zA-Z0-9_-]+)\\s*:\\s*([a-zA-Z0-9\.]+)", REG_EXTENDED) != 0) {
		DLOG("Cannot compile config RegExp");
		return;
	};

	char * FLine = new char[1000];
	while (!feof(Stream)) {
		if (fgets(FLine, 1000, Stream) == NULL)
			continue;

		if (regexec(&RegExp, FLine, 3, RegMatch, 0) == 0) {
			size_t szKey   = RegMatch[1].rm_eo - RegMatch[1].rm_so;
			size_t szValue = RegMatch[2].rm_eo - RegMatch[2].rm_so;
			char * Key   = new char[szKey + 1];
			char * Value = new char[szValue + 1];
			Key[szKey]     = 0;
			Value[szValue] = 0;

			strncpy(Key,   FLine + RegMatch[1].rm_so, szKey);
			strncpy(Value, FLine + RegMatch[2].rm_so, szValue);

			_Values[Key] = Value;
		};
	};

	regfree(&RegExp);
	delete[] FLine;

	fclose(Stream);
}

const char * CConfig::Get(const char * Name, const char * DefaultValue) {
	const char * Res = _Get(Name);

	if (Res != NULL)
		return Res;

	return DefaultValue;
}

int CConfig::GetInt(const char * Name, int DefaultValue) {
	const char * Res = _Get(Name);

	if (Res != NULL) {
		int IRes;
		if (sscanf(Res, "%i", &IRes))
			return IRes;
	};

	return DefaultValue;
}

float CConfig::GetFloat(const char * Name, float DefaultValue) {
	const char * Res = _Get(Name);

	if (Res != NULL) {
		float FRes;
		if (sscanf(Res, "%f", &FRes))
			return FRes;
	};

	return DefaultValue;
}
