% Basic rhythm generation software (initial testing with quarter and half notes in a 4/4 bar at 100 bpm)

% Initialize the bar array (length 16 to allow for 16th notes)
bar = -1 * ones(1, 16);  % -1 represents an unfilled section
bar_length = length(bar);

% Function to compute the available beats
function space = AvailableBeats(bar)
    % Count the number of unfilled sections (-1 values)
    space = sum(bar == -1);
end

% Function to generate a random note or rest
function chosen_note = RandomNoteGenerator(space)
    % Available note durations: half note = 8, quarter note = 4, eighth note = 2
    notes = [2, 4, 8];  % Note durations
    chosen_note = 0;
    
    % Generate a random note depending on available space
    if space >= 8
        random_number = randi([1, 3]);  % Random index between 1 and 3
    elseif space >= 4
        random_number = randi([1, 2]);  % Random index between 1 and 2
    else
        random_number = 1;  % Default to smallest note (index 1)
    end
    
    chosen_note = notes(random_number);
    % Determine if it's a rest (negative value) or a note
    rest_or_note = randi([0, 1]);  % Randomly choose 0 for note, 1 for rest
    if rest_or_note == 1
        chosen_note = -chosen_note;  % Negative value for a rest
    end
end

function arr_out = UpdateArray(arr, numValues, startIndex, newValue)
    % Updates 'numValues' elements in the array 'arr' starting at 'startIndex'
    % Replaces the values with 'newValue'
    
    if(newValue < 0)
        newValue = 0;
    else
        newValue = 1;
    end

    % Error handling for bounds
    if startIndex < 1 || startIndex > length(arr)
        error('Start index out of array bounds.');
    end
    if startIndex + numValues - 1 > length(arr)
        error('Number of values exceeds array length.');
    end
    
    % Update the specified values
    disp("NewValue: " + newValue);
    disp("StartIndex: " + startIndex);
    disp("NumValues: " + numValues);
    arr(startIndex:startIndex + numValues - 1) = newValue;

    arr_out = arr;
    
    % Display updated array (for debugging purposes)
end


% bar = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
% function chosen_note = RandomNoteGenerator(space)
% function space = AvailableBeats(bar)
% function void = UpdateArray(arr, numValues, startIndex, newValue)

%i is the current position in the bar
i = 1;
while i < (bar_length + 1)
    note = RandomNoteGenerator(AvailableBeats(bar));
    disp(bar);
    disp("note: " + note);
    disp("i : "+  i);
    disp("Available Beats: " + AvailableBeats(bar));
    disp("===============================================================")
    bar = UpdateArray(bar, abs(note), i, note);
    i = i + abs(note);
end


disp("++++++++++++++++++++++++++++++++++++++++++++++++++");

disp(bar);


