% Senior Design Project - Rhythm Focus Aid
% Author: Harrison Powell
% Co-editors: Ian Flores, Romina Gonzales

% This file is a prototype test for a 

% Create a figure to capture mouse clicks
fig = figure;
holdState = false; % Variable to track whether the mouse is being held down

% Set up event handlers for mouse press and release
set(fig, 'WindowButtonDownFcn', @(~,~) setappdata(fig, 'holdState', true));
set(fig, 'WindowButtonUpFcn', @(~,~) setappdata(fig, 'holdState', false));
setappdata(fig, 'holdState', false); % Initialize hold state


%create a randomized array (replace with Ian's generation code)
signal = randi([0, 1], 1, 10);

imagesc(signal); % Display matrix as an image
colormap(gray); % Set colormap to grayscale
colorbar; % Add a colorbar to show 0 (black) and 1 (white)
xlabel('Signal to Match');
title('Seconds');

disp(signal);

pause(2);

%pause to review the signal
holdState = getappdata(fig, 'holdState');
while(holdState~=1)
    pause(.01);
    %waitfor(fig, 'SelectionType');
    holdState = getappdata(fig, 'holdState');
end

%data averages (fill with averages in for loop)
averages = zeros(1,10);

% Initialize the 10x100 matrix
data = zeros(10, 100);

%array of pass/fail
passFail=zeros(1,10);

%user input signal
inputSignal=zeros(1,10);


for row = 1:10  % 10 seconds (1 row per second)
    tic; % Start timing for the second
    disp(row);

    %temp variable used to store averages in the array
    tempAvg=0;

    for col = 1:100  % 100 samples per second
        % Get the current hold state
        holdState = getappdata(fig, 'holdState');
        
        % Store the value (1 if held, 0 otherwise)
        data(row, col) = holdState;

        tempAvg= tempAvg+holdState;

        % Ensure 10ms delay to get 100 samples per second
        while toc < (col / 100)
            pause(0.0001); % Fine control to maintain timing
        end
    end
    averages(row)=tempAvg/100;
    
    %if the user clicked for more than half the time
    if(averages(row)>=.5)
        inputSignal(row)=1;
    else
        inputSignal(row)=0;
    end
end

% Close the figure after data collection
close(fig);

% Display the collected data
disp('Data');
disp(data);

disp('Averages');
disp(averages);


%--------- visualization of clicks ---------------

% -- randomized data to match --
figure; % Create a new figure
imagesc(signal); % Display matrix as an image
colormap(gray); % Set colormap to grayscale
colorbar; % Add a colorbar to show 0 (black) and 1 (white)
xlabel('Signal to Match');
title('Seconds');


% -- clicked info
%figure; % Create a new figure
imagesc(data); % Display matrix as an image
colormap(gray); % Set colormap to grayscale
colorbar; % Add a colorbar to show 0 (black) and 1 (white)
xlabel('Time Steps (100 per second)');
ylabel('Seconds (10 Rows)');
title('Mouse Click Data Visualization (10x100)');

%interpret data:
%compare each by removing leading zeros and then matching rhythm

%find value where 1s start in the expected signal
i=1;
while(signal(i)==0 && i<=length(signal))
    i=i+1;
end

%start counting at that value, comparing
%      average of the 100 data point clicks to the expected signal

%       (NOTE: averages and user input should always start with a 1 because
%       the user can't tell if the signal has started until it "vibrates"
%       or sends a 1 signal, so we compare the start of the input to the
%       position in the oroginal signal that has the first one, and attach
%       the leading zeros to the end, since it will be the same result)
%       
%       EXAMPLE: if the input signal was 01101, the user input should be
%       11010. The averages is used to actually store the average of the
%       user input, and then this gets translated to the user input signal
%       based on if it
%       
iter = 1;

%start from first 1
for int =i:length(signal)

    %compare
    if(signal(int) == inputSignal(iter))
        passFail(iter)= 1;
    else
        passFail(iter)=0;
    end

    iter = iter+1;
end

%pick up from the start (if it wasn't already the start)
if(i~=1)
    for int=1:i-1
    
        %compare
        if(signal(int) == inputSignal(iter))
            passFail(iter)= 1;
        else
            passFail(iter)=0;
        end
    
        iter = iter+1;
    
    end
end

disp('Signal');
disp(signal);

disp('User Input:');
disp(inputSignal);

disp(' ');
disp('Pass/Fail:');
disp(passFail);
