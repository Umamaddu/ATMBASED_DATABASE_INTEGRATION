#include "header.h"
void Save_To_File(void)
{
	int size,i;
	char filename[30];
	FILE *fptr1,*fptr2,*fptr3;
	fptr2=fopen("Bank_Details.csv","w");
	fptr1=fopen("Bank_Details.txt","w");
	SLL *temp=Account_Details;
	size=sizeof(SLL)-sizeof(SLL*);
	while(temp)
	{
		fprintf(fptr2,"%s,%d,%d,%d,%d,%s,%s,%ld,%lf,%d,%ld,%d\n",temp->Name,temp->Account_Number,temp->flag1,temp->flag2,temp->block,temp->pin,temp->RFID,temp->Contact_Number,temp->Account_Balance,temp->Trasnction_Count,temp->Aadhar_Number,temp->number);
	//	fwrite(temp,size,1,fptr1);
		sprintf(filename,"%d.csv",temp->Account_Number);
		fptr3=fopen(filename,"w");
		if(temp->flag1)
		{
			for(i=0;i<5;i++)
			{
				fprintf(fptr3,"%s,%lf,%d,%d,%d,%d,%d,%d,%d\n",temp->history[i].type,temp->history[i].amount,temp->history[i].Acc_Num,temp->history[i].month,temp->history[i].day,temp->history[i].year,temp->history[i].hour,temp->history[i].min,temp->history[i].trsncid);
			}
		}
		else
		{
			for(i=0;i<temp->Trasnction_Count;i++)
                        {
                                fprintf(fptr3,"%s,%lf,%d,%d,%d,%d,%d,%d,%d\n",temp->history[i].type,temp->history[i].amount,temp->history[i].Acc_Num,temp->history[i].month,temp->history[i].day,temp->history[i].year,temp->history[i].hour,temp->history[i].min,temp->history[i].trsncid);
                        }

		}
		fclose(fptr3);
		temp=temp->next;
	}
	fclose(fptr1);
	fclose(fptr2);
	printf("SAVED SUCCESSFULLY\n");
}
