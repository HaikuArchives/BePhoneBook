#ifndef _BEPHONE_H_
#define _BEPHONE_H_

#include <Application.h>
#include <string.h>
#include <Path.h>
#include <Entry.h>
#include "data.h"
#include "windows.h"
#include <NodeInfo.h>
#include <Screen.h>
#include <iostream>
#include <Roster.h>

class BephoneApp : public BApplication
{
	public:
	BephoneApp();
	virtual void MessageReceived(BMessage *);
	virtual void RefsReceived(BMessage *);
	
	MainWindow *Window;
};

extern Data *database;
extern BPath BePhonePath;
extern BString SkinPath,LastFileLoaded;
extern bool Saved;
extern rgb_color color;
extern char COM[255];
#endif
