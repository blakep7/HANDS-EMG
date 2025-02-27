%main script for creating dataset for ML deployment
%for use in conjunction with window_data.m function
%when utilizing this function, the following arguments must be taken into account:
%
%   window_data(emg, stimulus, repetition, windowSize, windowIncrement)
%
%   emg - the raw emg data in a 12 column matrix
%   stimulus - the labeled stimulus data
%   repetiton - the labeled repetition data
%   windowSize - the size of the data window (set to 400)
%   windowIncrement - the number of data points in between windows (set to 20)
%
%   for this script, please download Ninapro DB2 dataset for all subjects
%   at the following link: https://ninapro.hevs.ch/instructions/DB2.html
%
% when data is generated it will be saved into the dataForTFlite.csv
% this will only contain data for the four channels of interest, and the python script
% will filter out the gestures of interest. Rerun the data analysis script *FIXME*
% to find additional channels or gestures of interest (in descending order)

clear all;
clc;
subjects = 1:40;
subjects_label = string(subjects);



%remove the following line and uncomment the next line to generate a .csv with all channels of data. Useful for data analysis
%on channel correlation
channels_of_interest = [8 3 2 7];
%uncomment below for all 12 channels
%channels_of_interest = 1:12;


for i = subjects
    subject_name = sprintf("S%s_E1_A1.mat",subjects_label(i))
    data = load(subject_name);

    emg = data.emg;
    emg = emg(:,channels_of_interest);

    %**FIXME** script is in progress




end




