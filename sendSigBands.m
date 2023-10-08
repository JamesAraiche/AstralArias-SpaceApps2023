%Modifiables
clear;
numOfGroups = 10; %amount of rows and cols
ampDivider = 650000*10000;%change eventually

%-------------------
hcube = hypercube('frt00002fa6_07_if168j_mtr3.img', 'frt00002fa6_07_if168j_mtr3.hdr');
cubeSize = size(hcube.DataCube);
xSize = cubeSize(1);
xPixelSize = floor(xSize/numOfGroups);
ySize = cubeSize(2);
yPixelSize = floor(ySize/numOfGroups);
fSize = cubeSize(3);

soundFrequencies = waveToFreq(hcube.Wavelength);

opts = databaseConnectionOptions("native","MySQL");
opts = setoptions(opts, ...
    'DataSourceName', "astralAriasSource", ...
    'DatabaseName', "astralArias", ...
    'Server', "34.130.73.94", ...
    'PortNumber', 3306);

datasource = "astralAriasSource";
username = "root";
password = "space23";
conn = mysql(datasource,username,password);

status = testConnection(opts, username, password);
coordsTableName = "coordsBandAmount";
bandTableName = "sigBands";
%coordsBandAmountTable = sqlread(conn,tablename);

coordKey = 1;
bandSum = 0;
xRow = zeros(numOfGroups^2, 1);
yRow = zeros(numOfGroups^2, 1);
bandSumRow = zeros(numOfGroups^2, 1);
ampSum=0;
fRow = zeros(489*numOfGroups^2, 1);
ampRow = zeros(489*numOfGroups^2, 1);
KeyRowForBands = zeros(489*numOfGroups^2, 1);

for x=1:numOfGroups
    for y=1:numOfGroups
        bandSum=0;
        for f=1:fSize
            ampSum = sum(hcube.DataCube((x-1)*xPixelSize+1:x*xPixelSize,(y-1)*yPixelSize+1:y*yPixelSize,f), "all")/ampDivider; %Gets sum of all amplitudes for that pixel group and frequency 
            if ampSum>= 0.01
                bandSum = bandSum+1;
                ampRow((x-1)*numOfGroups+(y-1)*numOfGroups+f, 1) = ampSum;
                fRow((x-1)*numOfGroups+(y-1)*numOfGroups+f, 1) = soundFrequencies(f); %CHANGE THIS TO FREQUENCY
                KeyRowForBands((x-1)*numOfGroups+(y-1)*numOfGroups+f, 1) = coordKey;
            end
        end
        %Coordinate end
        xRow((x-1)*numOfGroups+y, 1) = x;
        yRow((x-1)*numOfGroups+y, 1) = y;
        bandSumRow((x-1)*numOfGroups+y, 1) = bandSum;
        coordKey = coordKey+1;
    end
end

coordsBandAmountTable = table();
coordsBandAmountTable.x = xRow;
coordsBandAmountTable.y = yRow;
coordsBandAmountTable.bandAmount = bandSumRow;

sigBandTable = table();
sigBandTable.frequency = fRow;
sigBandTable.amplitude = ampRow;
sigBandTable.coordKey = KeyRowForBands;
removeCond = sigBandTable.("coordKey") == 0;
sigBandTable(removeCond, :) = [];

%disp(coordsBandAmountTable);
sqlwrite(conn, coordsTableName, coordsBandAmountTable);
sqlwrite(conn, bandTableName, sigBandTable);

function f = waveToFreq(waveLengths)
    maxSound = 4000; %max frequency for sound
    minSound = 100; %min f for sound
    f = (waveLengths-361)/7.3*((maxSound-minSound)/489)+minSound;
end