% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %
% This script converts a .wav IR file to be readable by the glass     %
% harpsichord application. The resulting file should be placed in the %
% same folder as the executable                                       %
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %

clear ans file fileID fileWithPath formatSpec fs instrumentBody path;
close all;
clc;

% Get file from user
[file, path] = uigetfile('*.wav');
fileWithPath = append(path,file);

% Open audio file
[instrumentBody, fs] = audioread(fileWithPath); % random guitar body IR
instrumentBody = instrumentBody(:,1);

% Export file
formatSpec = '%4.8f\n';
fileID = fopen('body.txt', 'w');

fprintf(fileID,formatSpec,instrumentBody);
fprintf("Done\n");