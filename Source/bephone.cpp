#include "data.h"
#include "bephone.h"
#include "dos_beos.h"
#include "windows.h"
#include "views.h"
#include "confloadsave.h"

Data* database=new Data;
bool Saved=true;
BPath BePhonePath;
BString SkinPath;
rgb_color color;
char COM[255];
	
int main(void)
{	
	BephoneApp *TheApp=new BephoneApp;
	TheApp->Run();
	
	//save Settings
	ConfSave();
	
	delete TheApp;
}

BephoneApp::BephoneApp():BApplication("application/x-vnd.Be-BephoneBook")
{	
	
	//Determine where to place the window(vary with the definition used)
	BScreen *MainScreen= new BScreen;
	BRect Screen=MainScreen->Frame(),Rect(0,0,450,460);
	Rect.OffsetBy((Screen.Width())/6,(Screen.Height()/6));
	delete MainScreen;
	
	//Get the current BePhoneBook Path
	BPath path;
	BEntry entry;
	app_info TheInfo;
	GetAppInfo(&TheInfo);
	entry.SetTo(&(TheInfo.ref));
	entry.GetPath(&path); 
	path.GetParent(&path);
	BePhonePath=path;

	//load Settings
	ConfLoad();	

	Window = new MainWindow(Rect);
	Window->Show();	
}

void BephoneApp::RefsReceived(BMessage *msg)
{
	BPath path;
	BEntry entry;
	entry_ref ref;
	char *mimetype=new char[255];

	//Get reference of the file
	msg->FindRef("refs",&ref);
	entry.SetTo(&ref);
	entry.GetPath(&path); 
	
	//Get mime type of the File
	BNodeInfo Node;	
	BNode File;
	
	File.SetTo(path.Path());
	Node.SetTo(&File);
	Node.GetType(mimetype);
	
	//Is it a BephoneBook File?
	if(strcmp(mimetype,"text/BePhoneBook")!=0 || !database->Load((char*)path.Path()))
			(new BAlert("","The file you tried to Load is not\na valid BePhoneBook File :(((","Ok"))->Go();
	else
	{
		Saved=true;
		Window->PostMessage('refr');
	}
	
	delete mimetype;
}

void BephoneApp::MessageReceived(BMessage *msg)
{
	
	const char *name;
	BPath path;
	BEntry entry;
	entry_ref ref;
	char *Temp=new char[255];	
	
	BNodeInfo Node;	
	BNode File;
		
	switch(msg->what)
	{
		case B_SAVE_REQUESTED :
		//Get reference of the file
		msg->FindRef("directory",&ref);	
		msg->FindString("name",&name);
		
		entry.SetTo(&ref);
		entry.GetPath(&path);
		path.Append(name);
		
		//Save
		database->Save((char*)path.Path());
		Saved=true;
		
		//Add icon to the file (depend of the skin)
		File.SetTo(path.Path());		
		Node.SetTo(&File);	
		Node.SetType("text/BePhoneBook");

		BBitmap *Bicon,*Sicon;
	
		strcpy(Temp,SkinPath.String());
		strcat(Temp,"/Bicon");	
	
		Bicon = new BBitmap(BRect (0.0, 0.0, 31.0, 31.0), B_CMAP8);
		Bicon=load_flat_bitmap(Temp);		
		
		strcpy(Temp,SkinPath.String());
		strcat(Temp,"/Sicon");
			
		Sicon = new BBitmap(BRect (0.0, 0.0, 15.0, 15.0), B_CMAP8);
		Sicon=load_flat_bitmap(Temp);
		
		Node.SetIcon(Bicon,B_LARGE_ICON);
		Node.SetIcon(Sicon,B_MINI_ICON); 
		
		delete Sicon;
		delete Bicon;		
		break;
		
		case 'refr':
		Window->PostMessage('refr');
		break;
		
		case 'load':
		//Get reference of the file
		msg->FindRef("refs",&ref);
	
		entry.SetTo(&ref);
		entry.GetPath(&path);
	
		File.SetTo(path.Path());
		Node.SetTo(&File);
		Node.GetType(Temp);
		 
		//Is it a BephoneBook File
		if(strcmp(Temp,"text/BePhoneBook")!=0 || !database->Load((char*)path.Path()))
			(new BAlert("","The file you tried to Load is not\na valid BePhoneBook File :(((","Ok"))->Go();
		else
		{
			Saved=true;
			Window->PostMessage('refr');
		}
		break;
		
		case 'impo':
		//Get reference of the file
		msg->FindRef("refs",&ref);
		
		entry.SetTo(&ref);
		entry.GetPath(&path); 
		
		strcpy(Temp,BePhonePath.Path());
		strcat(Temp,"/importsed.pbk");
		
		//Convert the file from Dos text to Unix/Beos type
		imports((char*)path.Path(),Temp);
			
		//Is it a BephoneBook file (after conversion ;) )	
		if(!database->Load(Temp))
			(new BAlert("","The file you tried to imports is not\na valid BePhoneBook File :(((","Ok"))->Go();
		else	
		{
			Saved=false;
			Window->PostMessage('refr');
		}
		break;
		
		case 'expo':
		//Get reference of the file
		msg->FindRef("directory",&ref);	
		msg->FindString("name",&name);
		
		entry.SetTo(&ref);
		entry.GetPath(&path);
		path.Append(name);
		
		strcpy(Temp,BePhonePath.Path());
		strcat(Temp,"/exports.pbk");
		
		//Save the file and convert it to Dos text type
		database->Save(Temp);
		exports(Temp,(char*)path.Path());		
		break;
		
		case 'indx':
		Window->PostMessage(msg);
		break;		

		default : 
		BApplication::MessageReceived(msg);
		break;	
	}
	
	delete Temp;
}