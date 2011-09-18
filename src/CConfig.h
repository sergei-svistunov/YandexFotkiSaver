/*
 * CConfig.h
 *
 *  Created on: 05.01.2010
 *      Author: svistunov
 */

#ifndef CCONFIG_H_
#define CCONFIG_H_

#include "global.h"

#include "map"

struct LtStr {
	bool operator()(const char* s1, const char* s2) const {
		return strcmp(s1, s2) < 0;
	};
};

class CConfig {
public:
	CConfig();
	virtual ~CConfig();
	void Load(const char * FileName);
	const char * Get(const char * Name, const char * DefaultValue);
	int GetInt(const char * Name, int DefaultValue);
	float GetFloat(const char * Name, float DefaultValue);
private:
	const char * _Get(const char * Name);
	std::map<const char*, char*, LtStr> _Values;
};

#endif /* CCONFIG_H_ */
