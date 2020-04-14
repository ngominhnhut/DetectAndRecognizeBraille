clear all;
clc;
%%
folder = uigetdir;
img_files=dir(fullfile(folder, '*.jpg'));
%input = zeros(100,1);

input = [];
for i=1:length(img_files)
    temp = [];
    FullFileName = fullfile(folder, img_files(i).name);
   % Image = rgb2gray(imread(FullFileName));
   Image = imread(FullFileName);
    resizedImg = imresize(Image, [28 20]);
    for j=1:size(resizedImg,2)
        temp =  [temp;resizedImg(:,j)];
    end
    input = [input temp]; 

    clear temp;
end
input = double(input);
%% Test 
load('net_final.mat');
Result = net(input);