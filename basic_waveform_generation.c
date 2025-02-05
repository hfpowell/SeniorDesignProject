//Basic rythm generation software (initial testing is just quarter notes and half notes in a 4/4 bar at 100bpm

/*
1.) Check where you are in bar to see what notes/rests can be generated
2.) Randomly generate note/rest
3.) Move to next available beat not occupied by a note/rest



The "bar" will be an array of length 16, this allows for a maximum of 16th notes to be used in the program
*/

int bar[16] = {-1}; //check this line in case of errors
int bar_length = sizeof(bar) / sizeof(bar[0]);
//in the bar -1 will be an unfilled section, 0  will be a rest, and 1 will be a note


//make a type called note, things of type note will have a value(how much of the bar they take up), and a bool for if they are a rest or not


//The following function takes in an array bar and its length and outputs the ammount of free space in that bar
int AvailableBeats(int[] bar, int bar_length)
{
	int space = 0;
	for(int i = 0; i < bar_length; i++)
	{
		if(bar_length[i] == -1)
		{
			space++;
		}
	}
	return space;
}

int RandomNoteGenerator(int space)
{
	//all available random values [8, 4, 2]
	//half note: 8, quarter note: 4, eighth note: 2
	//rests are negative
	
	int notes[3] = [2, 4, 8]
	int chosen_note = 0;
	int random_number;
	
	if(space >= 8)
	{
		random_number = (rand()%3) - 1;
	}
	if(space < 8 && space >= 4)
	{
		random_number = (rand()%2) - 1;
	}
	else
	{
		random_number = 0;
	}
	chosen_note = notes[random_number];

	//chooses if it is a rest or a note
	//later make it less likely for a rest to appear if the previous note was a rest
	rest_or_note = rand()%2;
	if(rest_or_note == 1) //if rest_or_note == 1 it will be a rest
	{
		chosen_note * -1;
	}
	
	return chosen_note;
}