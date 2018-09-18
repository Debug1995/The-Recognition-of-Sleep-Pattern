#include<stdio.h>
#include<math.h>
#include<time.h>
// The function of one Support Vector Machine(SVM), implment the algorithm of PCA and SVM, models are loaded by read from local files, all model files locate in the model document
// Input:
// data: new data
// index: index of model(for Boosting)
// level: index of level(level 1: separate lateral and supine; level 2: separate left lateral and right lateral)
//
// Output:
// predict value: 1(supine) or -1(lateral)
//
// Print:
// time cost: the time cost of running one SVM, in ms

#define col_pca 20

int SVM(float x_test[136], int index, int level){
	// time stamp
	time_t t_start, t_end;
	t_start = clock();
	//----------------------------------------------------------------------------
	// Parameters
	int i, j, k;
	int r, c;
	int col_data = 136;
	// 1 PCA
	//int col_pca = 20;
	float pca_coef[col_data][col_pca];
	float pca_mean[col_data];
	float x_test_pca[col_pca];
	char path[30];
	//----------------------------------------------------------------------------
	// 1.1 Load model
	FILE *f_pca_coef;
	FILE *f_pca_mean;

	sprintf(path, "./models%d/pca_coef", level);
	f_pca_coef = fopen(path, "r");
	sprintf(path, "./models%d/pca_mean", level);
	f_pca_mean = fopen(path, "r");	

	for(r = 0; r < col_data; r++)
	{
		fscanf(f_pca_mean, "%f", &pca_mean[r]);
		for(c = 0; c < col_pca; c++)
		{
			fscanf(f_pca_coef, "%f", &pca_coef[r][c]);
		}
	}
	fclose(f_pca_coef);
	fclose(f_pca_mean);
	// printf("pca load done\n");
	//----------------------------------------------------------------------------
	// 1.2 Modify data
	for(j = 0; j < col_pca; j++)
	{
		x_test_pca[j] = 0;
		for(k = 0; k < col_data; k++)
		{
			x_test_pca[j] += (x_test[k] - pca_mean[k]) * pca_coef[k][j];
		}
	}
	// printf("pca done\n");
	// 2 SVM
	// 2.1 Standardization
	float sv_sigma[col_pca];
	float sv_mean[col_pca];
	float x_test_stan[col_pca];
	//----------------------------------------------------------------------------
	// 2.1.1 Load model
	FILE *f_sv_sigma;
	FILE *f_sv_mean;

	sprintf(path, "./models%d/sv_sigma%d", level, index);
	f_sv_sigma = fopen(path, "r");
	sprintf(path, "./models%d/sv_mean%d", level, index);
	f_sv_mean = fopen(path, "r");
	
	for(r = 0; r < col_pca; r++)
	{
		fscanf(f_sv_sigma, "%f", &sv_sigma[r]);
		fscanf(f_sv_mean, "%f", &sv_mean[r]);
	}
	fclose(f_sv_sigma);
	fclose(f_sv_mean);
	// printf("svm standardization load done\n");
	//----------------------------------------------------------------------------
	// 2.1.2 Standardize data
	for(j = 0; j < col_pca; j++)
	{
		x_test_stan[j] = (x_test_pca[j] - sv_mean[j]) / sv_sigma[j];
	}
	// printf("svm standardization done\n");
	// 2.2 Support vectors
	int num_sv;
	// float sv[num_sv][col_pca];
	// int sv_labels[num_sv];
	// float sv_alphas[num_sv];
	float sv_bias;
	float sv_lambda;
	float sv_scores;
	//----------------------------------------------------------------------------
	// 2.2.1 Load model
	FILE *f_sv;
	FILE *f_sv_labels;
	FILE *f_sv_alphas;
	FILE *f_sv_bias;
	FILE *f_sv_lambda;

	sprintf(path, "./models%d/sv%d", level, index);
	f_sv = fopen(path, "r");
	sprintf(path, "./models%d/sv_labels%d", level, index);
	f_sv_labels = fopen(path, "r");
	sprintf(path, "./models%d/sv_alphas%d", level, index);
	f_sv_alphas = fopen(path, "r");	
	sprintf(path, "./models%d/sv_bias%d", level, index);
	f_sv_bias = fopen(path, "r");
	sprintf(path, "./models%d/sv_lambda%d", level, index);
	f_sv_lambda = fopen(path, "r");			

	fscanf(f_sv_bias, "%f", &sv_bias);
	fscanf(f_sv_lambda, "%f", &sv_lambda);
	fscanf(f_sv, "%d", &num_sv);

	float sv[num_sv][col_pca];
	int sv_labels[num_sv];
	float sv_alphas[num_sv];

	for(r = 0; r < num_sv; r++)
	{
		fscanf(f_sv_labels, "%d", &sv_labels[r]);
		fscanf(f_sv_alphas, "%f", &sv_alphas[r]);
		for(c = 0; c < col_pca; c++)
		{
			fscanf(f_sv, "%f", &sv[r][c]);
		}
	}
	fclose(f_sv);
	fclose(f_sv_labels);
	fclose(f_sv_alphas);
	fclose(f_sv_bias);
	fclose(f_sv_lambda);
	// printf("svm load done\n");
	//----------------------------------------------------------------------------
	// 2.2.2 Implement model
	sv_scores = sv_bias;
	for(j = 0; j < num_sv; j++)
	{
		float EXP = 0;
		for(k = 0; k < col_pca; k++)
		{
			EXP += (sv[j][k] - x_test_stan[k]) * (sv[j][k] - x_test_stan[k]);
		}
		EXP = EXP * (-1) * sv_lambda;
		sv_scores += sv_alphas[j] * sv_labels[j] * exp(EXP);
	}
	// printf("svm done\n");
	t_end = clock();
	//printf("The time cost is %f seconds \n", difftime(t_end, t_start)/CLOCKS_PER_SEC);
	if(sv_scores > 0)
		return 1;
	if(sv_scores < 0)
		return -1;
}