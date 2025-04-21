#define LED_PIN D10  // Pin for LED output
#define BUTTON_PIN D5 //Pin for Button (can change)


const int bar_length = 32;
int bar[bar_length];  // Rhythm pattern

//Sample variables (start from matlab values):

//  Beats per minute
const int BPM = 60;

//  Samples per beat
const int sampPerBeat = 100;

//  total number of samples we collect
const int samples=sampPerBeat*bar_length;


/*
//  total number of seconds we collect samples for
//      (might be the same as bar_length, FIGURE OUT)
//      I BELIEVE THEY ARE THE SAME - THIS IS REDUNDANT/OUTDATED CODE
const int secondsCount = 10;
//*/

//  The delay (in ms) between user input samples
//  msDelay = 60,000 millis / (BPM*samples);
int msDelay;

//  averages of each sample per beat
int averages[bar_length];

//  array holding whether 
int highLow[bar_length];

//pass/fail array (might be redundant)
int passFail[bar_length];

//  user input samples
int userInput[samples];

//boolean telling the code the button has been pressed
bool buttonPressed;

//
bool userInputDetected;

//Millis timer (for test code)
unsigned long MilliTimer;

//takes the user-input-filled array and checks it against the generated array
void rhythmCheck(){

    int firstHigh = 0;
    //find where first rhythm is 1
    while(bar[firstHigh]==0 && firstHigh<bar_length){
        firstHigh=firstHigh+1;
    }

    //if there is no first zero, throw error
    if(firstHigh>=bar_length){
        Serial.println("ERROR: rhythm is full of 0s");
    }

    //start from first high and compare to start of user input
    Serial.println("Pass/Fail output:");
    int counter = 0;
    for(int i =firstHigh;i<bar_length;i++){
        if((bar[i]==1&&averages[counter]>=.5)||(bar[i]==0&&averages[counter]<=.5)){
            passFail[i]=1;
            Serial.print("1");
        }
        else{
            passFail[i]=0;
            Serial.print("0");
        }
        counter++;
    }

    //if firstHigh isnt at the beginning,
    //  finish the remaining user input checks
    if(firstHigh!=0){
        for(int i = 0;i<firstHigh;i++){
            if((bar[i]==1&&averages[counter]>=.5)||(bar[i]==0&&averages[counter]<=.5)){
                passFail[i]=1;
                Serial.print("1");
            }
            else{
                passFail[i]=0;
                Serial.print("0");
            }
            counter++;
        }
    }

  

}

//controls timing for the button input
void getUserInput(){
    int lastSampleTime = millis();

    //For every beat, read in user samples
    //      Should run for 32 seconds
    for(int i=0;i<samples;i++){
        if(digitalRead(BUTTON_PIN)==HIGH){
            userInput[i] = 1;
        }
        else{
            userInput[i] = 0;
        }
        

        /*TEST CODE - REMOVE
        if(millis()-MilliTimer>=1000){
            Serial.print(".");
            MilliTimer=millis();
        }
        //*/

        while(millis()-lastSampleTime<msDelay){
            //looper function
            Serial.print("Looping at ");
            Serial.print(millis());
            Serial.println("Millis");
        }
        //delay(msDelay);

    }
}




// Function to compute available beats
int availableBeats(){
    int space = 0;
    for (int i = 0; i < bar_length; i++)
    {
        if (bar[i] == -1)
            space++;
    }
    return space;
}

// Function to generate a random note or rest
int randomNoteGenerator(int space){
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
void updateArray(int startIndex, int numValues, int newValue){
    if (newValue < 0)
        newValue = 0;
    else
        newValue = 1;

    for (int i = startIndex; i < startIndex + numValues && i < bar_length; i++)
    {
        bar[i] = newValue;
    }
}

void printArray(){
    Serial.println("=== NEW RHYTHM ===");
    for (int i = 0; i < bar_length; i++)
    {
        Serial.print(bar[i]);
    }
    Serial.println();
}

//General function to print arrays (seems to work)
void printArray(const int *arr, size_t len){
  Serial.print('[');
  for (size_t i = 0; i < len; ++i) {
    Serial.print(arr[i]);
    if (i < len - 1) Serial.print(", ");
  }
  Serial.println(']');
}



void generateRhythm(){
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



void playRhythm(){
      printArray();
    for (int i = 0; i < bar_length; i++)
    {
        digitalWrite(LED_PIN, bar[i]);
        delay(200);  // Adjust delay for timing (200ms per step)
    }
}


//waits for the ISR to run and set the value (DOESNT LOOP?)
void waitForButton(){
    while(!buttonPressed){
        Serial.println("Entered wait state");
    }

    Serial.println("Exited wait state");
}

//*
//ISR for button being pressed
void ButtonPressed(){

    //Set boolean depending if it's a low edge or high edge
    if(digitalRead(BUTTON_PIN)){
        buttonPressed=true;
        //userInputDetected=true;
    }
    else{
        buttonPressed=false;
    }
    
}
//*/


//setup
void setup(){
    pinMode(LED_PIN, OUTPUT);

    //set button pin to input
    pinMode(BUTTON_PIN, INPUT);

    Serial.begin(115200);
    
    
    //generateRhythm();
    Serial.println("TEST");

    //attach ISR
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ButtonPressed, CHANGE);

    MilliTimer=millis();

    //reset buttonPressed
    buttonPressed = false;

    //reset userInputDetected
    userInputDetected=false;

    //set msDelay based off BPM and # of samples
    //  should be 10ms in our implementation;
    msDelay = 60000 / (BPM*sampPerBeat);

    //Calculate total sample amount
    //samples=sampPerBeat*bar_length;

    //TEST - print ms delay calculation
    Serial.println("ms Delay:");
    Serial.println(msDelay);
}

//loop function
void loop()
{

    //IF WE CANT GET THIS TO WORK DO A COUNTDOWN
    //waitForButton();

    Serial.println("3");
    delay(1000);
    Serial.println("2");
    delay(1000);
    Serial.println("1");
    delay(1000);
    Serial.print("READING INPUT");

    getUserInput();

    printArray(userInput,samples);

    delay(2000);

    /*
    if(userInputDetected){
        if(millis()-MilliTimer>=1000){
            if(buttonPressed){
            Serial.println("Button Pressed");
            }
            else{
                Serial.println("Button Not Pressed");
            }
            MilliTimer=millis();
        }
        userInputDetected=false;
    }
    //*/

    /* WORKING TEST CODE - USE FOR REFERENCE
    if(millis()-MilliTimer>=100){
        if(digitalRead(BUTTON_PIN)==HIGH){
        Serial.println("Button Pressed");
        }
        else{
            Serial.println("Button Not Pressed");
        }
        MilliTimer=millis();
    }

    //*/
    
    
    /*
    //print array (error checking code - remove in final product)
    printArray();





    //-------START OF FINAL PRODUCT CODE----------------

    //generate pattern & check to make sure it has a 1
    generateRhythm();

    //CHECK FOR ALL 0's

    //play rhythm
    playRhythm();

    //enter wait state
    waitForButton();

    //check rhythm
    rhythmCheck();

    //*/

    /*
    playRhythm();
    delay(1000);  // Wait before repeating the pattern
    generateRhythm();
    */
    
}
