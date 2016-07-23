#include "bephone.h"
#include "dos_beos.h"
#include "windows.h"
#include "views.h"
#include "MenuItem.h"
#include "Button.h"
#include "ListView.h"

#include "Dial.h"

int32 Dial(void *data)
{
	char Temp[255];
	char **data2=(char **)data;
		
	char *PhoneNumber=data2[0];
	char *SerialName=data2[1];

	int j=0;
	
	char *Number=new char[strlen(PhoneNumber)];
	for(int i=0;i<strlen(PhoneNumber);i++)
		if(PhoneNumber[i]>='0' && PhoneNumber[i]<='9')
			Number[j++]=PhoneNumber[i];			
	Number[j]=0;
		
	BSerialPort serial;
	
	//serial init
	serial.SetDataBits(B_DATA_BITS_8);
	serial.SetStopBits(B_STOP_BITS_1);
	serial.SetParityMode(B_NO_PARITY);
	serial.SetDataRate(B_19200_BPS);
	serial.SetFlowControl(B_HARDWARE_CONTROL);

	char num[255]="ATDT";
	strcat(num,Number);
	char u='\r';
	int nb=strlen(num);
	num[nb]=u;
	num[nb+1]=0;

	serial.Open(SerialName);	
	
	//Compose the phone number
	serial.Write(num,strlen(num) *sizeof(char));

	
	
	BString Text="Composing ";
	sscanf(PhoneNumber,"%s",Temp);
	Text+=Temp;
	Text+="\nTake the Phone and press Ok";

	BAlert *Alert;
	Alert=new BAlert("",Text.String(),"Ok");
	Alert->Go();
	
	char command[255]="ATH0";
	nb=strlen(command);
	command[nb]=u;
	command[nb+1]=0;
	
	serial.Write(command,strlen(command) *sizeof(char));
	
	serial.Close();
}
