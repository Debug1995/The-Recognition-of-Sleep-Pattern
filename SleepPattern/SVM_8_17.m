%%
clear;
% 1. Load Data
% 17 * 8 = 136
% ZC
% supine
supine_zc = csvread("supine_8_17_zc.csv");
supine_zc_x = supine_zc(:,2:137);
supine_zc_y = supine_zc(:,1);
% lateral
lateral_zc = csvread("lateral_8_17_zc.csv");
lateral_zc_x = lateral_zc(:,2:137);
lateral_zc_y = lateral_zc(:,1);

% HAO
% supine
supine_hao = csvread("supine_8_17_hao.csv");
supine_hao_x = supine_hao(:,2:137);
supine_hao_y = supine_hao(:,1);
% lateral
lateral_hao = csvread("lateral_8_17_hao.csv");
lateral_hao_x = lateral_hao(:,2:137);
lateral_hao_y = lateral_hao(:,1);

% Data Modify
% ZC
x_zc = [supine_zc_x; lateral_zc_x];
y_zc = [supine_zc_y; lateral_zc_y];

% HAO
x_hao = [supine_hao_x; lateral_hao_x];
y_hao = [supine_hao_y; lateral_hao_y];

% Combine data
supine_x = [supine_zc_x; supine_hao_x];
supine_y = [supine_zc_y; supine_hao_y];

lateral_x = [lateral_zc_x; lateral_hao_x];
lateral_y = [lateral_zc_y; lateral_hao_y];

% 
% x_data = [x_zc; x_hao];
% y_data = [y_zc; y_hao];

x_data = [supine_x; lateral_x];
y_data = [supine_y; lateral_y];

% seperate train and test
N = length(x_data);
num_train = 452;
rand_num = randperm(N);
index_train = sort(rand_num(:,1:num_train));
index_test = sort(rand_num(:,num_train + 1:N));

x_train = x_data(index_train,:);
y_train = y_data(index_train,:);
x_test = x_data(index_test,:);
y_test = y_data(index_test,:);
%%
% 2. PCA
col_pca = 20;
col_data = 136;
[coef, score, latent] = pca(x_train);
mean_data = mean(x_train);
x_pca_train = score(:,1:col_pca);
x_pca_test = (x_test - mean_data)*coef(:,1:col_pca);

x_train = x_pca_train;
x_test = x_pca_test;

% write data
f_pca_mean = fopen('pca_mean.txt', 'w');
f_pca_coef = fopen('pca_coef.txt', 'w');
for i = 1:col_data
    fprintf(f_pca_mean, '%f ', mean_data(1,i));
end

for r = 1:col_data
    for c = 1:col_pca
        fprintf(f_pca_coef, '%f ', coef(r,c));
    end
    fprintf(f_pca_coef, '\n');
end
fclose(f_pca_mean);
fclose(f_pca_coef);
%%
% 3. SVM
% using function of svmstruct = svmtrain and pre = svmclassify
SVMModel = fitcsvm(x_train, y_train,'Standardize',true,'KernelFunction','RBF','KernelScale','auto','BoxConstraint',Inf);
% SVMModel = fitcsvm([supine_x; lateral_x], y_data,'Standardize',true,'KernelFunction','RBF','KernelScale','auto','BoxConstraint',Inf);
%%
% 4. SVM update and test
% only use params
% no using function predict
% Standarization
sv_mean = SVMModel.Mu;
sv_sigma = SVMModel.Sigma;
x_test_standardize = (x_test - sv_mean)./(sv_sigma);

sv = SVMModel.SupportVectors;
num_sv = length(sv);
sv_label = SVMModel.SupportVectorLabels;
sv_alpha = SVMModel.Alpha;
sv_bias = SVMModel.Bias;
lambda = 1/(SVMModel.KernelParameters.Scale)^2;  %% correct, can get the same result of the scores
% lambda = 0.4;
x_test_sv = sv_bias*ones(length(x_test_standardize), 1);
for i = 1:length(sv)
    for j = 1:length(x_test_standardize)
        x_test_sv(j,1) = x_test_sv(j,1) + sv_alpha(i,1)*sv_label(i,1)*exp(-lambda*sum((x_test_standardize(j,:) - sv(i,:)).^2));
    end
end
svm_save(SVMModel, 0);
%%
%5. The res of SVM
[pre_label, pre_score] = predict(SVMModel, x_test);
correct = 0;
for i = 1:(N - num_train)
    if(pre_label(i,1) == y_test(i,1))
        correct = correct + 1;
    end
end
correct/(N - num_train)

correct = 0;
for i = 1:(N - num_train)
    if(x_test_sv(i,1) * y_test(i,1) > 0)
        correct = correct + 1;
    end
end
correct/(N - num_train)

% CVSVMModel = crossval(SVMModel);
% classLoss = kfoldLoss(CVSVMModel);
% 1 - classLoss


