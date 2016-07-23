#include "bephone.h"
#include "data.h"

int Data::Alphabetic(int nb)
{
	int i,j,index=nb;
	struct Person* Temp;
	
	//Alphabetical order
	for(i=0;i < Nb_Pers;i++)
		for(j=i+1;j < Nb_Pers;j++)
			if (strcmp(Db[i]->Name,Db[j]->Name)>0)
			{
				if(i==nb)
				{
					index=j;
					nb=j;
				}
				else if(j==nb)
				{
					index=i;
					nb=i;
				}
				
				Temp=Db[i];
				Db[i]=Db[j];
				Db[j]=Temp;
			}
						
	return index;
}

int Data::Create(struct Person &New_Item)
{
	int i,j;
	
	if(Nb_Pers==(NbMax-1))
		ReNew();
	
	//Alphabetical order
	for(i=0;i<Nb_Pers ;i++)
		if (strcmp(New_Item.Name,Db[i]->Name)<0)
			break;
				
	for(j=Nb_Pers;j>i;j--)
		Db[j]=Db[j-1];
	
	Db[i]=new Person(New_Item);

	Db[i]->Name=new char[strlen(New_Item.Name)+1];
	strcpy(Db[i]->Name,New_Item.Name);
	
	Db[i]->Tel_Number=new char[strlen(New_Item.Tel_Number)+1];
	strcpy(Db[i]->Tel_Number,New_Item.Tel_Number);
	
	Db[i]->Location=New_Item.Location;
	
	Db[i]->Note=new char[strlen(New_Item.Note)+1];
	strcpy(Db[i]->Note,New_Item.Note);
	
	Nb_Pers++;
		
	return i;

}

void Data::Delete(int Nb)
{
	int i;
	
	for(i=Nb;i<Nb_Pers;i++)
		Db[i]= Db[i+1];	
	if(Nb_Pers!=0)	
		Nb_Pers--;
}

int Data::Modif(struct Person *New_Item,int Nb)
{
	//Delete the current one	
	delete Db[Nb]->Note;
    delete Db[Nb]->Tel_Number;
    delete Db[Nb]->Name;
    delete Db[Nb];	

	printf("%d\n",Nb);
    
    struct Person* Temp = new Person; //Add a new person
		
	Temp->Name=new char[strlen(New_Item->Name)+1];
	strcpy(Temp->Name,New_Item->Name);
		
	Temp->Tel_Number=new char[strlen(New_Item->Tel_Number)+1];
	strcpy(Temp->Tel_Number,New_Item->Tel_Number);

	Temp->Location=New_Item->Location;
		
	Temp->Note=new char[strlen(New_Item->Note)+1];
	strcpy(Temp->Note,New_Item->Note);

	Db[Nb]= Temp; //Assign the pointer of the new person
		
	return Alphabetic(Nb);
}

bool Data::Save()
{
	if (NameFile[0]!=0)
	{
		Save(NameFile);
		return(true);
	}
	else
		return(false);
}

bool Data::Save(char * Path)
{
	FILE *stream;
	
	stream = fopen(Path,"w");
	strcpy(NameFile,Path);
	for(int i=0;i<Nb_Pers;i++)
	{
		fprintf(stream,"%s\n",Db[i]->Name);
		fprintf(stream,"%s\n",Db[i]->Tel_Number);
		fprintf(stream,"%c\n",Db[i]->Location);
		if(strlen(Db[i]->Note))
			fprintf(stream,"[%s]\n",Db[i]->Note);
		else
			fprintf(stream,"\n");
		fprintf(stream,"\n");
	}	
	
	fclose (stream);

	return (true);
}

bool Data::Load(char* Path)
{
	FILE *fichier;
       
    fichier=fopen(Path, "r");
    strcpy(NameFile,Path);
    Free();

    char u;
	struct Person *New_Item;
	BString Temp;
		
	New_Item=new Person;
	
	while(!feof(fichier))
    {
       	u=fgetc(fichier);
        	
       	if(feof(fichier))
       		break;
        	
       	Temp="";                
        while(u!='\n')
        {
           	 Temp+=u;
	         u=fgetc(fichier);
        }

		if(strlen(Temp.String())!=0)
		{
			New_Item->Name=new char[Temp.Length()+1];
			strcpy(New_Item->Name,Temp.String());
		}
		else
		{
			Free();
			fclose(fichier);
			return (false);
		}
				
        Temp="";                
        u=fgetc(fichier);
            
        if(feof(fichier))
      		break;
            
        while(u!='\n')
        {
            Temp+=u;
            u=fgetc(fichier);
        }

		if(strlen(Temp.String())!=0)	
		{
			New_Item->Tel_Number=new char[Temp.Length()+1];
			strcpy(New_Item->Tel_Number,Temp.String());
		}
		else
		{
			Free();
			fclose(fichier);
			return (false);
        }
                
        Temp="";                
        u=fgetc(fichier);

       	if(feof(fichier))
       		break;
        	
       	while(u!='\n')
        {
 	       Temp+=u;
           u=fgetc(fichier);
        }
                
		if(strlen(Temp.String())==1)
			New_Item->Location= *(Temp.String());
		else
		{
			Free();
			fclose(fichier);
			return (false);
		}
			
        Temp="";                
        u=fgetc(fichier);
            
       	if(feof(fichier))
       		break;            
            
        if (u=='[')
        {
       		u=fgetc(fichier);
           	while(u!=']')
            {
           	    Temp+=u;
                u=fgetc(fichier);
            }
                
           	New_Item->Note=new char[Temp.Length()+1];
			strcpy(New_Item->Note,Temp.String());
               
            u=fgetc(fichier);
        }
		else
		{	
			New_Item->Note=new char;
			strcpy(New_Item->Note,"");
		}
		Create(*New_Item);   
			
		delete New_Item->Note;
      	delete New_Item->Tel_Number;
     	delete New_Item->Name;
				
 		u=fgetc(fichier);
	}
    fclose(fichier);
        
    delete New_Item;
        
	return (true);
}

void Data::New()
{
	NameFile[0]=0;
	Free();
}
void Data::Free()
{
	for(int i=0;i<Nb_Pers;i++)
	{
		//delete Db[i]->Location;
		delete[] Db[i]->Note;
    	delete[] Db[i]->Tel_Number;
    	delete[] Db[i]->Name;
    	delete Db[i];
	}
	Nb_Pers=0;
}

void Data::ReNew()
{
	struct Person **TDb;
	TDb=new struct Person *[NbMax+10];
	memcpy(TDb,Db,sizeof(Person *)*Nb_Pers);
	delete[] Db;
	Db=TDb;
	NbMax+=10;
} 
	