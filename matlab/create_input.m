clear all;
clc;
%% Convert ppm to jpg
% Select main folder
main_folder = uigetdir;
% get list of subfolders
sub_folders=dir(fullfile(main_folder, '*'));
% get images in each subfolders 
for i=3:length(sub_folders)
    img_files=dir(fullfile(main_folder,sub_folders(i).name, '*.ppm'));
    % Read, resize and write
    for j=1:length(img_files)
        FullFileName = fullfile(main_folder,sub_folders(i).name, img_files(j).name);
        Image = imread(FullFileName);
        resizedImg = imresize(Image, [50 50]);
        outputFullFileName = strrep(lower(FullFileName), '.ppm', '.jpg');
        imwrite(resizedImg, outputFullFileName,'Quality',85);
    end
end

%% create input and target data
% Select main folder
main_folder = uigetdir;
% get list of subfolders
sub_folders=dir(fullfile(main_folder, '*'));
% get images in each subfolders 
input = [];
target = [];
for i=3:length(sub_folders)
    img_files=dir(fullfile(main_folder,sub_folders(i).name, '*.jpg'));
    % Read, change image to column vetor, create target
    for j=1:length(img_files)
        temp1 = []; temp2 = []; 
        FullFileName = fullfile(main_folder,sub_folders(i).name, img_files(j).name);
        %Image = rgb2gray(imread(FullFileName));
        Image = imread(FullFileName);
        resizedImg = imresize(Image, [28 20]);
        for j=1:size(resizedImg,2)
            temp1 =  [temp1;resizedImg(:,j)];
        end
        input = [input temp1]; 
        temp2 = zeros(length(sub_folders)-2, 1);
        temp2(i-2,1) = 1;
        target = [target temp2];
        clear temp;
    end
end
input = double(input);
target = double(target);
save('input.mat','input');
save('target.mat','target');
