clear; clc; close all;

outputFile = fopen('wrist_flexion.csv', 'a');


% --- Serial Port Configuration ---
port = 'COM5';
baudRate = 921600;
syncWord = uint32(0x53594E43); % 'SYNC'

s = serialport(port, baudRate, "Parity", "even", "Timeout", 10);
flush(s);

% --- Chunk Parameters ---
samplesPerChunk = 1024;
numChannels = 4;
bytesPerSample = 4;  % float32
dataBytes = samplesPerChunk * numChannels * bytesPerSample;
totalBytes = 4 + dataBytes + 4; % SYNC + data + class_id

% --- Initialize Storage ---
emg_chunk = zeros(samplesPerChunk, numChannels);
class_id = 0;
tally = 0;

% --- Live Plotting ---
fig = figure('Name', 'EMG Chunk View', 'Units', 'normalized', 'Position', [0.1 0.2 0.8 0.6]);
t = (0:samplesPerChunk-1) / 2000; % Assume 2kHz sampling
for ch = 1:4
    subplot(2,2,ch);
    plots(ch) = plot(t, zeros(1, samplesPerChunk));
    title(['Channel ', num2str(ch)]);
    grid on;
end
sgtitle('Waiting for first chunk...');
drawnow;
% --- Continuous Reading Loop ---
disp("Reading EMG chunks...");
while isvalid(s)
    % --- Wait for SYNC word ---
    found_sync = false;
    while ~found_sync && isvalid(s)
        if s.NumBytesAvailable >= 4
            sync = read(s, 1, 'uint32');
            if sync == syncWord
                found_sync = true;
            else
                % Skip one byte and resync
                read(s, 1, 'uint8');
            end
        end
    end

    if ~isvalid(s), break; end

    % --- Wait for full packet ---
    while s.NumBytesAvailable < dataBytes + 4
        pause(0.001);
    end

    % --- Read EMG data ---
    raw_data = read(s, samplesPerChunk * numChannels, 'single');
    tally = tally + 1;
    % --- Read classification ID ---
    class_id = read(s, 1, 'uint32');

    % --- Reshape to [samples x channels] ---
    emg_chunk = reshape(raw_data, [numChannels, samplesPerChunk])';

    % --- Update plots ---
    for ch = 1:numChannels
        set(plots(ch), 'YData', emg_chunk(:, ch));
    end
    %sgtitle(['Classification: ', num2str(class_id)]);
    sgtitle(['Tally: ', num2str(tally)]);
    drawnow;

    % --- Print interleaved chunk in required format ---
    interleaved = reshape(emg_chunk.', 1, []);  % Interleave as [Ch1_0, Ch2_0, ..., Ch4_1023]
    fprintf(outputFile, '%f,', interleaved(1:end-1));  % Print all but last with comma
    fprintf(outputFile, '%f\n', interleaved(end));     % Print last without comma then newline
end

% --- Cleanup ---
if isvalid(s)
    clear s;
end
disp("Serial connection closed.");

fclose(outputFile);