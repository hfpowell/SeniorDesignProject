#define LED_PIN D10  // Pin for LED output
#define BUTTON_PIN D9 //Pin for Button (can change)


const int bar_length = 32;
int bar[bar_length];  // Rhythm pattern

//Sample variables (start from matlab values):

//  Beats per minute
const int BPM = 60;

//  total number of samples we collect per rhythm
const int samples = 100;

//  total number of seconds we collect samples for
//      (might be the same as bar_length, FIGURE OUT)
const int secondsCount = 10;

//  averages of each sample per second
int averages[secondsCount];

//pass/fail array (might be redundant)
int passFail[secondsCount];

//  user input samples
int userInput[samples];

//boolean telling the code the button has been pressed
bool buttonPressed;

//takes the user-input-filled array and checks it against the generated array
void rhythmCheck(){

    int firstHigh = 0;
    //find where first rhythm is 1
    while(bar(firstHigh)==0 && firstHigh<secondsCount){
        firstHigh=firstHigh+1;
    }

    //if there is no first zero, throw error
    if(firstHigh>=secondsCount){
        Serial.println("ERROR: rhythm is full of 0s");
    }

    //start from first high and compare to start of user input
    for(int i =firstHigh;i<secondsCount;i++){

    }

    //if firstHigh isnt at the beginning,
    //  finish the remaining user input checks
    if(firstHigh!=0){
        for(int i = 0;i<firstHigh;i++){

        }
    }

  

}

//controls timing for the button input ISR
void getUserInput(){
    
}

//ISR for button being pressed
void ButtonPressed(){

}



// Function to compute available beats
int availableBeats()
{
    int space = 0;
    for (int i = 0; i < bar_length; i++)
    {
        if (bar[i] == -1)
            space++;
    }
    return space;
}

// Function to generate a random note or rest
int randomNoteGenerator(int space)
{
    int notes[] = {2, 4, 8}; // Eighth, quarter, half note durations
    int chosen_note = 0;
    int random_number;

    if (space >= 8)
        random_number = random(0, 3);
    else if (space >= 4)
        random_number = random(0, 2);
    else
        random_number = 0;

    chosen_note = notes[random_number];
    if (random(0, 2) == 1)
        chosen_note = -chosen_note; // Negative value for rest

    return chosen_note;
}

// Function to update the array
void updateArray(int startIndex, int numValues, int newValue)
{
    if (newValue < 0)
        newValue = 0;
    else
        newValue = 1;

    for (int i = startIndex; i < startIndex + numValues && i < bar_length; i++)
    {
        bar[i] = newValue;
    }
}

void printArray()
{
    Serial.println("=== NEW RYTHM ===");
    for (int i = 0; i < bar_length; i++)
    {
        Serial.print(bar[i]);
    }
    Serial.println();
}

void generateRhythm()
{
    for (int i = 0; i < bar_length; i++)
        bar[i] = -1;

    int i = 0;
    while (i < bar_length)
    {
        int note = randomNoteGenerator(availableBeats());
        updateArray(i, abs(note), note);
        i += abs(note);
    }
}

void playRhythm()
{
      printArray();
    for (int i = 0; i < bar_length; i++)
    {
        digitalWrite(LED_PIN, bar[i]);
        delay(200);  // Adjust delay for timing (200ms per step)
    }
}

//waits for the ISR to run and set the value
void waitForButton(){
    while(!buttonPressed){

    }
}


//setup
void setup()
{
    pinMode(LED_PIN, OUTPUT);

    //set button pin to input

    Serial.begin(115200);
    
    
    //generateRhythm();
    Serial.println("TEST");

    //attach ISR

    //reset buttonPressed
    buttonPressed = false;


}

//loop function
void loop()
{
    //print array (error checking code - remove in final product)
    printArray();

    //generate pattern & check to make sure it has a 1
    generateRhythm();

    //CHECK FOR ALL 0's

    //play rhythm
    playRhythm();

    //enter wait state
    waitForButton();

    //check rhythm
    rhythmCheck();

    /*
    playRhythm();
    delay(1000);  // Wait before repeating the pattern
    generateRhythm();
    */
}
