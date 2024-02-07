#include <cmath>
#include <iostream>
#include <vector>
#include <type_traits>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

ofstream ofile;
ifstream ifile("Accountslist.txt");
vector<vector < string>> Accounts = {
		{
		"CID",
		"Username",
		"Name",
		"Account Type", "Org", "Status", "DOB", "DOJ", "SSN", "Password" }
};

int Changed, Active = 0;
int Nums = 1;
vector<vector < string>> Inbox;
vector<vector < float>> Transactions;
int g_cid;
string AdminAccount[2][2] = {
		{
		"UserName",
		"Password" },
	{
		"None",
		"None" }
};

void main_menu();
void Customersignup();
void Adminsignin();
void Customersignin();
template < typename T > string encrypt(T to_en)
{
	stringstream ss;
	ss << to_en;
	string out = ss.str();
	for (int i = 0; i < out.length(); i++)
	{
		out[i] += 5;
	}

	return out;
}

template < typename T > T decrypt(string to_de)
{
	vector<char> chararr;
	for (int i = 0; i < to_de.length(); i++)
	{
		to_de[i] = to_de[i] - 5;
		chararr.push_back(to_de[i]);
	}

	stringstream ss(to_de);
	T out;
	if (is_same<T, string>::value)
	{
		for (int i = 0; i < chararr.size(); i++)
		{
			out += chararr[i];
		}
	}
	else
	{
		ss >> out;
	}

	return out;
}

vector<string> split(string inputStr)
{
	vector<string> words;
	string word;
	for (int i = 0; i < inputStr.length(); i++)
	{
		if (inputStr[i] != ' ')
		{
			word += inputStr[i];
		}
		else if (!word.empty())
		{
			words.push_back(word);
			word = "";
		}
	}

	if (!word.empty())
	{
		words.push_back(word);
	}

	return words;
}

class Customer
{
	private:
		int CID;
	string Username, Name, Type, Organization, SSN, Status, DOB, DOJ, Password;
	vector<string> messages;
	vector<float> transactions;
	float currentbal;
	public:
		Customer() {}

	Customer(string user, string pass)
	{
		Username = user;
		Password = pass;
		CID = g_cid + 1;
		Name = Accounts[CID][2];
		Type = Accounts[CID][3];
		Organization = Accounts[CID][4];
		Status = Accounts[CID][5];
		DOB = Accounts[CID][6];
		DOJ = Accounts[CID][7];
		SSN = Accounts[CID][8];
		messages = Inbox[g_cid];
		transactions = Transactions[g_cid];
		currentbal = initbal();
	}

	Customer(int cid, string user, string name, string type, string org, string stat, string dob, string doj, string ssn, string pass)
	{
		Username = user;
		Password = pass;
		CID = cid;
		Name = name;
		Type = type;
		Organization = org;
		Status = stat;
		DOB = dob;
		DOJ = doj;
		SSN = ssn;
		messages = Inbox[g_cid];
		transactions = Transactions[g_cid];
		currentbal = 0;
	}

	void StatementsummarylastNtransactions()
	{
		int num;
		int len = Inbox[g_cid].size();
		cout << "Enter the number of Transactions you want a summary for: ";
		cin >> num;
		cout << "Summary Last " << num << " Transaction(s):\n\n";
		if (num > Transactions[g_cid].size())
		{
			cout << "The input value is greater than total transactions.\n";
		}
		else
		{
			for (int i = 0; i < num; i++)
			{
				cout << "Transaction Number: " << len - i - 1 << ":\n";
				float amnt = Transactions[g_cid][len - i - 1];
				float tot = 0;
				if (amnt >= 0)
				{
					cout << "Deposited/Earned " << amnt << endl;
				}
				else
				{
					cout << "Withdrew " << -1 * amnt << endl;
				}

				for (int j = 0; j < len - i; j++)
				{
					tot += Transactions[g_cid][j];
				}

				cout << "Balance After Transaction: " <<
					tot <<
					endl << endl;
			}
		}

		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		CustomerMenu();
	}

	void CurrentBalance()
	{
		cout << "Your Current Balance is: ";
		cout << currentbal << endl;
		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		CustomerMenu();
	}

	float initbal()
	{
		float Bal;
		for (int j = 0; j < Transactions[g_cid].size(); j++)
		{
			Bal += Transactions[g_cid][j];
		}

		return Bal;
	}

	void Withdraw()
	{
		int amnt;
		cout << "Enter the amount you want to Withdraw: ";
		cin >> amnt;
		// if (Transactions[g_cid].back() < amnt) {
		//  cout << "Insufficient Balance\n";
		// } else {
		Transactions[g_cid].push_back(-1 *amnt);
		Inbox[g_cid].push_back("Withdrew " + to_string(amnt));
		currentbal -= amnt;
		//}

		cout << "Approved\n";
		cout << "Press Enter to continue...";
		cin.get();
		cin.get();
		CustomerMenu();
	}

	void Deposit()
	{
		int amnt;
		cout << "Enter the amount you want to Deposit: ";
		cin >> amnt;
		Transactions[g_cid].push_back(amnt);
		string newmess = "Deposited " + to_string(amnt);
		Inbox[g_cid].push_back(newmess);
		currentbal += amnt;
		cout << "Approved.\n";
		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		CustomerMenu();
	}

	void change_cid()
	{
		int input = 0;
		cout << "1)Confirm change of CID\n";
		cout << "2)Go Back\n";
		cin >> input;
		switch (input)
		{
			case 1:
				{
					vector<string> cacc = { to_string(Nums),
						Accounts[g_cid + 1][1],
						Accounts[g_cid + 1][2],
						Accounts[g_cid + 1][3],
						Accounts[g_cid + 1][4],
						Accounts[g_cid + 1][5],
						Accounts[g_cid + 1][6],
						Accounts[g_cid + 1][7],
						Accounts[g_cid + 1][8],
						Accounts[g_cid + 1][9]
					};

					Accounts.push_back(cacc);
					Accounts[g_cid + 1][5] = "Changed";
					Transactions.push_back(Transactions[g_cid]);
          Inbox[g_cid].push_back("Account Changed!");
					Inbox.push_back(Inbox[g_cid]);
					Transactions[g_cid] = { 0 };

					g_cid = Nums - 1;
					Nums++;
					Changed++;
					cout << "CID Changed.\n";
					cout << "Press Enter to Continue...";
					cin.get();
					cin.get();
					CustomerMenu();
				}

				CustomerMenu();
			case 2:
				{
					CustomerMenu();
				}

			default:
				{
					cout << "Please make a valid choice\n";
					change_cid();
				}
		}
	}

	void check_inbox()
	{
		cout << "Your Most Recent Messages:\n";
		int len = Inbox[g_cid].size();
		for (int i = 0; i < len; i++)
		{
			cout << Inbox[g_cid][len - i - 1] << endl;
		}

		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		CustomerMenu();
	}

	void CustomerMenu()
	{
		int input = 0;
		cout << "\n1)Statement Summary of Recent Transactions\n";
		cout << "2)CurrentBalance\n";
		cout << "3)Withdraw.\n";
		cout << "4)Deposit.\n";
		cout << "5)Change CID\n";
		cout << "6)Check Inbox\n";
		cout << "7)Logout\n";
		cout << "\nPlease enter your choice: ";
		cin >> input;
		switch (input)
		{
			case 1:
				StatementsummarylastNtransactions();
			case 2:
				CurrentBalance();
			case 3:
				Withdraw();
			case 4:
				Deposit();
			case 5:
				change_cid();
			case 6:
				check_inbox();
			case 7:
				cout << "Good Bye\n";
				g_cid = 0;
				main_menu();
			default:
				cout << "Please make a valid choice\n";
				CustomerMenu();
		}
	}
};

class Admin
{
	private:
		string Username, Password;
	public:
		Admin() {}

	Admin(string user, string pass)
	{
		Username = user;
		Password = pass;
	}

	void payinterest()
	{
		int cid;
		float amnt;
		cout << "Enter Account CID you want to pay interest to: \n";
		cin >> cid;
		if (cid < Nums)
		{
			cout << "Enter amount: ";
			cin >> amnt;
			Transactions[cid - 1].push_back(amnt);
			Inbox[cid - 1].push_back("Interest Earned " + to_string(amnt));
			cout << "Interest Paid.\n";
		}
		else
		{
			cout << "CID does not exist\n";
		}

		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		AdminMenu();
	}

	void checking_neg_Balance()
	{
		cout << "Accounts with Negative Balances: \n";
		for (int i = 1; i < Nums; i++)
		{
			float Bal = 0;
			for (int j = 0; j < Transactions[i - 1].size(); j++)
			{
				Bal += Transactions[i - 1][j];
			}

			if (Bal < 0)
			{
				cout << "CID: " << Accounts[i][0] << " Username: " <<
					Accounts[i][1] << " Balance: " << Bal << endl;
			}
		}

		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		AdminMenu();
	}

	void checking_pos_Balance()
	{
		cout << "Accounts with Positive Balances: \n";
		for (int i = 1; i < Nums; i++)
		{
			float Bal = 0;
			for (int j = 0; j < Transactions[i - 1].size(); j++)
			{
				Bal += Transactions[i - 1][j];
			}

			if (Bal >= 0)
			{
				cout << "CID: " << Accounts[i][0] << " Username: " <<
					Accounts[i][1] << " Balance: " << Bal << endl;
			}
		}

		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		AdminMenu();
	}

	void warn_account()
	{
		for (int i = 1; i < Nums; i++)
		{
			float Bal = 0;
			for (int j = 0; j < Transactions[i - 1].size(); j++)
			{
				Bal += Transactions[i - 1][j];
			}

			if (Bal < 0)
			{
				Inbox[i - 1].push_back("Your Account has a Negative Balance. You are at risk for being Banned.");
			}
		}

		cout << "Accounts with Negative Balance Warned" << endl;
		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		AdminMenu();
	}

	void blockaccount()
	{
		int cid;
		cout << "Accounts with Negative Balances: \n";
		for (int i = 1; i < Nums; i++)
		{
			float Bal = 0;
			for (int j = 0; j < Transactions[i - 1].size(); j++)
			{
				Bal += Transactions[i - 1][j];
			}

			if (Bal < 0)
			{
				cout << "CID: " << Accounts[i][0] << " Username: " <<
					Accounts[i][1] << " Balance: " << Bal << endl;
			}
		}

		cout << "Enter CID to block\n";
		cin >> cid;
		if (cid < Nums)
		{
			Accounts[cid][5] = "Blocked";
			Active--;
			Inbox[cid - 1].push_back("Your Account has Been blocked.");
			cout << "Account Blocked\n";
		}
		else
		{
			cout << "CID does not exist\n";
		}

		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		AdminMenu();
	}

	void summary()
	{
		cout << "Bank Summary: \n";
		cout << "Total Customers = " << Nums - 1 - Changed << endl;
		cout << "Active Customers= " << Active << endl;
		cout << "Inactive Customers= " << Nums - Active - 1 - Changed << endl;
		cout << "Accounts with Negative Balances: \n";
		for (int i = 1; i < Nums; i++)
		{
			float Bal = 0;
			for (int j = 0; j < Transactions[i - 1].size(); j++)
			{
				Bal += Transactions[i - 1][j];
			}

			if (Bal < 0)
			{
				cout << "CID: " << Accounts[i][0] << " Username: " <<
					Accounts[i][1] << " Balance: " << Bal << endl;
			}
		}

		cout << "Accounts with Positive Balances: \n";
		for (int i = 1; i < Nums; i++)
		{
			float Bal = 0;
			for (int j = 0; j < Transactions[i - 1].size(); j++)
			{
				Bal += Transactions[i - 1][j];
			}

			if (Bal >= 0)
			{
				cout << "CID: " << Accounts[i][0] << " Username: " <<
					Accounts[i][1] << " Balance: " << Bal << " Status: " << Accounts[i][5] << endl;
			}
		}

		cout << "Press Enter to Continue...";
		cin.get();
		cin.get();
		AdminMenu();
	}

	void AdminMenu()
	{
		int input = 0;
		cout << "\n1)Pay Monthly Interest.\n";
		cout << "2)Check Account with Negative Balance\n";
		cout << "3)Check Account with Positive Balance\n";
		cout << "4)Warn Accounts with Negative Balance.\n";
		cout << "5)Block account.\n";
		cout << "6)Generate Summary Report.\n";
		cout << "7)Logout\n";
		cout << "\nPlease enter your choice: ";
		cin >> input;
		switch (input)
		{
			case 1:
				payinterest();
			case 2:
				checking_neg_Balance();
			case 3:
				checking_pos_Balance();
			case 4:
				warn_account();
			case 5:
				blockaccount();
			case 6:
				summary();
			case 7:
				cout << "Good Bye\n";
				main_menu();
			default:
				cout << "Please make a valid choice\n";
				AdminMenu();
		}
	}
};

string CPassword()
{
	string pass;
	cout <<
		"\nEnter Desired Password (1 lower, 1 upper, 1 digit, 6+ characters): ";
	cin >> pass;
	int l = pass.length();
	bool lower = false, upper = false, digit = false;
	for (int i = 0; i < l; i++)
	{
		if (islower(pass[i]))
			lower = true;
		if (isupper(pass[i]))
			upper = true;
		if (isdigit(pass[i]))
			digit = true;
	}

	if (lower && upper && digit && (l >= 6))
	{
		return pass;
	}
	else
	{
		cout <<
			"Password is too weak. Enter at least 1 Uppercase letter, 1 Lowercase letter, 1 Digit and have at least 6 characters\n";
		Customersignup();
	}

	return "0";
}

string User_Name()
{
	string user;
	cout << "\nEnter your Desired Username: ";
	cin >> user;
	for (int i = 0; i < Accounts.size(); i++)
	{
		if (user == Accounts[i][1])
		{
			cout << "Sorry Username is already taken.\n";
			Customersignup();
		}
	}

	return user;
}

void Customersignup()
{
	cin.clear();
	cin.ignore();
	string name, username, cid, type, org, stat, dob, doj, ssn,
	pass;
	int input = 0;
	cout << "\nI am Customer SignUp Page\n";
	cout << "Enter your FullName: ";
	getline(cin, name);
	int space=0,count=0;
	while(count!=name.length()){
	    if(name[count]==' '){
	        space=1;
	    }
	    count++;
	}
	if(!space){
	    cout<<"Please enter a full name.\nPress Enter to Continue...";
	    Customersignup();
	}
	username = User_Name();
	if(username.length()<3){
	    cout<<"Username must be atleast 3 characters long.\n";
	    Customersignup();
	}
	cout << "\nEnter today's date(mm/dd/yyyy): ";
	cin >> doj;
	if(doj.length()!=10 || !isdigit(doj[0]) || !isdigit(doj[1]) ||
	!isdigit(doj[3]) || !isdigit(doj[4]) || !isdigit(doj[6]) || !isdigit(doj[7])
	|| !isdigit(doj[8]) || !isdigit(doj[9])){
	    cout<<"Please Enter a valid date\n";
	    Customersignup();
	}
	cout << "\nChoose your Account Type\n";
	cout << "1)Checking Account\n";
	cout << "2)Savings Account\n";
	cout << "Please enter your choice: ";
	cin >> input;
	if (input == 1)
	{
		type = "Checking";
	}
	else if (input == 2)
	{
		type = "Savings";
	}
	else
	{
		cout << "Invalid value. Try again.\n";
		Customersignup();
	}

	cout << "\nEnter your Organization: ";
	cin >> org;
	cout << "\nEnter your Date of Birth(mm/dd/yyyy): ";
	cin >> dob;
	if(dob.length()!=10 || !isdigit(dob[0]) || !isdigit(dob[1]) ||
	!isdigit(dob[3]) || !isdigit(dob[4]) || !isdigit(dob[6]) || !isdigit(dob[7])
	|| !isdigit(dob[8]) || !isdigit(dob[9])){
	    cout<<"Please Enter a valid date\n";
	    Customersignup();
	}
	cout << "\nEnter your Social Security Number: ";
	cin >> ssn;
	if(ssn.length()!=9 ||!isdigit(ssn[0]) || !isdigit(ssn[1]) || !isdigit(ssn[2]) ||
	!isdigit(ssn[3]) || !isdigit(ssn[4]) || !isdigit(ssn[5]) || !isdigit(ssn[6]) || !isdigit(ssn[7])
	|| !isdigit(ssn[8])) {
	    cout<<"Please Enter a valid SSN\n";
	    Customersignup();
	}
	pass = CPassword();
	cid = to_string(Nums);
	stat = "Open";
	vector<string> newa = { cid,
		username,
		name,
		type,
		org,
		stat,
		dob,
		doj,
		ssn,
		pass
	};

	Accounts.push_back(newa);
	vector<float> zero = { 0 };

	vector<string> welc = { "Welcome" };

	Inbox.push_back(welc);
	Transactions.push_back(zero);
	g_cid = Nums - 1;
	Customer CustomerObj = Customer(Nums, username, name, type, org, stat, dob, doj, ssn, pass);
	Nums++;
	Active++;
	CustomerObj.CustomerMenu();
}

void Customersignin()
{
	string user, pass;
	bool exist = 0;
	int x = 0;
	if (Active > 0)
	{
		cout << "Enter the username: ";
		cin >> user;
		for (int i = 0; i < Accounts.size(); i++)
		{
			if (user == Accounts[i][1] && Accounts[i][5] != "Changed" && Accounts[i][5] != "Blocked")
			{
				g_cid = i;
				exist = 1;
				break;
			}
		}

		if (!exist)
		{
			cout << "User does not exist.";
			cin.ignore();
			cin.clear();
			main_menu();
		}
		else
		{
			cout << "Enter the Password: ";
			cin >> pass;
			if (pass == Accounts[g_cid][9])
			{
				g_cid--;
				Customer CustomerObj = Customer(user, pass);
				CustomerObj.CustomerMenu();
			}
			else
			{
				cout << "Wrong Password. Please try again.\n";
				cin.ignore();
				cin.clear();
				main_menu();
			}
		}
	}
	else
	{
		cout << "There are no Active users. SignUp first.";
		main_menu();
	}
}

string APassword()
{
	string pass;
	cout <<
		"Enter Desired Password (1 lower, 1 upper, 1 digit, 6+ characters): ";
	cin >> pass;
	int l = pass.length();
	bool lower = false, upper = false, digit = false;
	for (int i = 0; i < l; i++)
	{
		if (islower(pass[i]))
			lower = true;
		if (isupper(pass[i]))
			upper = true;
		if (isdigit(pass[i]))
			digit = true;
	}

	if (lower && upper && digit && (l >= 6))
	{
		return pass;
	}
	else
	{
		cout <<
			"Password is too weak. Enter at least 1 Uppercase letter, 1 Lowercase letter, 1 Digit and have at least 6 characters\n";
		Adminsignin();
	}

	return "0";
}

void Adminsignin()
{
	string adm_user, adm_pass;
	if (AdminAccount[1][0] != "None" && AdminAccount[1][1] != "None")
	{
		cout << "Enter the username: ";
		cin >> adm_user;
		cout << "Enter the password: ";
		cin >> adm_pass;

		if (AdminAccount[1][0] != adm_user)
		{
			cout << "Wrong Username.\nPlease try again.\n";
			main_menu();
		}

		if (AdminAccount[1][1] != adm_pass)
		{
			cout << "Wrong Password.\nPlease try again.\n";
			main_menu();
		}
	}
	else
	{
		cout << "There is no Admin Account.\n";
		cout << "Enter preferred username: ";
		cin >> adm_user;
		AdminAccount[1][0] = adm_user;
		AdminAccount[1][1] = APassword();
		cout << "Account Created.\n";
	}

	Admin AdminObj = Admin(adm_user, AdminAccount[1][1]);
	AdminObj.AdminMenu();
}

void main_menu()
{
	int input;
	cout << "\n1)Admin SignIn\n";
	cout << "2)Customer SignIn\n";
	cout << "3)Customer SignUp\n";
	cout << "4)Exit\n";
	cout << "\nPlease enter your choice: ";
	cin >> input;
	switch (input)
	{
		case 1:
			Adminsignin();
		case 2:
			Customersignin();
		case 3:
			Customersignup();
		case 4:
			cout << "Good Bye\n";
			ofile <<encrypt<string> (AdminAccount[1][0]) << ' ' <<encrypt<string> (AdminAccount[1][1]) << endl;
			for (int i = 0; i < Accounts.size() - 1; i++)
			{
				for (int j = 0; j < Accounts[i].size(); j++)
				{
					ofile <<encrypt<string> (Accounts[i + 1][j]) << ' ';
				}

				ofile << endl;
				for (int j = 0; j < Inbox[i].size(); j++)
				{
					ofile <<encrypt<string> (Inbox[i][j]) << ' ';
				}

				ofile << endl;
				for (int j = 0; j < Transactions[i].size(); j++)
				{
					ofile <<encrypt<float> (Transactions[i][j]) << ' ';
				}

				ofile << endl;
			}

			exit(0);
		default:
			cout << "Please make a valid choice\n";
			main_menu();
	}
}

int main()
{
	string line;
	string val;
	float transact_val;
	int count = 1;
	while (getline(ifile, line))
	{
		vector<string> acc;
		vector<string> inb;
		vector<float> trans;
		vector<string> array_str = split(line);
		if (count == 1)
		{
			AdminAccount[1][0] = array_str[0];
			AdminAccount[1][1] = array_str[1];
		}
		else if (count % 3 == 2)
		{
			for (int i = 0; i < array_str.size(); i++)
			{
				val = decrypt<string> (array_str[i]);
				acc.push_back(val);
				if (val == "Changed")
				{
					Changed++;
					Active--;
				}

				if (val == "Blocked")
				{
					Active--;
				}
			}

			Accounts.push_back(acc);
			Nums++;
			Active++;
		}
		else if (count % 3 == 0)
		{
			for (int i = 0; i < array_str.size(); i++)
			{
				val = decrypt<string> (array_str[i]);
				inb.push_back(val);
			}

			Inbox.push_back(inb);
		}
		else
		{
			for (int i = 0; i < array_str.size(); i++)
			{
				transact_val = decrypt<float> (array_str[i]);
				trans.push_back(transact_val);
			}

			Transactions.push_back(trans);
		}

		count++;
	}

	ofile.open("Accountslist.txt");
	main_menu();
	return 0;
}