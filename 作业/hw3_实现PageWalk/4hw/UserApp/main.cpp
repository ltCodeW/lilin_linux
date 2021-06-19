#include <iostream>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "../PrintingDriver/IoCtlSupport.h"

using namespace std;

int main()
{
	int fd = 0;
	int ret;

	string toSend = "hello driver";

	fd = open("/dev/msg_printer", O_RDWR);
	if(fd < 0)
	{
		cout << "open error" << endl;	
		return -1;
	}

	ret = write(fd, toSend.c_str(), toSend.size());	
	if(ret < 0)
	{
		cout << "write error" << endl;	
		return -1;
	}
	
	close(ret);
	
	return 0;
}
