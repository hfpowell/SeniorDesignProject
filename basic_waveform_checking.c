
//program start
//wire [1000:0] expected_waveform;
//wire [1000:0] user_input;	

//wire [10:0] i = 10b'0;



//the graphs will be translated into simple 1s and 0s
int expected_waveform[1000];
int user_input[1000];

int score = 0;

for(int i = 0; i < 1000; i++)
{
	if(expected_waveform[i] == user_input[i])
	{
		score++;
	}
}



//program to make the expected_waveform more tollerant

//the following program looks for groups of 1s that indicate high points on the graph


int[] increaseTollerance(int[] waveform)
{
	for(int i = 0; i < waveform.c
}
