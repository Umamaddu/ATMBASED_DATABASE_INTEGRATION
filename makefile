a.out:main.o Print_Menu.o Create_Account.o Withdraw.o Deposit.o Balance_Enquery.o Transfer_Money.o Display_All_Accounts.o Find_Specific_Account.o Save_To_File.o Sync_From_File.o History.o Compare_Aadhar.o Update_Account.o  
	cc main.o Print_Menu.o Create_Account.o Withdraw.o Deposit.o Balance_Enquery.o Transfer_Money.o Display_All_Accounts.o Find_Specific_Account.o Save_To_File.o Sync_From_File.o History.o Compare_Aadhar.o Update_Account.o -o a.out
main.o:main.c
	cc -c main.c
Print_Menu.o:Print_Menu.c
	cc -c Print_Menu.c
Create_Account.o:Create_Account.c
	cc -c Create_Account.c
Withdraw.o:Withdraw.c
	cc -c Withdraw.c
Deposit.o:Deposit.c
	cc -c Deposit.c
Balance_Enquery.o:Balance_Enquery.c
	cc -c Balance_Enquery.c
Transfer_Money.o:Transfer_Money.c
	cc -c Transfer_Money.c
Disolay_All_Accounts.o:Display_All_Accounts.c
	cc -c Display_All_Accounts.c
Find_Specific_Account.o:Find_Specific_Account.c
	cc -c Find_Specific_Account.c
Save_To_File.o:Save_To_File.c
	cc -c Save_To_File.c
Sync_From_File.o:Sync_From_File.c
	cc -c Sync_From_File.c
History.o:History.c
	cc -c History.c
Compare_Aadhar.o:Compare_Aadhar.c
	cc -c Compare_Aadhar.c
Update_Account.o:Update_Account.c
	cc -c Update_Account.c
