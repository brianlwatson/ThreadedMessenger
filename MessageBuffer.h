#include <queue>
#include "server.h"
using namespace std;

class MessageBuffer
{
		
	private:
	
		int server_;
		string cache_;
		string request;
		char* buf_;
		int client_socket;
		
	public:
	
		MessageBuffer()
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
		
		string getCache()
		{
			return cache_;
		}
		
		void setCache(string c)
		{
			cache_ = c;
		}
		
		void setBuf(char* buf)
		{
			buf_  = buf;
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
