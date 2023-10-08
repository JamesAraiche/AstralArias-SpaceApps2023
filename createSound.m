%clear;
hcube = hypercube('frt00002fa6_07_if168j_mtr3.img', 'frt00002fa6_07_if168j_mtr3.hdr');
cubeSize = size(hcube.DataCube);
xSize = cubeSize(1)-1055;
ySize = cubeSize(2)-1000;
fSize = cubeSize(3);

cond = 37500000; %condensing ratio
vol = 10^6; %amplitude ratio
%((y-362)/7.3)*((20000-20)/489)+20

fs = 44100; %sampling frequency
Ts = 1/fs; %sampling period
tPixStart = 0;
tPixEnd = 0.25; %length of each pixel's sound
nPixel = tPixStart:Ts:tPixEnd; %time vector for each sound section
nFinal = [];

%value; %to hold the important amplitudes
Fsound=0; %value to hold the new sound frequency

pixelSound=zeros(size(nPixel)); %temp to hold each pixel's sound
finalSound=[]; %To hold the full audio file

for x=300:30:xSize+300
    for y=300:30:ySize+300
        pixelSound = zeros(size(nPixel)); %clear the array for each pixel
        for f=1:fSize
            if hcube.DataCube(y,y,f)>500 %500 is cutoff amplitude for now
                %Fsound = 3*10^8/(hcube.Wavelength(f)*10^(-9)*cond);
                Fsound = ((hcube.Wavelength(f)-361)/7.3)*((4000-20)/489)+20;
                %disp(size(pixelSound));
                temp = (hcube.DataCube(y,y,f)/vol)*sin(2*pi*Fsound*nPixel);
                %disp(size(temp));
                pixelSound = pixelSound+temp;
            end
            disp(f);
        end
        finalSound = cat(2,finalSound,pixelSound);
        tPixStart = tPixEnd; %Set new start time to previous end time
        tPixEnd = tPixEnd+0.25; %Set new end time
        nFinal = cat(2,nFinal, nPixel);
        nPixel = tPixStart:Ts:tPixEnd; %Set new time vector
    end
    disp(x);
end

audiowrite('testSpaceAudio.wav', finalSound, fs);
plot(finalSound);
