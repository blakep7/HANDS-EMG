% Normal Mode GUI-Compatible Script (Live Serial Plotting with Classification Viewer and Auto-Scaling Toggle)
function normal_mode_script()
    clearvars -except stopCapture; clc; close all;

    % Serial Port Configuration
    port = 'COM5';
    baudRate = 921600;
    syncBytes = uint32(0x53594E43);
    s = serialport(port, baudRate, "Parity", "even");
    flush(s);

    % Sampling Parameters
    Fs = 2000;
    dt = 1/Fs;
    bufferSize = 10000;
    captureSize = 1000;
    bytesPerPacket = 5 * 4;

    % Initialize buffer
    chData = zeros(bufferSize, 4);
    timeVec = (0:bufferSize-1) * dt;

    % Filter Settings
    global hpEnabled lpEnabled notchEnabled hpFreq lpFreq notchFreq autoScaleEnabled simulateKeys
    hpEnabled = false; lpEnabled = false; notchEnabled = false;
    hpFreq = 5; lpFreq = 100; notchFreq = 60;
    autoScaleEnabled = true;
    simulateKeys = false;

    % Stop control
    global stopCapture;
    stopCapture = false;

    % Classification map (update as needed)
    classMap = containers.Map('KeyType', 'uint32', 'ValueType', 'any');
    classMap(3) = struct('name', 'Rest', 'image', '', 'keyboardSimKey', 'up');
    classMap(1) = struct('name', 'Wrist Flexion', 'image', 'images/wrist-flexion.png', 'keyboardSimKey', 'up');
    classMap(2) = struct('name', 'Wrist Extension', 'image', 'images/wrist-extension.png', 'keyboardSimKey', 'down');
    classMap(4) = struct('name', 'Fist Clench', 'image', 'images/fist-clench.png', 'keyboardSimKey', 'L');

    % === Plotting Window ===
    fig = figure('Name', 'Live Serial Plot', 'Units', 'normalized', 'Position', [0.1 0.2 0.8 0.7]);
    subplot(2,2,1); ch1Plot = plot(timeVec, chData(:,1)); title('Channel 1'); grid on; ch1Ax = gca;
    subplot(2,2,2); ch2Plot = plot(timeVec, chData(:,2)); title('Channel 2'); grid on; ch2Ax = gca;
    subplot(2,2,3); ch3Plot = plot(timeVec, chData(:,3)); title('Channel 3'); grid on; ch3Ax = gca;
    subplot(2,2,4); ch4Plot = plot(timeVec, chData(:,4)); title('Channel 4'); grid on; ch4Ax = gca;
    
    % Set Initial
    if ~autoScaleEnabled
        ylim(ch1Ax, [-5e-3 5e-3]);
        ylim(ch2Ax, [-5e-3 5e-3]);
        ylim(ch3Ax, [-5e-3 5e-3]);
        ylim(ch4Ax, [-5e-3 5e-3]);
    else
        ylim(ch1Ax, 'auto');
        ylim(ch2Ax, 'auto');
        ylim(ch3Ax, 'auto');
        ylim(ch4Ax, 'auto');
    end

    % === Classification Output Window ===
    classFig = figure('Name', 'Live Classification', 'Units', 'normalized', 'Position', [0.75 0.75 0.2 0.2]);
    classAx = axes('Parent', classFig);
    classImage = imshow(ones(500,500,3));
    classTitle = title(classAx, 'Waiting...', 'FontSize', 14);

    % === Control Panel ===
    ctrlFig = figure('Name', 'Control Panel', 'Units', 'normalized', 'Position', [0.3 0.05 0.4 0.15]);
    ctrlPanel = uipanel('Parent', ctrlFig, 'Units', 'normalized', 'Position', [0 0 1 1], 'Title', 'Controls');

    uicontrol(ctrlPanel, 'Style', 'checkbox', 'String', 'High Pass', 'Units', 'normalized', 'Position', [0.01 0.1 0.1 0.8], 'Value', 1, 'Callback', @(src,~) toggleFilter(src, 'hp'));
    uicontrol(ctrlPanel, 'Style', 'edit', 'String', num2str(hpFreq), 'Units', 'normalized', 'Position', [0.11 0.1 0.05 0.8], 'Callback', @(src,~) setFilterFreq(src, 'hp'));
    uicontrol(ctrlPanel, 'Style', 'checkbox', 'String', 'Low Pass', 'Units', 'normalized', 'Position', [0.18 0.1 0.1 0.8], 'Value', 1, 'Callback', @(src,~) toggleFilter(src, 'lp'));
    uicontrol(ctrlPanel, 'Style', 'edit', 'String', num2str(lpFreq), 'Units', 'normalized', 'Position', [0.28 0.1 0.05 0.8], 'Callback', @(src,~) setFilterFreq(src, 'lp'));
    uicontrol(ctrlPanel, 'Style', 'checkbox', 'String', 'Notch', 'Units', 'normalized', 'Position', [0.35 0.1 0.1 0.8], 'Callback', @(src,~) toggleFilter(src, 'notch'));
    uicontrol(ctrlPanel, 'Style', 'edit', 'String', num2str(notchFreq), 'Units', 'normalized', 'Position', [0.45 0.1 0.05 0.8], 'Callback', @(src,~) setFilterFreq(src, 'notch'));
    uicontrol(ctrlPanel, 'Style', 'checkbox', 'String', 'Auto Scale', 'Units', 'normalized', 'Position', [0.62 0.1 0.1 0.8], 'Callback', @(src,~) toggleAutoScaleRealtime(src, ch1Ax, ch2Ax, ch3Ax, ch4Ax));
    uicontrol(ctrlPanel, 'Style', 'checkbox', 'String', 'Simulate Keyboard', 'Units', 'normalized', 'Position', [0.62 0.1 0.1 0.8], 'Callback', @(src,~) simulateKeyboardPresses(src));
    uicontrol(ctrlPanel, 'Style', 'pushbutton', 'String', 'Stop', 'Units', 'normalized', 'Position', [0.75 0.1 0.1 0.8], 'Callback', @(~,~) stopSerial());

    flush(s);
    disp('Starting continuous capture...');

    while isvalid(s) && ~stopCapture
        try
            samples = zeros(captureSize, 4);
            class_id = zeros(captureSize, 1);
            i = 1;

            while i <= captureSize
                if s.NumBytesAvailable >= bytesPerPacket
                    raw = read(s, 6, 'uint32');
                    if raw(1) == syncBytes
                        samples(i, :) = [typecast(uint32(raw(2)), 'single'), ...
                                         typecast(uint32(raw(3)), 'single'), ...
                                         typecast(uint32(raw(4)), 'single'), ...
                                         typecast(uint32(raw(5)), 'single')];
                        class_id(i, :) = uint32(raw(6));
                        i = i + 1;
                    else
                        read(s, 1, 'uint8');
                    end
                end
            end

            chData = [chData(captureSize+1:end, :); samples];
            filteredData = applyFilters(chData, Fs);

            set(ch1Plot, 'YData', filteredData(:,1));
            set(ch2Plot, 'YData', filteredData(:,2));
            set(ch3Plot, 'YData', filteredData(:,3));
            set(ch4Plot, 'YData', filteredData(:,4));

            drawnow limitrate nocallbacks;

            idx = find(class_id, 1);
            if ~isempty(idx)
                first_nonzero = class_id(idx);
            else
                first_nonzero = 0;
            end

            if isKey(classMap, first_nonzero)
                classInfo = classMap(first_nonzero);
                if exist(classInfo.image, 'file')
                    img = imread(classInfo.image);
                    set(classImage, 'CData', img);
                end
                set(classTitle, 'String', classInfo.name);

                if simulateKeys
                    sim_keyboard(classInfo.keyboardSimKey);
                end
            end

        catch ME
            disp(['Error: ', ME.message]);
        end
    end

    if isvalid(s), clear s; end
    if isvalid(fig), close(fig); end
    if isvalid(ctrlFig), close(ctrlFig); end
    if isvalid(classFig), close(classFig); end
    disp('Serial Connection Closed');
end

function toggleFilter(src, type)
    global hpEnabled lpEnabled notchEnabled
    switch type
        case 'hp', hpEnabled = src.Value;
        case 'lp', lpEnabled = src.Value;
        case 'notch', notchEnabled = src.Value;
    end
end

function setFilterFreq(src, type)
    global hpFreq lpFreq notchFreq
    val = str2double(src.String);
    if isnan(val) || val <= 0, return; end
    switch type
        case 'hp', hpFreq = val;
        case 'lp', lpFreq = val;
        case 'notch', notchFreq = val;
    end
end

function toggleAutoScaleRealtime(src, ch1Ax, ch2Ax, ch3Ax, ch4Ax)
    global autoScaleEnabled
    autoScaleEnabled = logical(src.Value);
    if ~autoScaleEnabled
        ylim(ch1Ax, [-5e-3 5e-3]);
        ylim(ch2Ax, [-5e-3 5e-3]);
        ylim(ch3Ax, [-5e-3 5e-3]);
        ylim(ch4Ax, [-5e-3 5e-3]);
    else
        ylim(ch1Ax, 'auto');
        ylim(ch2Ax, 'auto');
        ylim(ch3Ax, 'auto');
        ylim(ch4Ax, 'auto');
    end
end

function simulateKeyboardPresses(src)
    global simulateKeys
    simulateKeys = logical(src.Value);
end

function data = applyFilters(data, Fs)
    global hpEnabled lpEnabled notchEnabled hpFreq lpFreq notchFreq
    if hpEnabled
        [b,a] = butter(2, hpFreq / (Fs/2), 'high');
        data = filtfilt(b, a, data);
    end
    if lpEnabled
        [b,a] = butter(2, lpFreq / (Fs/2), 'low');
        data = filtfilt(b, a, data);
    end
    if notchEnabled
        wo = notchFreq/(Fs/2); bw = wo/35;
        [b,a] = iirnotch(wo, bw);
        data = filtfilt(b, a, data);
    end
end

function stopSerial()
    global stopCapture
    stopCapture = true;
    disp('Stop button pressed. Cleaning up...');
end