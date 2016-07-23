#ifndef _VIEWS_H_
#define _VIEWS_H_

#include <View.h>
#include <MenuBar.h>
#include <ScrollView.h>
#include <RadioButton.h>
#include <TextControl.h>
#include <Alert.h>
#include <Bitmap.h>
#include <Picture.h>
#include <PictureButton.h>

#include "ColumnListView.h"
#include "CLVListItem.h"
#include "Dial.h"
#include <Region.h>
#include <BitmapStream.h>
#include <TranslationUtils.h>
#include <String.h>

BBitmap *load_flat_bitmap(const char *path);

class MainList;

class MainView:public BView
{
	public :
	MainView(BRect);
	void Refresh();
	void SkinBPK();
	
	ColumnListView *List;
	BPictureButton *Add,*Edit,*Delete,*Note,*Dial;
	bool FirstTime;
};

class MyColumnListView:public ColumnListView
{
	public:
	MyColumnListView(CLVContainerView** containerView):ColumnListView(BRect(10,30,425,380),containerView,"List",B_FOLLOW_ALL_SIDES,
		B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE,B_MULTIPLE_SELECTION_LIST,false,false,true,false,B_PLAIN_BORDER){SetInvocationMessage(new BMessage('Dial'));}
	virtual void KeyDown(const char*,int32);
	virtual bool InitiateDrag(BPoint ,int32 ,bool);
};

class AddView:public BView
{
	public :
	AddView(BRect);
	virtual void Draw(BRect);
	virtual void AttachedToWindow(){Name->MakeFocus();}
	
	BTextControl *Name,*Phone;
	BRadioButton *h,*w;	
	BTextView *Note;
};

class EditView:public BView
{
	public :
	EditView(BRect,int);
	virtual void Draw(BRect);
	virtual void AttachedToWindow(){Name->MakeFocus();}
	
	BTextControl *Name,*Phone;
	BRadioButton *h,*w;	
	BTextView *Note;
	
	private:
	int index;
};

class AboutView:public BView
{
	public :
	AboutView(BRect);
	BBitmap *Bitmap;
	virtual void Draw(BRect);
};

class Item : public CLVListItem
{
	public:
		Item(uint32 level, bool superitem, bool expanded, BBitmap* icon, char* text0, char* text1, char* text2,char* text3);
		~Item();
		void DrawItemColumn(BView* owner, BRect item_column_rect, int32 column_index, bool complete);
		void Update(BView *owner, const BFont *font);
		static int MyCompare(const CLVListItem* Item1, const CLVListItem* Item2, int32 KeyColumn);

		BString fText[4];
			
	private:
		BBitmap* fIcon;
		float fTextOffset;
};

#endif