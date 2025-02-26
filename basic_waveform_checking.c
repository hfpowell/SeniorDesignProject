
//program start
//wire [1000:0] expected_waveform;
//wire [1000:0] user_input;	

//wire [10:0] i = 10b'0;



//the graphs will be translated into simple 1s and 0s
/*int expected_waveform[1000];
int user_input[1000];

//populate expected array

*/
fig = figure;
set(fig, 'WindowButtonDownFcn', @(~,~) disp('Click detected')); % Dummy function to register clicks

% Initialize the 10x100 matrix
data = zeros(10, 100);

for row = 1:10  % 10 seconds (1 row per second)
    tic; % Start timing for the second
    
    for col = 1:100  % 100 samples per second
        % Check if the mouse is clicked
        clickState = strcmp(get(fig, 'SelectionType'), 'normal'); % 1 if clicked, 0 if not
        
        % Store the value
        data(row, col) = clickState;
        
        % Ensure 10ms delay to get 100 samples per second
        while toc < (col / 100)
            pause(0.0001); % Fine control to maintain timing
        end
    end
    
    % Reset the SelectionType to avoid persistent detection
    set(fig, 'SelectionType', 'open');
end

% Close the figure after data collection
close(fig);

% Display the collected data
disp(data);






//program to make the expected_waveform more tollerant

//the following program looks for groups of 1s that indicate high points on the graph

/*
int[] increaseTollerance(int[] waveform)
{
	for(int i = 0; i < waveform.c
}
*/