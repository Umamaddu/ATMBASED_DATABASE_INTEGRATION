#include "header.h"
int count(long int num)
{
	int d=0,j;
	while(num>0)
	{
		j=num%10;
		d++;
		num/=10;
	}
	if(d>10)
	return 1;
	else
	return 0;
}
void Withdraw(void)
{
	SLL *temp;
	//Trsnc *New_History;
	long int acc_num;
	int i;
	double amount;
	printf("Enter Account Number/Aadhar Number:");
	scanf("%ld",&acc_num);
	temp=Account_Details;
	while(temp)
	{
		if((temp->Account_Number==acc_num)||(temp->Aadhar_Number==acc_num))
		{
			break;
		}
		temp=temp->next;
	}
	if(temp==NULL)
	{
		printf("Invalid Account Number!\n");
	}
	else 
	{
		if(count(acc_num))
		{
OTP:printf("Enter the OTP sent to your mobile number:");
			scanf("%d",&i);
			if(i<0)
			{
				printf("Invalid OTP !");
				goto OTP;
			}
		}

amount:printf("Enter amount to withdraw:");
		scanf("%lf",&amount);
		if(amount<0.0)
		{
			printf("Invalid amount!\n");
			goto amount;
		}
		else if(amount>temp->Account_Balance)
		{
			printf("Insufficient balance!\n");
			goto amount;
		}
		else
		{
			if(temp->number>=5)
			{
				temp->number=0;
				temp->flag=1;
			}
			temp->Account_Balance=(temp->Account_Balance-amount);
			temp->Trasnction_Count++;
			strcpy(temp->history[temp->number].type,"withdraw");
			temp->history[temp->number].amount=amount;
			temp->history[temp->number].Acc_Num=temp->Account_Number;
			temp->number++;
			printf("WITHDRAWAL SUCCESSFUL\n");	
		}
	}
}
