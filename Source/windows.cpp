#include "data.h"
#include "dos_beos.h"
#include "views.h"
#include "windows.h"
#include "bephone.h"
#include "Dial.h"

#include "be/kernel/OS.h"

MainWindow::MainWindow(BRect rect):BWindow(rect,"BePhoneBook",B_TITLED_WINDOW,B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	View= new MainView(Bounds());
	AddChild(View);
	

	//Let skin it :)
	View->SkinBPK();
}

void MainWindow::AddPerson(BNode fFile)
{
	int index=-1,j;
	attr_info	info;
	BString note="";
	char *TempString=NULL;
	Person Temp;
	Temp.Name ="Problem with the Name";
	Temp.Tel_Number="No phone number";
	Temp.Location='h';
	Temp.Note="";

	if(fFile.GetAttrInfo("META:name", &info)==B_NO_ERROR && info.size!=1)
	{
		Temp.Name = new char[info.size];
		fFile.ReadAttr("META:name", B_STRING_TYPE, 0, Temp.Name, info.size);
	}	
	else if(fFile.GetAttrInfo("META:nickname", &info)==B_NO_ERROR && info.size!=1)
	{
		Temp.Name = new char[info.size];
		fFile.ReadAttr("META:nickname", B_STRING_TYPE, 0, Temp.Name, info.size);
	}
	
	if(fFile.GetAttrInfo("META:hphone", &info)==B_NO_ERROR && info.size!=1)
	{
		Temp.Location='h';
		Temp.Tel_Number = new char[info.size];
		fFile.ReadAttr("META:hphone", B_STRING_TYPE, 0, Temp.Tel_Number, info.size);
	}
	
	if(fFile.GetAttrInfo("META:wphone", &info)==B_NO_ERROR && info.size!=1)
	{
		Temp.Location='w';
		Temp.Tel_Number = new char[info.size];
		fFile.ReadAttr("META:wphone", B_STRING_TYPE, 0, Temp.Tel_Number, info.size);
	}	
	
	if(fFile.GetAttrInfo("META:fax", &info)==B_NO_ERROR && info.size!=1)
	{
		Temp.Location='w';
		Temp.Tel_Number = new char[info.size];
		fFile.ReadAttr("META:fax", B_STRING_TYPE, 0, Temp.Tel_Number, info.size);
	}

	if(fFile.GetAttrInfo("META:address", &info)==B_NO_ERROR && info.size!=1)
	{
		if(TempString)
			delete TempString;
		TempString = new char[info.size];
		fFile.ReadAttr("META:address", B_STRING_TYPE, 0, TempString, info.size);
		note+="Adress :";
		note+=TempString;
		note+="\n";
	}
	
	if(fFile.GetAttrInfo("META:city", &info)==B_NO_ERROR && info.size!=1)
	{
		if(TempString)
			delete TempString;
		TempString = new char[info.size];
		fFile.ReadAttr("META:city", B_STRING_TYPE, 0, TempString, info.size);
		note+="Adress :";
		note+=TempString;
		note+="\n";
	}

	if(fFile.GetAttrInfo("META:state", &info)==B_NO_ERROR && info.size!=1)
	{
		if(TempString)
			delete TempString;
		TempString = new char[info.size];
		fFile.ReadAttr("META:state", B_STRING_TYPE, 0, TempString, info.size);
		note+="State :";
		note+=TempString;
		note+="\n";
	}

	if(fFile.GetAttrInfo("META:zip", &info)==B_NO_ERROR && info.size!=1)
	{
		if(TempString)
			delete TempString;
		TempString = new char[info.size];
		fFile.ReadAttr("META:zip", B_STRING_TYPE, 0, TempString, info.size);
		note+="Zip :";
		note+=TempString;
		note+="\n";
	}

	if(fFile.GetAttrInfo("META:country", &info)==B_NO_ERROR && info.size!=1)
	{
		if(TempString)
			delete TempString;
		TempString = new char[info.size];
		fFile.ReadAttr("META:country", B_STRING_TYPE, 0, TempString, info.size);
		note+="Country :";
		note+=TempString;
		note+="\n";
	}

	if(fFile.GetAttrInfo("META:email", &info)==B_NO_ERROR && info.size!=1)
	{
		if(TempString)
			delete TempString;
		TempString = new char[info.size];
		fFile.ReadAttr("META:email", B_STRING_TYPE, 0, TempString, info.size);
		note+="email :";
		note+=TempString;
		note+="\n";
	}

	if(fFile.GetAttrInfo("META:url", &info)==B_NO_ERROR && info.size!=1)
	{
		if(TempString)
			delete TempString;
		TempString = new char[info.size];
		fFile.ReadAttr("META:url", B_STRING_TYPE, 0, TempString, info.size);
		note+="url :";
		note+=TempString;
		note+="\n";
	}
	
	if(strcmp(note.String(),"")!=0)
	{
		Temp.Note=new char[note.Length()+1];
		strcpy(Temp.Note,note.String());
	}
	
	if(TempString)
		delete TempString;
	
	for(j=0;j<database->Get_Nb_Pers();j++)
		if(strcmp(database->Db[j]->Name,Temp.Name)==0)
			if(strcmp(database->Db[j]->Tel_Number,Temp.Tel_Number)==0)
				break;
		    		    
	if(j==database->Get_Nb_Pers())
		if(index==-1)
			index=database->Create(Temp);
		else
			database->Create(Temp);
		
	Saved=false;
	if(index!=-1)
		be_app->PostMessage('refr');
	
	//go to added person in the list
	BMessage *msg2;
	msg2= new BMessage('indx');
	msg2->AddInt32("index",index);
	be_app->PostMessage(msg2);
	delete msg2;
}

void MainWindow::MessageReceived(BMessage* msg)
{
	BString Temp2(BePhonePath.Path());
	BRoster MyRoster;
	BFilePanel *Save;
	BFilePanel *Load;
	BFilePanel *imports;
	BFilePanel *exports;
	BRect Self=Frame(),Rect;
	int i,j;
	BPath path;
	BEntry entry;
	entry_ref ref;
	char *mimetype=new char[255];	
	BNodeInfo Node;	
	BNode File;
	char *Tempchar;
	BDirectory Directory("/boot/home/people");
	
	switch(msg->what)
	{
		//Change the Skin
		case 'skin' :
		
		msg->FindString("SkinPath",(const char**)&Tempchar);
		SkinPath=Tempchar;

		View->SkinBPK();
		break;
		
		case 'com' :
		msg->FindString("com",(const char**)&Tempchar);
		strcpy(COM,Tempchar);
		break;
	
		case 'impe':
		while(Directory.GetNextEntry(&entry)!= B_ENTRY_NOT_FOUND)
		{
			entry.GetPath(&path); 
	
			File.SetTo(path.Path());
			Node.SetTo(&File);
			Node.GetType(mimetype);
				
			if(strcmp(mimetype,"application/x-person")==0)
				AddPerson(File);
		};	
		break;
	
		case 'DATA' :
		i=0;
		//Get reference of the file
		while(msg->FindRef("refs",i,&ref)==B_OK)
		{
			entry.SetTo(&ref);
			entry.GetPath(&path); 
	
			File.SetTo(path.Path());
			Node.SetTo(&File);
			Node.GetType(mimetype);
				
			if(strcmp(mimetype,"application/x-person")==0)
				AddPerson(File);
			//Is it a BePhoneBook File
			else if(strcmp(mimetype,"text/BePhoneBook")!=0 || !database->Load((char*)path.Path()))
						(new BAlert("","The file you tried to Load is not\na valid BePhoneBook File :(((","Ok"))->Go();
			else
			{
				Saved=true;
				PostMessage('refr');
			}
			i++;
		}
		break;
	
		case 'Add':
		//create an AddWindow
		Rect.Set(0,0,300,350);
		Rect.OffsetBy(Self.left+75,Self.top+75);
		AddW =new AddWindow(Rect);
		AddW->Show();
		break;
	
		case 'Edit':
		//create an EditWindow
		if (View->List->CurrentSelection()!=-1)
		{
			Rect.Set(0,0,300,350);
			Rect.OffsetBy(Self.left+75,Self.top+75);	
			EditW =new EditWindow(View->List->CurrentSelection(),Rect);
			EditW->Show();
		}
		break;
	
		case 'DAdd':
		struct Person *Temp;
		Temp=new struct Person;
		int index;
		i=0;
		index=-1;
		char *Name,*Tel,*Note,*Location;
	
		//Add items coming from another Bephonebook program running
		if (msg->IsSourceRemote()) //I am dragging to myself????
		{	
			while(msg->FindString("Name",i,(const char**)&Name)==B_OK)
			{
				msg->FindString("Tel",i,(const char**)&Tel);
				msg->FindString("Note",i,(const char**)&Note);
				msg->FindString("Location",i,(const char**)&Location);
				
				cout<<Name<<Tel<<Note<<Location<<"\n";
				
				Temp->Name=new char[(strlen(Name)+1)];
				strcpy(Temp->Name,Name);
				
				Temp->Tel_Number=new char[strlen(Tel)+1];
				strcpy(Temp->Tel_Number,Tel);
	
				Temp->Location=Location[0];
	
				Temp->Note=new char[strlen(Note)+1];
				strcpy(Temp->Note,Note);							
				
		    	for(j=0;j<database->Get_Nb_Pers();j++)
		    		if(strcmp(database->Db[j]->Name,Temp->Name)==0)
		    			if(strcmp(database->Db[j]->Tel_Number,Temp->Tel_Number)==0)
		    				break;
		    		    
		  		if(j==database->Get_Nb_Pers())
		   			if(index==-1)
		    			index=database->Create(*Temp);
					else
						database->Create(*Temp);
			
				i++;
				Saved=false;
				

			}
			
			delete Temp->Note;
       		delete Temp->Tel_Number;
      		delete Temp->Name;
     		delete Temp;

			if(index!=-1)
				be_app->PostMessage('refr');
			
			BMessage *msg2;
			msg2= new BMessage('indx');
			msg2->AddInt32("index",index);
		
			if(index!=-1)
				be_app->PostMessage(msg2);
		
			delete msg2;
		}
		break;

		case 'indx':
		//The Main list go  to index
		View->List->Select(msg->FindInt32("index"));
		View->List->ScrollToSelection();
		break;	
	
		case 'load':
		Load=new BFilePanel(B_OPEN_PANEL);
		
		Load->SetMessage(new BMessage ('load'));
		Load->Show();
		break;
	
		case 'impo':
		imports=new BFilePanel(B_OPEN_PANEL);
		
		imports->SetMessage(new BMessage ('impo'));
		imports->Show();
		break;
	
		case 'refr':
		View->Refresh();
		break;

		case 'save':
		if (database->Save()==true)
			Saved=true;
		else
			this->PostMessage('sava');
		break;
	
		case 'sava':
		Save=new BFilePanel(B_SAVE_PANEL);
	
		Save->Show();
		break;
	
		case 'expo':
		exports=new BFilePanel(B_SAVE_PANEL);
	
		exports->SetMessage(new BMessage ('expo'));
		exports->Show();
		break;
	
		case 'Dial':
		if(View->List->CurrentSelection()>-1)
		{
			char *Data[2];
			Data[0]=database->Db[View->List->CurrentSelection()]->Tel_Number;
			Data[1]=COM;
			thread_id ThreadId=spawn_thread(Dial,"",0,Data);
			resume_thread(ThreadId);
		}
		break;
		
		case 'new':
		if(Saved==false)
		{
			BAlert *myAlert;
			myAlert=new BAlert("","You didn't saved anything !!\nAre you sure you want to continue ?","Cancel","Save","Don't save", 
				B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_WARNING_ALERT); 
			myAlert->SetShortcut(0, B_ESCAPE);
			int32 button_index = myAlert->Go();
			
			//Save
			if(button_index==1)
			{
				if (database->Save()==false)
					this->PostMessage('sava');
				else
				{
					database->New();
					View->List->MakeEmpty();
					Saved=true;
				}
			
			}
			
			//Don t save
			if(button_index==2)
			{
				database->New();
				View->List->MakeEmpty();
				Saved=true;
			}
		}
		else
		{
			database->New();
			View->List->MakeEmpty();
		}
		break;
	
		case 'Note':
		if (View->List->CurrentSelection()>-1 && strlen(database->Db[View->List->CurrentSelection()]->Note)!=0) 
		{
			char *temp=new char[255];
			strcpy(temp,"Note for ");
			strcat(temp,database->Db[View->List->CurrentSelection()]->Name);
	
			Rect.Set(0,0,400,200);
			Rect.OffsetBy(Self.left+25,Self.top+75);
			NoteW=new NoteWindow(database->Db[View->List->CurrentSelection()]->Note,Rect,temp);
			NoteW->Show();
		
			delete temp;
		}
		break;

		case 'abut':
		//About requested
		Rect.Set(0,0,299,249);
		Rect.OffsetBy(Self.left+75,Self.top+75);
		AbW=new AboutWindow(Rect);
		AbW->Show();
		
		break;
	
		case 'Dele':
		//Delete for multiple selection list 11/11/99
		if (View->List->CountItems())
		{
			for(j=i=View->List->CurrentSelection();i<View->List->CountItems();)
				if(View->List->IsItemSelected(i)==true)
				{
					database->Delete(i);
					Saved=false;
					View->List->RemoveItem(i);
				}
				else
					i++;

			if(j!=database->Get_Nb_Pers())
				View->List->Select(j);
			else
				View->List->Select(j-1);
		}
		break;
	
		case 'help':
		Temp2+="/Doc/Readme.html";
		entry.SetTo(Temp2.String());
		entry.GetRef(&ref);
		MyRoster.Launch(&ref,0,(char **)NULL);
		
		break;
	
		default :
		BWindow::MessageReceived(msg);
		break;
	}
}

bool MainWindow::QuitRequested()
{
	if (Saved)
	{
		be_app->PostMessage(B_QUIT_REQUESTED);
		return(true);
	}
	else
	{
		BAlert *myAlert;
		myAlert=new BAlert("","You didn't saved anything !!\nAre you sure you want to quit ?","Cancel","Save and Exit","Don't save", 
			B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_WARNING_ALERT); 
		myAlert->SetShortcut(0, B_ESCAPE);
		int32 button_index = myAlert->Go();
		
		//Cancel
		if(button_index==0)
			return(false);
		
		//Save
		if(button_index==1)
		{
			if (database->Save()==true)
			{
				be_app->PostMessage(B_QUIT_REQUESTED);
				return(true);
			}
			else
			{
				
				this->PostMessage('sava');
				return(false);
			}
		}
		
		//Don t Save
		if(button_index==2)
		{
			be_app->PostMessage(B_QUIT_REQUESTED);
			return(true);
		}	
	}
}

NoteWindow::NoteWindow(char *Note,BRect rect,char *Title):BWindow(rect,Title,B_TITLED_WINDOW,B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	BRect Rect = Bounds();
	Rect.right-= 15;
	
	//BTextView + ScrollView
	View= new BTextView(Rect,"Note",Bounds(),B_FOLLOW_ALL,B_WILL_DRAW);
	View->SetText(Note);
	View->MakeEditable(false);
	
	AddChild(new BScrollView("scroll text",View,B_FOLLOW_NONE,0,false,true,B_PLAIN_BORDER));
}

AddWindow::AddWindow(BRect rect):BWindow(rect,"Add",B_TITLED_WINDOW,B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	View=new AddView(Bounds());
	AddChild(View);
}

void AddWindow::MessageReceived(BMessage* msg)
{
	int index;
	BString Name=View->Name->Text();
	Name.CapitalizeEachWord();
		
	switch(msg->what)
	{
		case 'Add':
		struct Person *Temp;
		Temp=new struct Person;
	
		//Have the user entered a Name and phone number?		
		if (strlen(View->Name->Text())==0 || strlen(View->Phone->Text())==0)
		{	
			(new BAlert("","To Add a Person,\n You Must enter a Name and a Phone Number","Ok"))->Go();	
			return;
		}	
		
		//creation of the Person
		Temp->Name=(char*)Name.String();
		Temp->Tel_Number=(char*)View->Phone->Text();	
		Temp->Note=(char*)View->Note->Text();
		
		if(View->h->Value())
			Temp->Location='h';
		else
			Temp->Location='w';	
	
		index=database->Create(*Temp);
	
		Saved=false;
	
		be_app->PostMessage('refr');
	
		delete Temp;
	
		//go to added person in the list
		BMessage *msg2;
		msg2= new BMessage('indx');
		msg2->AddInt32("index",index);
		be_app->PostMessage(msg2);
		delete msg2;
	
		Quit();
		break;

		case 'Canc':
		Quit();
		break;
	
		default :
		BWindow::MessageReceived(msg);
		break;
	}
}

EditWindow::EditWindow(int i,BRect rect):BWindow(rect,"Edit",B_TITLED_WINDOW,B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	index=i;
	
	View=new EditView(Bounds(),index);
	AddChild(View);
}

AboutWindow::AboutWindow(BRect rect):BWindow(rect,"About BePhoneBook",B_TITLED_WINDOW,B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	View=new AboutView(Bounds());
	AddChild(View);
}

void EditWindow::MessageReceived(BMessage* msg)
{
	BString Name=View->Name->Text();
	Name.CapitalizeEachWord();
		
	switch(msg->what)
	{
		case 'Edit':
		struct Person *Temp;
		Temp=new struct Person;
	
		//Have the user entered a Name and phone number?	
		if (strlen(View->Name->Text())==0 || strlen(View->Phone->Text())==0)
		{	
			(new BAlert("","To Edit a Person,\n You Must enter a Name and a Phone Number","Ok"))->Go();	
			return;
		}	
		
		//Modify the Person
		Temp->Name=(char*)Name.String();
		Temp->Tel_Number=(char*)View->Phone->Text();	
		Temp->Note=(char*)View->Note->Text();
		
		if(View->h->Value())
			Temp->Location='h';
		else
			Temp->Location='w';
		index=database->Modif(Temp,index);
		delete Temp;
	
		Saved=false;
	
		be_app->PostMessage('refr');
		
		//goto the modified person in the list
		BMessage *msg2;
		msg2= new BMessage('indx');
		msg2->AddInt32("index",index);
		be_app->PostMessage(msg2);
		delete msg2;
	
		Quit();
		break;
		
		case 'Canc':
		Quit();
		break;
	
		default :
		BWindow::MessageReceived(msg);
		break;
	}
}
