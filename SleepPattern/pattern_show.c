#include<stdio.h>
main()
{
	//0: 0
	//1: 1,2,4,8,
	//2: 3,5,6,9,A,C
	//3: 7,B,D,E
	//4: F
	char pic[16][4] = {"    ", "   #", "  # ", "  ##", " #  ", " # #", " ## ", " ###",
					   "#   ", "#  #", "# # ", "# ##", "##  ", "## #", "### ", "####"};
	//                   8       9       A       B       C       D       E       F

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
	while(!feof(f))
	{
		printf("%c",c);
		c = getc(f);
		if(c == '\n')
		{
			printf("\n");
			char points[8][17];
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
					//printf("%d  ", input[i] - 87);
				}
				else
				{
					points_weight[i/3] += weight[input[i] - 48];
					for(r = 7; r > 3; r--)
					{
						points[r][i/3] = pic[input[i] - 48][7 - r];						
					}
					//printf("%d  ", input[i] - 48);
				}

				//low digit
				if(input[i + 1] > 57)
				{
					points_weight[i/3] += weight[input[i + 1] - 87];
					for(r = 3; r >= 0; r--)
					{
						points[r][i/3] = pic[input[i + 1] - 87][3 - r];						
					}
					//printf("%d\n", input[i + 1] - 87);
				}
				else
				{
					points_weight[i/3] += weight[input[i + 1] - 48];
					for(r = 3; r >= 0; r--)
					{
						points[r][i/3] = pic[input[i + 1] - 48][3 - r];						
					}
					//printf("%d\n", input[i + 1] - 48);
				}
				weighted_sum += points_weight[i/3]*(i/3 + 1);
				weight_sum += points_weight[i/3];
			}

			int up = 0, down = 0;
			for(r = 0; r < 7; r++)
			{
				for(c = 0; c < 17; c++)
				{
					if(points[r][c] != 0)
					{
						up = r + 1;
						break;
					}
				}
			}
			for(r = 7; r >= 0; r--)
			{
				for(c = 0; c < 17; c++)
				{
					if(points[r][c] != 0)
					{
						down = r + 1;
						break;
					}
				}
			}
			int vertical_center = (up + down)/2;
			int left = 0, right = 0;
			for(i = 0; i < 17; i++)
			{
				if(points_weight[i] != 0)
				{
					left = i + 1;
					break;
				}
			}
			for(i = 16; i >= 0; i--)
			{
				if(points_weight[i] != 0)
				{
					right = i + 1;
					break;
				}
			}
			int horizontal_center = (left + right)/2;
			// int center_area = 0;

			// for(r = -1; r < 2; r++) {
			// 	for(c = -1; c < 2; c++) {
			// 		if(vertical_center - 1 + r >= 0 && vertical_center - 1 + r <= 16 && horizontal_center - 1 + c >= 0 && horizontal_center - 1 + c <= 16)
			// 		{
			// 			if(points[vertical_center - 1 + r][horizontal_center - 1 + c] == '#') center_area++;
			// 		}					
			// 	}
			// }
			int left_down = 0, right_down = 0;
			for(i = -2; i < 3; i++) {
				if(vertical_center - 1 + i >= 0 && vertical_center - 1 + i <= 16 && horizontal_center - 1 + i >= 0 && horizontal_center - 1 + i <= 16)
				{
					if(points[vertical_center - 1 + i][horizontal_center - 1 + i] == '#') right_down++;
				}					
			}
			for(i = -2; i < 3; i++) {
				if(vertical_center - 1 + i >= 0 && vertical_center - 1 + i <= 16 && horizontal_center - 1 - i >= 0 && horizontal_center - 1 - i <= 16)
				{
					if(points[vertical_center - 1 + i][horizontal_center - 1 - i] == '#') left_down++;
				}					
			}

			if(weight_sum == 0)
			{
				printf("BAD DATA\n\n");
				fseek(f, 1, SEEK_CUR);
				continue;
			}

			// print distance
			printf("%d    %d\n", vertical_center, horizontal_center);
			printf("left down = %d    right down = %d\n", left_down, right_down);
			int weight_center = weighted_sum*10/weight_sum;
			int shape_center = (left + right)*10/2;
			printf("%d - %d = %d\n\n", weight_center, shape_center, weight_center - shape_center);
			if(abs(weight_center - shape_center) < 6)
			{
				printf("仰卧\n");
			}

			// print picture
			printf("------------------------------------\n");
			for(r = 0; r < 8; r++)
			{
				printf("|");
				for(i = 0; i < 17; i++)
				{
					printf("%c%c", points[r][i], points[r][i]);
				}
				printf("|\n");
			}
			printf("------------------------------------\n");
			fseek(f, 1, SEEK_CUR);
		}
	}
	fclose(f);
}
