#ifndef _WINDOWS_H_
#define _WINDOWS_H_

#include <be/kernel/fs_attr.h>
#include <Node.h>
#include <Window.h>
#include <FilePanel.h>
#include "views.h"
#include "TextView.h"
#include "CheckBox.h"
#include "StringView.h"
#include "String.h"
#include "be/app/Roster.h"

class NoteWindow;
class EditWindow;
class AddWindow;
class AboutWindow;


class MainWindow:public BWindow
{
	public :
	MainWindow(BRect);
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage*);
	
	private :	
	void AddPerson(BNode);
	MainView *View;
	NoteWindow *NoteW;
	EditWindow *EditW;
	AddWindow *AddW;
	AboutWindow *AbW;
};

class NoteWindow:public BWindow
{
	public :
	NoteWindow(char *,BRect,char*);
		
	private:
	BTextView *View;
};

class AddWindow:public BWindow
{
	public :
	AddWindow(BRect);
	virtual void MessageReceived(BMessage*);
	
	private:
	AddView *View;
};

class EditWindow:public BWindow
{
	public :
	EditWindow(int,BRect);
	virtual void MessageReceived(BMessage*);
	int index;
		
	private:
	EditView *View;
};

class AboutWindow:public BWindow
{
	public :
	AboutWindow(BRect);
	int index;
		
	private:
	AboutView *View;
};

#endif