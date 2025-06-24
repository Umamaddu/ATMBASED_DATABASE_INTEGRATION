#include "header.h"
void History(void)
{
	SLL *temp;
	int i;
	long int num;
	temp=Account_Details;
	if(temp==NULL)
	{
		printf("no account is created up to now\n");
	}
	else
	{
		printf("enter account number/Aadhar number:");
		scanf("%ld",&num);
		while(temp)
		{
			if((temp->Account_Number==num)||(temp->Aadhar_Number==num))
			{
				break;
			}
			temp=temp->next;
		}
		if(temp==NULL)
		{
			printf("no account found on this number\n\n");
		}
		else
		{
			printf("\n");
			printf("No.of    Type of Trasnctions       D/M/Y    TIME Account Number      Amount\n");
			if(temp->flag1)
			{
				for(i=0;i<5;i++)
				{
				printf("%d \t%s\t\t\t%d/%d/%d    %d:%d  %d\t\t%.3f",
				i+1,temp->history[i].type,temp->history[i].day,temp->history[i].month,temp->history[i].year,temp->history[i].hour,temp->history[i].min,temp->history[i].Acc_Num,temp->history[i].amount);
					printf("\n");	
				}
			}
			else
			{
				for(i=0;i<temp->number;i++)
				{
				printf("%d \t%s\t\t\t%d/%d/%d    %d:%d  %d\t\t%.3f",
				i+1,temp->history[i].type,temp->history[i].day,temp->history[i].month,temp->history[i].year,temp->history[i].hour,temp->history[i].min,temp->history[i].Acc_Num,temp->history[i].amount);
					printf("\n");	
	/*printf("%d \t%s\t\t\t%d\t\t%.3f",i+1,temp->history[i].type,temp->history[i].Acc_Num,temp->history[i].amount);
					printf("\n");	*/
				}

			}
		}
		printf("\n");
	}
}
