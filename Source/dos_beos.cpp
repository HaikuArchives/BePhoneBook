#include "dos_beos.h"
#include "bephone.h"

//DOS text to Unix/BEOS text
void imports(char *Paths,char *Pathd)
{
	    FILE *fichiere,*fichiers;
        fichiere=fopen(Paths, "r");
        fichiers=fopen(Pathd, "w");

        char u;
        u=fgetc(fichiere);   
        while(u!=EOF)
        {
       			
       		if(u!=0X0D)
       			fprintf(fichiers,"%c",u);	
 
        	u=fgetc(fichiere);
        }
        
        fclose(fichiere);
        fclose(fichiers);
}

//Unix/BEOS text to DOS text
void exports(char *Paths,char *Pathd)
{
	    FILE *fichiere,*fichiers;
        fichiere=fopen(Paths, "r");
        fichiers=fopen(Pathd, "w");
        
        char u;
                
        u=fgetc(fichiere);
        while(u!=EOF)
        {
       		if(u==0X0A)
    			fprintf(fichiers,"%c",0X0D);

  			fprintf(fichiers,"%c",u);		
        	
        	u=fgetc(fichiere);       
        }
        fclose(fichiers);
        fclose(fichiere);
        
}

