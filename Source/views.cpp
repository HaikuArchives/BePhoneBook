//#include "global.h"
#include "data.h"
#include "bephone.h"
#include "dos_beos.h"
#include "windows.h"
#include "views.h"
#include "MenuItem.h"
#include "Button.h"
#include "ListView.h"
#include "IconListItem.h"

AboutView::AboutView(BRect Rect):BView(Rect,"AboutView",B_FOLLOW_ALL, B_WILL_DRAW)
{
	BString filename(BePhonePath.Path());
	filename+="/about.jpg";
	Bitmap=BTranslationUtils::GetBitmap(filename.String());
}

void AboutView::Draw(BRect Rect)
{
	DrawBitmap(Bitmap);
}


MainView::MainView(BRect Rect):BView(Rect,"MainView",B_FOLLOW_ALL, B_WILL_DRAW)
{
	//Menu creation
	BRect menuBarRect;
	BMenuBar *menuBar;
			
	menuBarRect=Bounds();
	menuBarRect.bottom = 18.0;
	
	menuBar= new BMenuBar(menuBarRect,"MenuBar");
	AddChild(menuBar);
	
	BMenu *File=new BMenu("File");
	menuBar->AddItem(File);
	
	File->AddItem(new BMenuItem("New",new BMessage('new'),'N'));
	File->AddItem(new BMenuItem("Load ...",new BMessage('load'),'L'));
	File->AddItem(new BMenuItem("Save",new BMessage('save'),'S'));
	File->AddItem(new BMenuItem("Save As ...",new BMessage('sava'),'A'));
	File->AddItem(new BMenuItem("Import People Files",new BMessage('impe'),'P'));
	File->AddItem(new BMenuItem("Import ...",new BMessage('impo'),'I'));
	File->AddItem(new BMenuItem("Export ...",new BMessage('expo'),'E'));

	//skin menu (hope it works ....)
	BMenu *Skins=new BMenu("Skins");
	Skins->SetRadioMode(true);

	//Get the Skin names and put them in BMenu *Skin
	BString Path;
	BPath DirName;
	Path=BePhonePath.Path();
	Path+="/Skins/";
	menuBar->AddItem(Skins);
	BDirectory dir(Path.String()); 
	BEntry entry;
	dir.Rewind(); 
	BMessage *msg;
	BMenuItem *MenuItem;

	while (dir.GetNextEntry(&entry) == B_OK )
	{
		msg= new BMessage('skin');
		entry.GetPath(&DirName);
		msg->AddString("SkinPath",DirName.Path());
		Skins->AddItem(MenuItem=new BMenuItem(DirName.Leaf(),msg));
		
		if (strcmp(DirName.Path(),SkinPath.String())==0)
			MenuItem->SetMarked(true);
		
	}
	
	BMenu *Dial=new BMenu("Dial");
	Dial->SetRadioMode(true);
	menuBar->AddItem(Dial);
	
	char *test;
	test=COM;
	
	BSerialPort serial;
	char devName[B_OS_NAME_LENGTH];
	
	for (int n=serial.CountDevices()-1;n >= 0;n--)
	{
		serial.GetDeviceName(n,devName);
		msg= new BMessage('com');
		msg->AddString("com",devName);
		Dial->AddItem(MenuItem=new BMenuItem(devName,msg));
		
		if (strcmp(devName,COM)==0)
			MenuItem->SetMarked(true);
	}	
		
	BMenu *Ab=new BMenu("?");
	menuBar->AddItem(Ab);
	Ab->AddItem(new BMenuItem("About",new BMessage('abut'),'B'));
	Ab->AddItem(new BMenuItem("Help",new BMessage('help'),'H'));


	//List ColomnView :)

	CLVContainerView* ContainerView;
	List = new MyColumnListView(&ContainerView);
	List->AddColumn(new CLVColumn(NULL,20.0,CLV_LOCK_AT_BEGINNING|CLV_NOT_MOVABLE|CLV_NOT_RESIZABLE|CLV_PUSH_PASS|CLV_MERGE_WITH_RIGHT));
	List->AddColumn(new CLVColumn("Name",220,CLV_NOT_MOVABLE|CLV_NOT_RESIZABLE));
	List->AddColumn(new CLVColumn("Location",60,CLV_NOT_MOVABLE|CLV_NOT_RESIZABLE));
	List->AddColumn(new CLVColumn("Phone Number",210.0,CLV_NOT_MOVABLE|CLV_NOT_RESIZABLE));
	List->SetSortFunction(Item::MyCompare);
	AddChild(ContainerView);
	
	FirstTime=true;
}

void MainView::SkinBPK()
{
	BRect _rect(0.0, 0.0, 15.0, 15.0); 
	char Temp[255];
	BBitmap *icon1 = new BBitmap(_rect, B_CMAP8);
	BBitmap *icon2 = new BBitmap(_rect, B_CMAP8);
	BPicture *ButtOn,*ButtOff;	
    FILE *fichier;

	//If it s the first time 
	if (FirstTime==false)
	{
		RemoveChild(Edit);
		RemoveChild(Add);
		RemoveChild(Delete);
		RemoveChild(Note);
		RemoveChild(Dial);
	}	
	
	FirstTime=false;
	
	//color of background
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/Color");
    fichier=fopen(Temp, "r");
	if(fichier==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}
	fscanf(fichier,"%d %d %d",&color.red,&color.green,&color.blue);
	color.alpha=255;
	fclose(fichier);

	SetViewColor(color); 
	SetHighColor(color);	

	//Picture Buttons
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/AddB");

	BeginPicture(new BPicture);
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(0,0);
	icon1=load_flat_bitmap(Temp);
	if(icon1==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}
	
	SetFont(be_bold_font);
	DrawBitmap(icon1);
	SetHighColor(0,0,0,200);
	MovePenTo(5,45);
	DrawString("Add");
	SetHighColor(color);
	ButtOn=EndPicture();

	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/AddB2");	

	BeginPicture(new BPicture);	
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(0,0);
	icon2=load_flat_bitmap(Temp);
	if(icon2==NULL)
		icon2=icon1;
	SetFont(be_bold_font);
	DrawBitmap(icon2);
	SetHighColor(0,0,0,200);
	MovePenTo(5,45);
	DrawString("Add");
	SetHighColor(color);
	ButtOff=EndPicture();

	BRect Rect(0,0,50,50);
	
	Rect.OffsetBy(20,400);
	Add=new BPictureButton(Rect,"Add",ButtOn,ButtOff,new BMessage('Add'));
	AddChild(Add);
	
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/EditB");

	delete ButtOn;
	BeginPicture(new BPicture);	
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(0,0);
	icon1=load_flat_bitmap(Temp);
	if(icon1==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}
	SetFont(be_bold_font);
	DrawBitmap(icon1);
	SetHighColor(0,0,0,200);
	MovePenTo(7,45);
	DrawString("Edit");
	SetHighColor(color);
	ButtOn=EndPicture();
	
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/EditB2");	
	
	delete ButtOff;
	
	BeginPicture(new BPicture);	
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(0,0);
	icon2=load_flat_bitmap(Temp);
	if(icon2==NULL)
		icon2=icon1;
	SetFont(be_bold_font);
	DrawBitmap(icon2);
	SetHighColor(0,0,0,200);
	MovePenTo(7,45);
	DrawString("Edit");
	SetHighColor(color);
	ButtOff=EndPicture();

	Rect.OffsetBy(95,0);
	Edit=new BPictureButton(Rect,"Edit",ButtOn,ButtOff,new BMessage('Edit'));
	AddChild(Edit);
	
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/DeleteB");

	delete ButtOn;
	BeginPicture(new BPicture);	
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(5,0);
	icon1=load_flat_bitmap(Temp);
	if(icon1==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}
	SetFont(be_bold_font);
	DrawBitmap(icon1);
	SetHighColor(0,0,0,200);
	MovePenTo(3,45);
	DrawString("Delete");
	SetHighColor(color);
	ButtOn=EndPicture();
	
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/DeleteB2");	
	
	delete ButtOff;
	BeginPicture(new BPicture);	
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(5,0);
	icon2=load_flat_bitmap(Temp);
	if(icon2==NULL)
		icon2=icon1;
	SetFont(be_bold_font);
	DrawBitmap(icon2);
	SetHighColor(0,0,0,200);
	MovePenTo(3,45);
	DrawString("Delete");
	SetHighColor(color);
	ButtOff=EndPicture();
	
	Rect.OffsetBy(90,0);
	Delete=new BPictureButton(Rect,"Delete",ButtOn,ButtOff,new BMessage('Dele'));
	AddChild(Delete);

	delete ButtOn;
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/NoteB");
	
	BeginPicture(new BPicture);	
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(0,0);
	icon1=load_flat_bitmap(Temp);
	if(icon1==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}
	SetFont(be_bold_font);
	DrawBitmap(icon1);
	SetHighColor(0,0,0,200);
	MovePenTo(3,45);
	DrawString("Note");
	SetHighColor(color);
	ButtOn=EndPicture();
	
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/NoteB2");	
	
	delete ButtOff;
	BeginPicture(new BPicture);	
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(0,0);
	icon2=load_flat_bitmap(Temp);
	if(icon2==NULL)
		icon2=icon1;
	SetFont(be_bold_font);
	DrawBitmap(icon2);
	SetHighColor(0,0,0,200);
	MovePenTo(3,45);
	DrawString("Note");
	SetHighColor(color);
	ButtOff=EndPicture();

	Rect.OffsetBy(95,0);
	Note=new BPictureButton(Rect,"Note",ButtOn,ButtOff,new BMessage('Note'));
	AddChild(Note);
	
	delete ButtOn;
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/DialB");

	BeginPicture(new BPicture);	
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(0,0);
	icon1=load_flat_bitmap(Temp);
	if(icon1==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}
	SetFont(be_bold_font);
	DrawBitmap(icon1);
	SetHighColor(0,0,0,200);
	MovePenTo(7,45);
	DrawString("Dial");
	SetHighColor(color);
	ButtOn=EndPicture();
	
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/DialB2");	

	delete ButtOff;
	BeginPicture(new BPicture);	
	FillRect(BRect(0,0,60,60),B_SOLID_HIGH);
	SetDrawingMode(B_OP_OVER); 
	MovePenTo(0,0);
	icon2=load_flat_bitmap(Temp);
	if(icon2==NULL)
		icon2=icon1;
	SetFont(be_bold_font);
	DrawBitmap(icon2);
	SetHighColor(0,0,0,200);
	MovePenTo(7,45);
	DrawString("Dial");
	SetHighColor(color);
	ButtOff=EndPicture();

	Rect.OffsetBy(95,0);
	Dial=new BPictureButton(Rect,"Dial",ButtOn,ButtOff,new BMessage('Dial'));
	AddChild(Dial);

	delete ButtOn;
	delete ButtOff;

	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/Note");
	
	icon1=load_flat_bitmap(Temp);
	if(icon1==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}

	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/NoNote");
	
	icon2=load_flat_bitmap(Temp);
	if(icon2==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}
	
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/Bicon");	
	icon1=load_flat_bitmap(Temp);
	if (icon1==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}

	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/Sicon");	
	icon2=load_flat_bitmap(Temp);
	if (icon2==NULL)
	{
		SkinPath=BePhonePath.Path();
		SkinPath+="/Skins/Default";
		SkinBPK();
		(new BAlert("","The Skin you tried to Load is not\na valid Skin returning to Default Skin :(((","Ok"))->Go();
		return;		
	}	
	
	delete icon1;
	delete icon2;
	
	Refresh();
	Invalidate();
}

void MainView::Refresh()
{
	//Empty the list
	List->MakeEmpty();

	BRect _rect(0.0, 0.0, 15.0, 15.0); 
	BBitmap *icon1 = new BBitmap(_rect, B_CMAP8);
	BBitmap *icon2 = new BBitmap(_rect, B_CMAP8);
	
	char *Temp=new char[255];
	
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/Note");
	
	icon1=load_flat_bitmap(Temp);
	
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/NoNote");
	
	icon2=load_flat_bitmap(Temp);
	
	for(int i=0;i<database->Get_Nb_Pers();i++)
		List->AddItem(new Item(0,false,false,(strlen(database->Db[i]->Note)==0)?icon2:icon1,
			database->Db[i]->Name,(database->Db[i]->Location=='h')?(char*)"HOME":(char*)"WORK",
				database->Db[i]->Tel_Number,"test"));
		
	if(database->Get_Nb_Pers()!=0)
		List->Select(0);
		
	List->MakeFocus(true);
	
	delete icon1;
	delete icon2;
	delete Temp;
}

AddView::AddView(BRect Rect):BView(Rect,"MainView",B_FOLLOW_ALL, B_WILL_DRAW)
{
	Rect.Set(0,0,280,10);
	
	//color of background	
	SetViewColor(color); 
	

	//Text Controls
	Rect.OffsetTo(10,10);
	Name=new BTextControl(Rect,"Name","Name :","",NULL);
	Name->SetDivider(100);
	AddChild(Name);

	Rect.OffsetTo(10,40);	
	Phone=new BTextControl(Rect,"Telephone","Phone Number :","",NULL);
	Phone->SetDivider(100);
	AddChild(Phone);
	
	//Radio Buttons
	Rect.Set(0,0,60,10);
	Rect.OffsetTo(10,85);
	AddChild(new BStringView(Rect,"","Location :"));

	Rect.Set(0,0,280,10);
	Rect.OffsetTo(70,80);
	h=new BRadioButton(Rect,"Home","Home",new BMessage('Home'));
	h->SetValue(true);
	AddChild(h);
	
	Rect.OffsetTo(70,100);
	w=new BRadioButton(Rect,"Work","Work",new BMessage('Work'));
	AddChild(w);
	
	//Note
	Rect.OffsetTo(10,135);
	AddChild(new BStringView(Rect,"","Note :"));
	 
	Rect.Set(0,0,260,110);
	Rect.OffsetTo(12,150);
	Note= new BTextView(Rect,"Note",BRect(0,0,Rect.Width(),Rect.Height()),B_FOLLOW_NONE, B_WILL_DRAW | 	B_NAVIGABLE);
	AddChild(new BScrollView("scroll text",Note,B_FOLLOW_NONE,0,false,true,B_PLAIN_BORDER));
	
	Rect.Set(0,0,100,30);	

	//Buttons
	Rect.OffsetTo(10,300);
	BButton *Add=new BButton(Rect,"Add","Add",new BMessage('Add'));
	AddChild(Add);
	
	Rect.OffsetTo(190,300);
	BButton *Cancel=new BButton(Rect,"Cancel","Cancel",new BMessage('Canc'));
	AddChild(Cancel);
}

void AddView::Draw(BRect) 
{
	//StrokeRect(BRect(10,148,292,262));
}

EditView::EditView(BRect Rect,int index):BView(Rect,"MainView",B_FOLLOW_ALL, B_WILL_DRAW)
{
	Rect.Set(0,0,280,10);
	
	//color of background	
	SetViewColor(color); 
	
	//Text Controls
	Rect.OffsetTo(10,10);
	Name=new BTextControl(Rect,"Name","Name :","",NULL);
	Name->SetDivider(100);
	Name->SetText(database->Db[index]->Name);
	AddChild(Name);
	
	Rect.OffsetTo(10,40);	
	Phone=new BTextControl(Rect,"Telephone","Phone Number :","",NULL);
	Phone->SetDivider(100);
	Phone->SetText(database->Db[index]->Tel_Number);
	AddChild(Phone);
	
	//Radio Buttons
	Rect.Set(0,0,60,10);
	Rect.OffsetTo(10,85);
	AddChild(new BStringView(Rect,"","Location :"));

	Rect.Set(0,0,280,10);
	Rect.OffsetTo(70,80);
	h=new BRadioButton(Rect,"Home","Home",new BMessage('Home'));
	AddChild(h);
	
	Rect.OffsetTo(70,100);
	w=new BRadioButton(Rect,"Work","Work",new BMessage('Work'));
	AddChild(w);
	
	(database->Db[index]->Location)=='h'?h->SetValue(true):w->SetValue(true);
	
	//Note
	Rect.OffsetTo(10,135);
	AddChild(new BStringView(Rect,"","Note :"));
	 
	Rect.Set(0,0,260,110);
	Rect.OffsetTo(12,150);
	Note= new BTextView(Rect,"Note",BRect(0,0,Rect.Width(),Rect.Height()),B_FOLLOW_NONE, B_WILL_DRAW | 	B_NAVIGABLE);
	AddChild(new BScrollView("scroll text",Note,B_FOLLOW_NONE,0,false,true,B_PLAIN_BORDER));
	Note->SetText(database->Db[index]->Note);
	
	//Buttons
	Rect.Set(0,0,100,30);	

	Rect.OffsetTo(10,300);
	BButton *Edit=new BButton(Rect,"Edit","Ok",new BMessage('Edit'));
	AddChild(Edit);
	
	Rect.OffsetTo(190,300);
	BButton *Cancel=new BButton(Rect,"Cancel","Cancel",new BMessage('Canc'));
	AddChild(Cancel);
}

void EditView::Draw(BRect) 
{	
//	StrokeRect(BRect(10,148,292,262));
}

void MyColumnListView::KeyDown(const char *bytes, int32 numBytes)
{
	char u;
	BMessage *msg;
	int i;

	msg= new BMessage('indx');

	u=*bytes;	
	
	if (u>='A' && u<='Z')
	{
		for(i=0;i<database->Get_Nb_Pers() && database->Db[i]->Name[0]!=u;i++);
		if(i<database->Get_Nb_Pers())
			msg->AddInt32("index",i);
			be_app->PostMessage(msg);
	}
		
	if (u>='a' && u<='z')
	{
		u=u-('a'-'A'); //toupper ;) I love little tricks in ascii
		
		for(i=0;i<database->Get_Nb_Pers() && database->Db[i]->Name[0]!=u;i++);
		if(i<database->Get_Nb_Pers())
			msg->AddInt32("index",i);
		be_app->PostMessage(msg);
	}
	
	if (u>='0' && u<='9')
	{
		for(i=0;i<database->Get_Nb_Pers() && database->Db[i]->Name[0]!=u;i++);
		if(i<database->Get_Nb_Pers())
			msg->AddInt32("index",i);
		be_app->PostMessage(msg);
	}
	
	delete msg;		

	BListView::KeyDown(bytes,numBytes);
}

bool MyColumnListView::InitiateDrag(BPoint point,int32 index,bool wasSelected)
{
	BRect _rect(0.0, 0.0, 15.0, 15.0); 
	BBitmap *icon1 = new BBitmap(_rect, B_CMAP8);
	BPoint Mouse;
	uint32 *buttons=new uint32;
	GetMouse(&Mouse,buttons);
	BPoint where=point-Mouse+BPoint(15,15);
	char *Temp=new char[255],u[2];
	int i=0;

	//Load the icon to drag
	strcpy(Temp,SkinPath.String());
	strcat(Temp,"/Note");
	
	icon1=load_flat_bitmap(Temp);

	BMessage *msg;
	msg= new BMessage('DAdd');
	
	for(i=CurrentSelection();i<CountItems();i++)
		if(IsItemSelected(i)==true)
		{
			u[0]=database->Db[i]->Location;
			u[1]=0;
			msg->AddString("Name",database->Db[i]->Name);
			msg->AddString("Note",database->Db[i]->Note);
			msg->AddString("Tel",database->Db[i]->Tel_Number);
			msg->AddString("Location",u);
		}
	//Let's Drag :)
	DragMessage(msg,icon1,where);
	
	delete msg;
	delete buttons;
	delete Temp;
		
	return true;
}


Item::Item(uint32 level, bool superitem, bool expanded, BBitmap* icon, char* text0, char* text1, char* text2,char* text3)
: CLVListItem(level, superitem, expanded, 20.0)
{
	fIcon =  new BBitmap(BRect(0,0,15,15), B_CMAP8);
   	memcpy(fIcon->Bits(), icon->Bits(), icon->BitsLength());

	fText[0] = text0;
	fText[1] = text1;
	fText[2] = text2;
	fText[3] = text3;
}

Item::~Item()
{
//	for(int Counter = 0; Counter <= 2; Counter++)
//		delete[] fText[Counter];
}

void Item::DrawItemColumn(BView* owner, BRect item_column_rect, int32 column_index, bool complete)
{
	rgb_color color;
	bool selected = IsSelected();
	if(selected)
		color = BeListSelectGrey;
	else
		color = White;
	owner->SetLowColor(color);
	owner->SetDrawingMode(B_OP_COPY);
	if(selected || complete)
	{
		owner->SetHighColor(color);
		owner->FillRect(item_column_rect);
	}
	BRegion Region;
	Region.Include(item_column_rect);
	owner->ConstrainClippingRegion(&Region);
	owner->SetHighColor(Black);
	if(column_index == 0)
	{
		item_column_rect.left += 2.0;
		item_column_rect.right = item_column_rect.left + 15.0;
		item_column_rect.top += 2.0;
		if(Height() > 20.0)
			item_column_rect.top += ceil((Height()-20.0)/2.0);
		item_column_rect.bottom = item_column_rect.top + 15.0;
		owner->SetDrawingMode(B_OP_OVER);
		owner->DrawBitmap(fIcon, BRect(0.0,0.0,15.0,15.0),item_column_rect);
		owner->SetDrawingMode(B_OP_COPY);
	}
	else if(column_index >= 1)
		owner->DrawString(fText[column_index-1].String(),
			BPoint(item_column_rect.left+2.0,item_column_rect.top+fTextOffset));
	owner->ConstrainClippingRegion(NULL);
}

void Item::Update(BView *owner, const BFont *font)
{
	CLVListItem::Update(owner,font);
	font_height FontAttributes;
	be_plain_font->GetHeight(&FontAttributes);
	float FontHeight = ceil(FontAttributes.ascent) + ceil(FontAttributes.descent);
	fTextOffset = ceil(FontAttributes.ascent) + (Height()-FontHeight)/2.0;
}

int Item::MyCompare(const CLVListItem* a_Item1, const CLVListItem* a_Item2, int32 KeyColumn)
{
	char* Text1 = (char*)((Item*)a_Item1)->fText[KeyColumn-1].String();
	char* Text2 = (char*)((Item*)a_Item2)->fText[KeyColumn-1].String();
	return strcasecmp((char*)Text1,(char*)Text2);
}


