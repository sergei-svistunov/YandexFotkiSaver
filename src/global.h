/*
 * global.h
 *
 *  Created on: 14.11.2008
 *      Author: svistunov
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/time.h>
#include "string.h"

#include <GL/glxew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define DLOG(t) syslog(LOG_INFO, t);

#endif /* GLOBAL_H_ */
