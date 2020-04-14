Input_folder = 'C:\Users\Ngo Minh Nhut\Desktop\matlab\chupanh\'; % folder with big images
Output_folder = 'C:\Users\Ngo Minh Nhut\Desktop\matlab\chupanh\';
D = dir([Input_folder '*.jpg']);
Inputs = {D.name}';
Outputs = Inputs; % preallocate
for k = 1:length(Inputs)
    X = imread([Input_folder Inputs{k}]);
    Image = rgb2gray(imread([Input_folder Inputs{k}]));
    his = adapthisteq(Image,'clipLimit',0.02,'Distribution','rayleigh');
    %resizedImg = imresize(Image, [12 12]);
    idx = k; % index number
    Outputs{k} = regexprep(Outputs{k}, 'big', ['small_' num2str(idx)]);
    imwrite(his, [Output_folder Outputs{k}],'jpg')
end