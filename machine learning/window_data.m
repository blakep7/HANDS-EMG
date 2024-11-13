function [windows, winStim, winRep] = window_data(emg, stimulus, repetition, winsize, wininc)
    % Written by Noah Marosok 
    % 
    % based on work done by:
    % Chan ADC, Green GC, "Myoelectric control development toolbox", 30th Conference of the Canadian Medical & Biological Engineering Society, Toronto, Canada, M0100, 2007.
    % http://www.sce.carleton.ca/faculty/chan/matlab/myoelectric%20control%20development%20toolbox.pdf
    %
    % This function only performs windowing on the input signals 
    % It validates each window based on unique stimulus and repetition vals
    %
    % Inputs:
    %   emg       - EMG data (can be multichannel)
    %   stimulus  - Corresponding stimulus labels (int8)
    %   repetition- Corresponding repetition labels
    %   winsize   - Size of each window (samples)
    %   wininc    - Increment between consecutive windows (samples)
    %
    % Outputs:
    %   windows   - Cell array containing the valid windows from emg
    %   winStim   - Vector of stimulus labels for each valid window
    %   winRep    - Vector of repetition labels for each valid window

    numwin = floor((size(emg, 1) - winsize) / wininc) + 1;
    nSignals = size(emg, 2);


    windows = cell(numwin, 1);
    winStim = zeros(numwin, 1);
    winRep = zeros(numwin, 1);
    validWindowCount = 0;

    
    for winInd = 1:numwin
        startIdx = (winInd - 1) * wininc + 1;
        endIdx = startIdx + winsize - 1;

        % extract current data
        curWin = emg(startIdx:endIdx, :);
        curStimWin = stimulus(startIdx:endIdx);
        curRepWin = repetition(startIdx:endIdx);

        % check for uniqueness
        if numel(unique(curStimWin)) == 1 && numel(unique(curRepWin)) == 1
            % store window and corresponding labels
            validWindowCount = validWindowCount + 1;
            windows{validWindowCount} = curWin;
            winStim(validWindowCount) = curStimWin(1);
            winRep(validWindowCount) = curRepWin(1);
        end
    end

    % remove unused space
    windows(validWindowCount + 1:end) = [];
    winStim(validWindowCount + 1:end) = [];
    winRep(validWindowCount + 1:end) = [];
end
