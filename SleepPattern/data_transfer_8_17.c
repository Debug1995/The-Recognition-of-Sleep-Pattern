#include<stdio.h>
main()
{
	//0: 0
	//1: 1,2,4,8,
	//2: 3,5,6,9,A,C
	//3: 7,B,D,E
	//4: F
	//                 0          1      	 2    	    3     	   4          5          6          7  
	int pic[16][4] = {{0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1}, {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1},
					  {1,0,0,0}, {1,0,0,1}, {1,0,1,0}, {1,0,1,1}, {1,1,0,0}, {1,1,0,1}, {1,1,1,0}, {1,1,1,1}};
	//                 8          9          A          B          C          D          E          F

	//	              0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F
	int weight[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	char file[10];
	scanf("%s", file);
	FILE *f;
	f = fopen(file,"r");
	fseek(f, 0, SEEK_SET);
	char c = getc(f);
	int SIZE = 1;
	int COUNT = 51;

	FILE *fw;
	if(strcmp(file, "supine") == 0)
	{
		fw = fopen("supine_data_8_17.csv", "w");
	}
	else if(strcmp(file, "lateral") == 0)
	{
		fw = fopen("lateral_data_8_17.csv", "w");
	}

	while(!feof(f))
	{
		//printf("%c",c);
		c = getc(f);
		if(c == '\n')
		{
			//printf("\n");
			int points[8][17];
			int points_weight[17] = {0};
			int weighted_sum = 0;
			int weight_sum = 0;
			char input[51] = {""};
			int num = fread(input, SIZE, COUNT, f);
			int i = 0;
			int r = 0, c = 0;
			for(i = 0; i < 51; i += 3)
			{
				//high digit
				if(input[i] > 57)
				{
					points_weight[i/3] += weight[input[i] - 87];
					for(r = 7; r > 3; r--)
					{
						points[r][i/3] = pic[input[i] - 87][7 - r];				
					}
				}
				else
				{
					points_weight[i/3] += weight[input[i] - 48];
					for(r = 7; r > 3; r--)
					{
						points[r][i/3] = pic[input[i] - 48][7 - r];						
					}
				}
				//low digit
				if(input[i + 1] > 57)
				{
					points_weight[i/3] += weight[input[i + 1] - 87];
					for(r = 3; r >= 0; r--)
					{
						points[r][i/3] = pic[input[i + 1] - 87][3 - r];						
					}
				}
				else
				{
					points_weight[i/3] += weight[input[i + 1] - 48];
					for(r = 3; r >= 0; r--)
					{
						points[r][i/3] = pic[input[i + 1] - 48][3 - r];						
					}
				}
				weighted_sum += points_weight[i/3]*(i/3 + 1);
				weight_sum += points_weight[i/3];
			}

			// file write
			if(strcmp(file, "supine") == 0)
			{
				fprintf(fw, "%d,", 1);
				//printf("1");
			}
			else if(strcmp(file, "lateral") == 0)
			{
				fprintf(fw, "%d,", -1);
				//printf("-1");
			}

			// for(i = 0; i < 17; i++)
			// {
			// 	if(i == 16)
			// 	{
			// 		fprintf(fw, "%d\n", points_weight[i]);
			// 		continue;
			// 	}
			// 	fprintf(fw, "%d,", points_weight[i]);
			// }

			for(i = 0; i < 17; i++)
			{
				for(r = 0; r < 8; r++)
				{
					if(i == 16 && r == 7)
					{
						fprintf(fw, "%d\n", points[r][i]);
						continue;
					}
					fprintf(fw, "%d,", points[r][i]);
				}
			}
			fseek(f, 1, SEEK_CUR);
		}
	}
	fclose(f);
	fclose(fw);
}
