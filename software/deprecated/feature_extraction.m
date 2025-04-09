function output = feature_extraction(windows, yVal, subjNum, numChannels)
output = table();
for i=1:length(windows)
    data = windows{i};
    output.yVal = yVal;
    output.subjNum(:) = subjNum;
    N = length(data);
    for channel = 1:numChannels
        label = sprintf("IEMG%d",channel);
        output.(label)(i) = sum(abs(data(:,channel)));

        label = sprintf("MAV%d",channel);
        output.(label)(i) = (1/N) * sum(abs(data(:,channel)));

        label = sprintf("SSI%d",channel);
        output.(label)(i) = sum(data(:,channel).^2);

        label = sprintf("RMS%d",channel);
        output.(label)(i) = sqrt((1/N) * sum(data(:,channel).^2));

        label = sprintf("VAR%d",channel);
        output.(label)(i) = (1/(N-1)) * sum(data(:,channel).^2);

        label = sprintf("MYOP%d",channel);
        dataIdx = (data(:,channel)> 2*std(data(:,channel)));
        output.(label)(i) = (1/N) * sum(abs(dataIdx));

        label = sprintf("WL%d",channel);
        output.(label)(i) = sum(data(2:end,channel) - data(1:end-1,channel));

        label = sprintf("DAMV%d",channel);
        output.(label)(i) = (1/(N-1)) * sum(abs(data(2:end,channel) - data(1:end-1,channel)));

        label = sprintf("M2%d",channel);
        output.(label)(i) = sum((data(2:end,channel) - data(1:end-1,channel)).^2);

        label = sprintf("DVARV%d",channel);
        output.(label)(i) = (1/(N-2)) * sum((data(2:end,channel) - data(1:end-1,channel)).^2);

        label = sprintf("DASDV%d",channel);
        output.(label)(i) = sqrt((1/(N-1)) * sum((data(2:end,channel) - data(1:end-1,channel)).^2));

        label = sprintf("WAMP%d",channel);
        dataIdx = (data(:,channel)> 2*std(data(:,channel)));
        output.(label)(i) = sum(abs(dataIdx(2:end) - dataIdx(1:end-1)));

        label = sprintf("mean%d",channel);
        output.(label)(i) = mean(data(:,channel));
    end
end


end