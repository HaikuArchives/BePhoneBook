#include "data.h"
#include "bephone.h"
#include "dos_beos.h"
#include "windows.h"
#include "views.h"
#include "MenuItem.h"
#include "Button.h"
#include "ListView.h"

//load the settings
void ConfLoad()
{
	FILE *fichier;
    
	BString Path(BePhonePath.Path());
	Path+="/Config";
	char skin[255];
     
    if((fichier=fopen(Path.String(), "r"))!=NULL)
    {
		fscanf(fichier,"%s\n",skin);
		SkinPath.SetTo(skin);
		
		fscanf(fichier,"%s\n",COM);
		
		BNode node(skin);
		if(node.InitCheck()!=B_OK)
		{
			Path.SetTo(BePhonePath.Path());
			Path+="/Skins/Default";
			SkinPath=Path;
			strcpy(COM,"serial1");
		}
    }
    else
    {
		Path.SetTo(BePhonePath.Path());
		Path+="/Skins/Default";
		SkinPath=Path;
		strcpy(COM,"serial1");
	}
};

//Save the settings
void ConfSave()
{
	FILE *fichier;
	BString Path(BePhonePath.Path());
	Path+="/Config";
    	 
    if((fichier=fopen(Path.String(), "w"))!=NULL)
    {
		fprintf(fichier,"%s\n",SkinPath.String());
		fprintf(fichier,"%s\n",COM);
    }
};
