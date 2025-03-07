data = load('S1_E1_A1.mat'); 

[windows, y_output] = window_data(data.emg, data.restimulus, data.rerepetition, winSize=400, winInc=20);
raw_signal = windows{1}(:,2);


%scale the data to match ADC range
adc_min = -2.85714;  
adc_max = 7.39796;
normalized_signal = (raw_signal - adc_min) / (adc_max - adc_min);

%Q1.23 format
word_length = 24;  
fraction_length = 23;  
fix_data = fi(normalized_signal, true, word_length, fraction_length);  % Signed 24-bit

% convert to int representation
int24_data = int32(round(double(fix_data) * 2^(fraction_length)));

% value fit in 24bit signed range
int24_data = min(max(int24_data, -2^23), 2^23 - 1);


%export
fid = fopen('emg_data_24bit.bin', 'wb');
fwrite(fid, int24_data, 'int32');  
fclose(fid);
