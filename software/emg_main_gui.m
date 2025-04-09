% Main Application GUI for EMG Device
function emg_main_gui()
    % App-wide settings (persistent across mode changes)
    appData.movementList = {'fist', 'peace', 'thumbs_up'};
    appData.useValidation = false;
    appData.saveEnabled = false;

    % Create Main GUI
    fig = figure('Name', 'EMG Control Panel', 'Position', [300 300 300 200], 'MenuBar', 'none');
    uicontrol('Style', 'text', 'String', 'Select Mode:', 'Position', [100 150 100 30], 'FontSize', 12);
    
    uicontrol('Style', 'pushbutton', 'String', 'Normal Mode', 'Position', [50 100 200 30], 'Callback', @(~,~)run_normal_mode());
    uicontrol('Style', 'pushbutton', 'String', 'Training Mode', 'Position', [50 60 200 30], 'Callback', @(~,~)run_training_mode(appData));
    
    uicontrol('Style', 'pushbutton', 'String', 'Training Settings', 'Position', [50 20 200 30], 'Callback', @(~,~)open_training_settings());

    % --- Normal Mode Runner ---
    function run_normal_mode()
        run('normal_mode_script.m');
    end

    % --- Training Mode Runner ---
    function run_training_mode(appData)
        assignin('base', 'movementList', appData.movementList);
        assignin('base', 'useValidation', appData.useValidation);
        assignin('base', 'saveEnabled', appData.saveEnabled);
        run('training_mode_gui.m');
    end

    % --- Callback to update appData in base workspace from settings ---
    function open_training_settings()
        settingsFig = figure('Name', 'Training Settings', 'Position', [500 400 300 300], 'MenuBar', 'none');

        % Movement list input
        uicontrol('Style', 'text', 'Position', [20 240 260 30], 'String', 'Enter movements (comma-separated):');
        mvmtEdit = uicontrol('Style', 'edit', 'Position', [20 210 260 30], 'String', strjoin(appData.movementList, ', '));

        % Validation checkbox
        valCheck = uicontrol('Style', 'checkbox', 'Position', [20 170 260 30], 'String', 'Enable Validation (adds _v to filenames)', 'Value', appData.useValidation);

        % Save checkbox
        saveCheck = uicontrol('Style', 'checkbox', 'Position', [20 140 260 30], 'String', 'Enable Saving to Files', 'Value', appData.saveEnabled);

        uicontrol('Style', 'pushbutton', 'String', 'Save', 'Position', [100 60 100 40], 'Callback', @save_settings);

        function save_settings(~,~)
            rawStr = get(mvmtEdit, 'String');
            appData.movementList = strtrim(strsplit(rawStr, ','));
            appData.useValidation = get(valCheck, 'Value');
            appData.saveEnabled = get(saveCheck, 'Value');
            close(settingsFig);
        end
    end
end
