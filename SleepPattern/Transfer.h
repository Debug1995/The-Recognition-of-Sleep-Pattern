// This function will transfer data from hex to int
// Input:
// original data: in hex
//
// Output:
// test data: have 136 variables

float *Transfer(char data[17][2]) 
{
	int i, j, k;
	//int col_data = 136;
	static float x_test[136];
	float pic[16][4] = {{0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1}, {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1},
				   	    {1,0,0,0}, {1,0,0,1}, {1,0,1,0}, {1,0,1,1}, {1,1,0,0}, {1,1,0,1}, {1,1,1,0}, {1,1,1,1}};
	//                    8          9          A          B          C          D          E          F
	// data: 17 * 2
	for(i = 0; i < 17; i++)
	{
		for(j = 0; j < 2; j++)
		{
			char hex = data[i][j];
			float *bin;
			if(hex > 57)
				bin = pic[hex - 87];
			else
				bin = pic[hex - 48];
			int index = 8 * i + 4 * j;
			for(k = 0; k < 4; k++) 
			{
				x_test[index + k] = bin[k];
			}
		}
	}
	return x_test;
}