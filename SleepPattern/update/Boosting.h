#include<stdio.h>
#include"SVM.h"
// The function of Boosting(using SVM), implment the algorithm of PCA ,SVM and Boosting
// Input:
// data: new data
// level: index of level(level 1: separate lateral and supine; level 2: separate left lateral and right lateral)
//
// Output:
// predict value: 1(supine) or -1(lateral)
//
// Print:
// time cost: the time cost of running one SVM, in ms

#define num_svm 10

int Boosting(float x_test[136], int level)
{
	int i, t;
	float res = 0;
	// Alpha
	float alpha[num_svm];
	char path[30];
	sprintf(path, "./models%d/boosting_alphas", level);
	FILE *f_boosting_alpha = fopen(path, "r");
	for(i = 0; i < num_svm; i++)
	{
		fscanf(f_boosting_alpha, "%f", &alpha[i]);
	}
	for(t = 1; t <= num_svm; t++)
	{
		float yi = SVM(x_test, t);
		res = res + yi * alpha[t - 1];
	}
	if(res > 0)
		return 1;
	if(res < 0)
		return -1;
}