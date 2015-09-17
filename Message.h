#include<iostream>
using namespace std;

 class Message
{

public:
	Message(string n,string s,string m, int i)
	{
		name = n;
		subject = s;
		msg = m;
		index = i;
	}

	~Message(){};


	string getName()
	{
		return name;
	}

	string getSubject()
	{
		return subject;
	}

	string getMsg()
	{
		return msg;
	}

	void toString()
	{
		cout << "Name: " << name;
		cout << " Subject: " << subject;
		cout << " Index: " << index;
		cout << " Length: " << msg.size() << endl;
		//cout << " Msg: " << msg << endl;
	}

	int getIndex()
	{
		return index;
	}

private:
	string name;
	string subject;
	string msg;
	int index;


};