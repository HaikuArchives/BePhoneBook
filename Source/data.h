#ifndef DATA_H
#define DATA_H

#include <stdio.h>


struct Person
{
	char *Name;
	char *Tel_Number;
	char Location;
	char *Note;
};
	
class Data
{
	public:	
	struct Person **Db;
	
	int Alphabetic(int);
	bool Save(char*);
	bool Save(void);
	bool Load(char*);
	Data(void){Nb_Pers=0;NameFile[0]=0;NbMax=10;Db=new struct Person*[10];};
	int Create(struct Person &);
	void Delete(int);
	int Modif(struct Person*,int);
	int Get_Nb_Pers(){return Nb_Pers;};
	void Free();
	void New();
	void ReNew();
	
	private:
	int Nb_Pers;
	int NbMax;
	char NameFile[255];
	
};

#endif


