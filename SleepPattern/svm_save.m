function [ success ] = svm_save( SVMModel, index )
% Use this function to save svm model when running boosting, parameters includes:
% 1. sv_mean, sv_sigma: standardize support vectors
% 2. sv: all support vectors
% 3. sv_labels: the label of support vector
% 4. sv_alphas, sv_bias, sv_lambda: parameters for Gaussian Kernel
% Input:
% SVMModel: the structure of the svm model, contains all parameters
% index: the index of this SVMModel, used as prefix in file names
% important parameter:
col_pca = 20;
% 
success = false;
sv_mean = SVMModel.Mu;
sv_sigma = SVMModel.Sigma;
sv = SVMModel.SupportVectors;
sv_label = SVMModel.SupportVectorLabels;
sv_alpha = SVMModel.Alpha;

[num_sv, col_sv] = size(sv);
sv_bias = SVMModel.Bias;
lambda = 1/(SVMModel.KernelParameters.Scale)^2;

% write data
file = sprintf('./models/sv_mean%d',index);
f_sv_mean = fopen(file, 'w');

file = sprintf('./models/sv_sigma%d',index);
f_sv_sigma = fopen(file, 'w');

file = sprintf('./models/sv%d',index);
f_sv = fopen(file, 'w');

file = sprintf('./models/sv_labels%d',index);
f_sv_label = fopen(file, 'w');

file = sprintf('./models/sv_alphas%d',index);
f_sv_alpha = fopen(file, 'w');

file = sprintf('./models/sv_bias%d',index);
f_sv_bias = fopen(file, 'w');

file = sprintf('./models/sv_lambda%d',index);
f_sv_lambda = fopen(file, 'w');

for i = 1:col_pca
    fprintf(f_sv_mean, '%f ', sv_mean(1,i));
    fprintf(f_sv_sigma, '%f ', sv_sigma(1,i));
end

% save the num of sv
fprintf(f_sv, '%d\n', num_sv);

for r = 1:num_sv
    fprintf(f_sv_label, '%d ', sv_label(r,1));
    fprintf(f_sv_alpha, '%f ', sv_alpha(r,1));
    for c = 1:col_pca
        fprintf(f_sv, '%f ', sv(r,c));
    end
    fprintf(f_sv, '\n');
end
fprintf(f_sv_bias, '%f', sv_bias);
fprintf(f_sv_lambda, '%f', lambda);

% close file
fclose(f_sv_mean);
fclose(f_sv_sigma);
fclose(f_sv);
fclose(f_sv_label);
fclose(f_sv_alpha);
fclose(f_sv_bias);
fclose(f_sv_lambda);
success = true;
end

