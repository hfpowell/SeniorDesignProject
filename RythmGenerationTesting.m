%Current Changes: Trying to make the generated waveforms more interesting
%Made the code cover 2 bars to allow for more interesting patterns to show
%up
%Also Made a function to convert the matrix to a function of unit steps

% Basic rhythm generation software (initial testing with eighth, quarter, and half notes in a 4/4 bar at 100 bpm)
bar = -1 * ones(1, 32);  % -1 represents a null value
bar_length = length(bar);

% Function to compute the available beats
function space = AvailableBeats(bar)
    space = sum(bar == -1);
end

% Function to generate a random note or rest
function chosen_note = RandomNoteGenerator(space)
    % Available note durations: half note = 8, quarter note = 4, eighth note = 2
    notes = [2, 4, 8]; 
    chosen_note = 0;
    
    % Generate a random note depending on available space
    if space >= 8
        random_number = randi([1, 3]);  
    elseif space >= 4
        random_number = randi([1, 2]);  
    else
        random_number = 1;  % Default to smallest note 
    end
    
    chosen_note = notes(random_number);
    rest_or_note = randi([0, 1]);  % Randomly choose 0 for note, 1 for rest
    if rest_or_note == 1
        chosen_note = -chosen_note;  % Negative value for a rest
    end
end

function arr_out = UpdateArray(arr, numValues, startIndex, newValue)

    if(newValue < 0)
        newValue = 0;
    else
        newValue = 1;
    end

    if startIndex < 1 || startIndex > length(arr)
        error('Start index out of array bounds.');
    end
    if startIndex + numValues - 1 > length(arr)
        error('Number of values exceeds array length.');
    end
    
    % disp("NewValue: " + newValue);
    % disp("StartIndex: " + startIndex);
    % disp("NumValues: " + numValues);
    arr(startIndex:startIndex + numValues - 1) = newValue;

    arr_out = arr;
    
end



function PrintArray(array)
    array_length = length(array);
    for i = 1:array_length
        if array(i) < 1
            fprintf('0');
        else
            fprintf('1');
        end
    end
end

%Takes the perfect "output waveform" and generates a number of user inputs
%that are going to be used to test the rythm checking code
function new_array =  DistortArrayShift(array)
    new_array = -1 * ones(1, 32);
    array_length = length(array);

    %y(t) = x(t + 3)

end

function new_array =  DistortArrayScale(array)
    new_array = -1 * ones(1, 32);
    array_length = length(array);

    %y(t) = x(2t)
end

function new_array =  DistortArrayShiftScale(array)
    new_array = -1 * ones(1, 32);
    array_length = length(array);
    

    %y(t) = x(2t+3)
end

i = 1;
while i < (bar_length + 1)
    note = RandomNoteGenerator(AvailableBeats(bar));
    % disp(bar);
    % disp("note: " + note);
    % disp("i : "+  i);
    % disp("Available Beats: " + AvailableBeats(bar));
    % disp("===============================================================")
    bar = UpdateArray(bar, abs(note), i, note);
    i = i + abs(note);
end

PrintArray(bar);
%disp(bar);


% Initialize plotting variables
x = []; % x-axis points
y = []; % y-axis points

for i = 1:length(bar)
    % Define square wave segment based on bar value
    x_segment = [i - 1, i];
    y_segment = [bar(i), bar(i)];
    
    % Append points to the plot data
    x = [x, x_segment];
    y = [y, y_segment];
    
    % Add vertical transition if not at the end
    if i < length(bar)
        x = [x, i];
        y = [y, bar(i + 1)];
    end
end

% Plot the square wave
clf;
subplot(2,1,1);
plot(x, y, 'LineWidth', 2);
grid on;
xlabel('Place in Bar');
ylabel('Signal');
title('Waveform Generation');
axis([0 length(bar) -0.1 1.1]);


%Converts the Matrix to a step function to make scaling and shifting it
%easier
function stepfunction_out = MatrixToSepFunction(array)
    syms t;
    stepFunction = 0;
    
    lastVal = 0;

    n = length(array);
    for i = 1:n
        if array(i) == 1 && lastVal == 0 %on posedge
            stepFunction = stepFunction + heaviside(t - (i - 1));
        elseif array(i) == 0 && lastVal == 1 %on negedge
            stepFunction = stepFunction - heaviside(t - (i - 1));

        end
        lastVal = array(i);
        
    end

    stepfunction_out = stepFunction;


end

stepfunction = MatrixToSepFunction(bar);

% Display the symbolic step function
disp('Symbolic Step Function:');
disp(stepfunction);

% Plot the step function
subplot(2,1,2);
fplot(stepfunction, [0 bar_length], 'LineWidth', 1.5);
xlabel('Time');
ylabel('Signal');
title('Step Function from Matrix');
grid on;
axis([0 bar_length -0.1 1.1]);
