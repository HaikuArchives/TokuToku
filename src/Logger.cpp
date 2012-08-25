/*
 * Copyright 2012 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak, sil2100@vexillium.org
 */

#include "Logger.h"

#include <Messenger>
#include <String>

#include <stdarg.h>

static Logger *
Logger::GetLoggerInstance(int level)
{
	if (!fSingleton)
		fSingleton = new Logger(level);

	return fSingleton;
}

void
Logger::Log(int level, char *fmt, ...)
{
	if (level > fCurrentLevel)
		return;

	BString log;
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(p, size, fmt, ap);
	va_end(ap); // TODO: fill log string

	if (fLogRely) {
		BMessage msg(LOG_MESSAGE);
		msg.AddString("text", log);
		fMessenger->SendMessage(&msg);
	}
}

void
Logger::SetLogLevel(int level)
{
	fCurrentLevel = level;
}

int
Logger::GetLogLevel()
{
	return fCurrentLevel;
}

void
Logger::SetMessenger(BMessenger *rely)
{
	// This messenger, if set, relies log messages to other places
	fLogRely = rely;
	// TODO: maybe adding a buffer that would be flushed here
}

// Private

Logger::Logger(int level)
	:
	fCurrentLevel(level),
	fLogRely(NULL)
{
}
