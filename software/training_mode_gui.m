% Training Mode GUI for EMG Chunk Collection
function training_mode_gui()
    movementList = evalin('base', 'movementList');
    useValidation = evalin('base', 'useValidation');
    saveEnabled = evalin('base', 'saveEnabled');

    % Ask for number of repetitions and delay
    prompt = {'Enter number of repetitions per movement:', 'Enter delay (sec) between movements:'};
    dlgtitle = 'Training Setup';
    dims = [1 35];
    definput = {'3', '5'};
    answer = inputdlg(prompt, dlgtitle, dims, definput);
    if isempty(answer), return; end
    reps = str2double(answer{1});
    delaySec = str2double(answer{2});
    if isnan(reps) || reps <= 0 || isnan(delaySec) || delaySec < 0, return; end

    % Generate randomized sequence of movements
    allMovements = repmat(movementList, 1, reps);
    allMovements = allMovements(randperm(length(allMovements)));

    % Serial Port Config
    port = 'COM7';
    baudRate = 921600;
    syncWord = uint32(0x53594E43);
    s = serialport(port, baudRate, "Parity", "even", "Timeout", 10);
    flush(s);

    samplesPerChunk = 1024;
    numChannels = 4;
    dataBytes = samplesPerChunk * numChannels * 4;

    % GUI
    fig = figure('Name', 'Training Mode', 'Units', 'normalized', 'Position', [0.2 0.3 0.6 0.5]);
    promptText = uicontrol('Style', 'text', 'Units', 'normalized', 'Position', [0.1 0.85 0.8 0.1], 'FontSize', 14, 'HorizontalAlignment', 'center');
    sgtitle('Waiting...');

    t = (0:samplesPerChunk-1) / 2000;
    for ch = 1:numChannels
        subplot(2,2,ch);
        plots(ch) = plot(t, zeros(1, samplesPerChunk));
        title(['Channel ', num2str(ch)]);
        grid on;
    end

    % Function to get one synced chunk (display only if saveFlag is false)
    function emg_chunk = get_chunk(saveFlag, fileName)
        % Wait for sync
        found_sync = false;
        while ~found_sync && isvalid(s)
            if s.NumBytesAvailable >= 4
                sync = read(s, 1, 'uint32');
                if sync == syncWord
                    found_sync = true;
                else
                    read(s, 1, 'uint8');
                end
            end
        end

        while s.NumBytesAvailable < dataBytes + 4
            pause(0.001);
        end

        raw_data = read(s, samplesPerChunk * numChannels, 'single');
        read(s, 1, 'uint32'); % discard class_id
        emg_chunk = reshape(raw_data, [numChannels, samplesPerChunk])';

        % Plot chunk
        for ch = 1:numChannels
            set(plots(ch), 'YData', emg_chunk(:, ch));
        end
        drawnow;

        % Save if enabled and flagged
        if saveEnabled && saveFlag
            f = fopen(fileName, 'a');
            interleaved = reshape(emg_chunk.', 1, []);
            fprintf(f, '%f,', interleaved(1:end-1));
            fprintf(f, '%f\n', interleaved(end));
            fclose(f);
        end
    end

    for m = 1:length(allMovements)
        if ~isvalid(fig), break; end

        currentMovement = strtrim(allMovements{m});
        if useValidation
            fileName = [currentMovement, '_v.csv'];
        else
            fileName = [currentMovement, '.csv'];
        end

        % Display prompt and capture chunk to save
        promptText.String = ['Perform: ', strrep(currentMovement, '_', '\_')];
        pause(0.5); % brief delay so prompt appears before read
        get_chunk(true, fileName);

        % Delay period where chunks are just displayed but discarded
        delayStart = tic;
        while toc(delayStart) < delaySec && isvalid(fig)
            get_chunk(false, '');
        end
    end

    if isvalid(s)
        clear s;
    end
    if isvalid(fig)
        close(fig);
    end
end