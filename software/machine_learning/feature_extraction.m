function output = feature_extraction(windows, yVal, subjNum, numChannels)
output = table(length(windows));
for i=1:length(windows)
    data = windows{i};
    output.yVal = yVal;
    output.subjNum = subjNum;
    N = length(data);
    for channel = 1:numChannels
        label = sprintf("IEMG%s",channel);
        output.(label) = sum(abs(data));

        label = sprintf("MAV%s",channel);
        output.(label) = (1/N) * sum(abs(data));

        label = sprintf("SSI%s",channel);
        output.(label) = sum(data.^2);

        label = sprintf("RMS%s",channel);
        output.(label) = sqrt((1/N) * sum(data.^2));

        label = sprintf("VAR%s",channel);
        output.(label) = (1/(N-1)) * sum(data.^2);

        label = sprintf("MYOP%s",channel);
        dataIdx = (data> 3*std(data));
        output.(label) = (1/N) * sum(abs(dataIdx));

        label = sprintf("WL%s",channel);
        output.(label) = sum(data(2:end) - data(1:end-1));

        label = sprintf("DAMV%s",channel);
        output.(label) = (1/(N-1)) * sum(abs(data(2:end) - data(1:end-1)));

        label = sprintf("M2%s",channel);
        output.(label) = sum((data(2:end) - data(1:end-1)).^2);

        label = sprintf("DVARV%s",channel);
        output.(label) = (1/(N-2)) * sum((data(2:end) - data(1:end-1)).^2);

        label = sprintf("DASDV%s",channel);
        output.(label) = sqrt((1/(N-1)) * sum((data(2:end) - data(1:end-1)).^2));

        label = sprintf("WAMP%s",channel);
        dataIdx = (data> 3*std(data));
        output.(label) = sum(abs(dataIdx(2:end) - dataIdx(1:end-1)));

        label = sprintf("mean%s",channel);
        output.(label) = mean(data);
    end
end


end