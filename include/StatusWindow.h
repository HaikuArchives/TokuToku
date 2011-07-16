/*
 * Copyright 2011 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */

#ifndef __BEGADU_STATUSWINDOW_H__
#define __BEGADU_STATUSWINDOW_H__

#include <Window.h>

class BButton;
class BTextView;
class BStringView;

class StatusWindow : public BWindow {
public:
	StatusWindow();

private:
	BButton *iRefreshButton;
	BTextView *iDescrView;
	BStringView *iNameString;
	BStringView *iSurnameString;
	BStringView *iAliasString;
	BStringView *iUINString;
};

#endif
