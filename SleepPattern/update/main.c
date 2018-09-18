#include"Transfer.h"
#include"Boosting.h"

main()
{
	char data[17][2] = {"00", "00", "06", "0e", "7e", "7c", "7e", "fe", "7c", "3d", "07", "00", "00", "00", "00", "00", "00"};
	int record[30];
	float *x_test = Transfer(data);
	printf("data transfer done\n");
	int res = Boosting(x_test, 1);
	if(res < 0)
	{
		res = Boosting(x_test, 2);
	}
	printf("%d\n", res);
}