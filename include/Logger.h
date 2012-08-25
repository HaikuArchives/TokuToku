/*
 * Copyright 2012 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#define DEFAULT_LOG_LEVEL 5

class BMessenger;

class Logger {
public:
	static Logger *GetLoggerInstance(int level = DEFAULT_LOG_LEVEL);

	//operator() (
	void Log(int level, char *fmt, ...);
	void SetLogLevel(int level);
	int GetLogLevel();

	void SetMessenger(BMessenger *rely);

private:
	Logger(int level = DEFAULT_LOG_LEVEL);

	int fCurrentLevel;
	BMessenger *fLogRely;

	static Logger fSingleton = NULL;
};

#endif
