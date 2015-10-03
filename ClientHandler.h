#include <queue>
#include "server.h"
using namespace std;

class ClientHandler
{
		
	private:
	
		int server_;
		string cache;
		string request;
		char* buf_;
		int client_socket;
		
	public:
	
		ClientHandler()
		{
			buf_ = new char[1024+1];
		}
	
		string getRequest()
		{
			return request;
		}
		
		void setRequest(string r)
		{
			request = r;
		}
	
		int getServer()
		{
			return server_;
		}
		
		void setServer(int ser)
		{
			ser = server_;
		}
		
	
		char* getBuf()
		{
			return buf_;
		}
		
		void setBuf(char* b)
		{
			buf_ = b;
		}

		string getCache()
		{
			return cache;
		}
		
		void setCache(string c)
		{
			cache = c;
		}
				
		int getSocket()
		{
			return client_socket;
		}
	
		int setSocket(int c)
		{
			client_socket = c;
		}
	

		


	

};
