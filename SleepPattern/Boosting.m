%%
clear;
% 1. Load Data
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

x_data = [supine_x; lateral_x];
y_data = [supine_y; lateral_y];

% seperate train and test
[N,M] = size(x_data);
num_train = 352;
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
f_pca_mean = fopen('./models/pca_mean', 'w');
f_pca_coef = fopen('./models/pca_coef', 'w');
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
% 3. Boosting update and test
% choose samples based on identified probability
prob = 1/num_train * ones(1,num_train);
index = (1:num_train);
T = 10;
alpha = zeros(T,1);
eps = zeros(T,1);
test_pre = zeros(N - num_train, 1);
f_boosting_alpha = fopen('./models/boosting_alphas', 'w');
for t = 1:T
    % 1. resample
    index_sample = randsrc(num_train,1,[index;prob]);
    x_train_resample = x_train(index_sample,:);
    y_train_resample = y_train(index_sample,:);
    % 2. learn a classifier
    SVMModel = fitcsvm(x_train_resample,y_train_resample,'Standardize',true,'KernelFunction','RBF','KernelScale','auto','BoxConstraint',Inf);
    svm_save(SVMModel, t);
    % 3. update alpha
    y_pre = predict(SVMModel, x_train);
    eps(t,1) = sum(prob'.*(y_train ~= y_pre));
    alpha(t,1) = 1/2 * log((1-eps(t,1))/eps(t,1));
    % save the alpha of boosting
    fprintf(f_boosting_alpha, '%f ', alpha(t,1));
    test_pre = test_pre + predict(SVMModel, x_test)*alpha(t,1);
    % 4. update probability
    for i = 1:num_train
        prob(1,i) = prob(1,i)*exp(-eps(t,1)*y_pre(i,1)*y_train(i,1));
    end
    prob = prob/sum(prob);   
end
fclose(f_boosting_alpha);
%%
% 4. SVM
% using function of svmstruct = svmtrain and pre = svmclassify
SVMModel = fitcsvm(x_train, y_train,'Standardize',true,'KernelFunction','RBF','KernelScale','auto','BoxConstraint',Inf);
% SVMModel = fitcsvm([supine_x; lateral_x], y_data,'Standardize',true,'KernelFunction','RBF','KernelScale','auto','BoxConstraint',Inf);
%%
% 5. Compare the res of SVM and Boosting
[pre_label, pre_score] = predict(SVMModel, x_test);
correct = 0;
for i = 1:(N - num_train)
    if(pre_label(i,1) == y_test(i,1))
        correct = correct + 1;
    end
end
num_test - correct
correct/(N - num_train)

correct = 0;
for i = 1:(N - num_train)
    if(test_pre(i,1) * y_test(i,1) > 0)
        correct = correct + 1;
    end
end
num_test - correct
correct/(N - num_train)

% CVSVMModel = crossval(SVMModel);
% classLoss = kfoldLoss(CVSVMModel);
% 1 - classLoss


