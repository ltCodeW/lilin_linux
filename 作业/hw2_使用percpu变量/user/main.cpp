#include <iostream>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>

using namespace std;

int main()
{
	int fd = 0;
	int ret;

	string toSend = "hello driver";

	if((fd = open("/dev/adder", O_RDWR)) < 0)
	{
		cerr << strerror(errno) << endl;	
		return -1;
	}

	int nums[2] = {125,125};
	int sum =0;

	ret = write(fd, nums, sizeof(nums));
    if(ret < 0)
    {
        cout << "failed to write the message\n" << endl;
        return errno;
    }
	ret = read(fd, &sum, sizeof(sum));
	if(ret < 0)
    {
        cout << "failed to read the message\n" << endl;
        return errno;
    }
	cout << "The sum is： " << sum << endl;

	close(fd);

	return 0;
}
