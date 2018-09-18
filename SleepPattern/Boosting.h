#include<stdio.h>
#include"SVM.h"
// The function of Boosting(using SVM), implment the algorithm of PCA ,SVM and Boosting
// Input:
// data: new data, in hex
//
// Output:
// predict value: 1(supine) or -1(lateral)
//
// Print:
// time cost: the time cost of running one SVM, in ms

#define num_svm 10

int Boosting(float x_test[136])
{
	int i, t;
	float res = 0;
	// Alpha
	float alpha[num_svm];
	FILE *f_boosting_alpha = fopen("./models/boosting_alphas", "r");
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