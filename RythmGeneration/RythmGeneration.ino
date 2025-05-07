//#define LED_PIN D10  // Pin for LED output
#define BUTTON_PIN D5 //Pin for Button (can change)
#define VIBRATION_PIN D10 //Pin for Motor


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
//      MIGHT NEED TO MAKE IT A FLOAT
int averages[bar_length];

//  array holding whether 
int highLow[bar_length];

//pass/fail array (might be redundant)
int passFail[bar_length];

//  user input samples
int userInput[samples];

//boolean telling the code the button has been pressed
bool buttonPressed;

//experimental boolean (only used for bad testing)
bool userInputDetected;

//Millis timer (for test code)
unsigned long MilliTimer;


bool printedWaiting;


//takes the user-input-filled array and checks it against the generated array
//  NEED TO TEST
//
//      NEED TO ACTUALLY CALCULATE AVERAGES BEFORE PASS/FAIL!!!!!!!
void rhythmCheck(){

    //create array of averages of each userInput row (leave as raw, dont shift based on firstHigh)
    int avgIter = 0;
    int tempAvg=0;
    for(int i=0;i<samples;i++){

        tempAvg = tempAvg + userInput[i];

        if((i+1)%sampPerBeat==0){
            //put average into average array
            if(((float)(tempAvg/sampPerBeat))>=.5){
                averages[avgIter]=1;
            }
            else{
                averages[avgIter]=0;
            }
            //iterate average & reset tempAvg
            tempAvg=0;
            avgIter++;
        }
    }

    //find where first rhythm is 1
    int firstHigh = 0;
    while(bar[firstHigh]==0 && firstHigh<bar_length){
        firstHigh=firstHigh+1;
    }

    //if there is no first zero, throw error
    if(firstHigh>=bar_length){
        Serial.println("ERROR: rhythm is full of 0s");
    }


    //start from first high and compare to start of user input
    //Serial.println("Pass/Fail output:");
    int counter = 0;
    for(int i =firstHigh;i<bar_length;i++){
        if((bar[i]==1&&averages[counter]>=.5)||(bar[i]==0&&averages[counter]<=.5)){
            passFail[i]=1;
            //Serial.print("1");
        }
        else{
            passFail[i]=0;
            //Serial.print("0");
        }
        counter++;
    }

    //if firstHigh isnt at the beginning,
    //  finish the remaining user input checks
    if(firstHigh!=0){
        for(int i = 0;i<firstHigh;i++){
            if((bar[i]==1&&averages[counter]>=.5)||(bar[i]==0&&averages[counter]<=.5)){
                passFail[i]=1;
                //Serial.print("1");
            }
            else{
                passFail[i]=0;
                //Serial.print("0");
            }
            counter++;
        }
    }

  

}

//controls timing for the button input
//      (TIMING WORKS)
void getUserInput(){
    int lastSampleTime = millis();
    int firstSampleTime=lastSampleTime;

    //For every beat, read in user samples
    //      Should run for 32 seconds - FIX
    for(int i=0;i<samples;i++){
        bool testTimingLoop = false;

        if(digitalRead(BUTTON_PIN)==HIGH){
            userInput[i] = 1;
            //digitalWrite(VIBRATION_PIN,HIGH);
        }
        else{
            userInput[i] = 0;
            //digitalWrite(VIBRATION_PIN,LOW);
        }
        
        //loops until timing requirement is met
        while(millis()-lastSampleTime<msDelay){
            
            /*
            //looper function (below is test timing code)
            if(!testTimingLoop){
                testTimingLoop=true;
                Serial.print("Looping at ");
                Serial.print(millis());
                Serial.print(" Millis");
            }
            else{
                Serial.print(".");
            }
            //*/
            
        }

        lastSampleTime = millis();

        /*
        Serial.println();
        Serial.print("Millis: ");
        Serial.println(millis());
        //*/

        /*
        Serial.print("Sample Iteration: ");
        Serial.println(i);
        //*/

    }
    Serial.print("Elapsed Sample Time: ");
    Serial.print((lastSampleTime-firstSampleTime)/1000);
    Serial.println(" Seconds");
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

//prints the active bar
void printArray(){
    Serial.println("=== NEW RHYTHM ===");
    for (int i = 0; i < bar_length; i++)
    {
        Serial.print(bar[i]);
    }
    Serial.println();
}

//General function to print arrays (seems to work)
//  (prints each beat on a separate line)
void printArray(const int *arr, size_t len){
  Serial.print('[');
  for (size_t i = 0; i < len; ++i) {

    Serial.print(arr[i]);
    if (i < len - 1) Serial.print(", ");

  }
  Serial.println(']');
}

//Function that prints total output of all samples
//  (prints each beat on a separate line)
//      j is # of samples per beat (named bc of 2d array)
void printArray(const int *arr, size_t len, int j){
  Serial.print('[');
  for (size_t i = 0; i < len; ++i) {

    //Every beat print on a new line
    if(i%j==0){
        Serial.println();
    }

    Serial.print(arr[i]);
    if (i < len - 1) Serial.print(", ");

    

  }
  Serial.println(']');
}


//TEST
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


//TEST
void playRhythm(){
    printArray();
    Serial.print("Bar Vibrations: ");
    for (int i = 0; i < bar_length; i++)
    {
        Serial.print(bar[i]);
        digitalWrite(VIBRATION_PIN, bar[i]);
        delay((BPM/60)*1000);  // Adjust delay for timing
    }
    Serial.println();
}


//waits for the ISR to run and set the value
//      (works for now)
void waitForButton(){
    long waitLastMillis=millis();
    Serial.println("Entered wait state");
    bool waitStateBool = false;

    //attempted debounce
    while(!waitStateBool){

        if(digitalRead(BUTTON_PIN)){
            delay(10);
            if(digitalRead(BUTTON_PIN)){
                waitStateBool=true;
            }
        }

        if(millis()-waitLastMillis>1000){
            if(!printedWaiting){
                Serial.print("In wait state");
                printedWaiting=true;
            }
            else{
                Serial.print(".");
            }
            
            waitLastMillis=millis();
        }
    }
    
    /*  Enters but doesnt leave (test with interrupt attached)
    while(!buttonPressed){
        if(millis()-waitLastMillis>1000){
            Serial.println("In wait state");
            waitLastMillis=millis();
        }
        
    }
    */
    Serial.println();
    Serial.println("Exited wait state");
}

/*
//ISR for button being pressed (MIGHT NEED TO DELETE)
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

void displayResults(){
    Serial.println("User Input: ");
    printArray(userInput,samples,sampPerBeat);

    Serial.println();

    Serial.print("Averages: ");
    printArray(averages,bar_length);

    Serial.println();
    Serial.print("Pass/Fail: ");
    printArray(passFail,bar_length);
}


//setup
void setup(){
    pinMode(LED_PIN, OUTPUT);

    //set button pin to input
    pinMode(BUTTON_PIN, INPUT);

    pinMode(VIBRATION_PIN,OUTPUT);

    Serial.begin(115200);


    Serial.println("----------BEGINNING OF CODE-----------");

    //attach ISR
    //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ButtonPressed, CHANGE);

    //
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

    printedWaiting=false;

    //digitalWrite(VIBRATION_PIN,LOW);
}

//loop function
void loop()
{
    
    //IF WE CANT GET THIS TO WORK DO A COUNTDOWN
    //waitForButton();

    /*

    Serial.println("3");
    delay(1000);
    Serial.println("2");
    delay(1000);
    Serial.println("1");
    delay(1000);
    Serial.println("READING INPUT");

    //*/

    //getUserInput();



    //BEGINNING OF TEST RHYTHM CHECK - PLUG IN HARDCODE ARRAY FOR GENERATED AND USER ONE
    //                       AND CHECK EXPECTED OUTPUT:

    /*
    //hardcode arrays
    //EXPECTED ARRAY: FIRST HALF 0s SECOND HALF 1s
    for(int i=0;i<bar_length;i++){
        if(i<(bar_length/2)){
            bar[i]=0;
        }
        else{
            bar[i]=1;
        }

    }
    //User input array (SHOULD BE FIRST HALF 1s AND SECOND HALF 0s based on our logic)
    for(int i=0;i<samples;i++){
        if(i<(samples/2)){
            userInput[i]=1;
        }
        else{
            userInput[i]=0;
        }

    }
    
    //print user input array
    Serial.println("User input array:");
    printArray(userInput,samples,sampPerBeat);
    
    //run rhythmCheck()
    rhythmCheck();

    

    //print out averages and pass/fail (should be all 1s for first test)
    Serial.print("Averages: ");
    printArray(averages,bar_length);

    Serial.println();
    Serial.print("Pass/Fail: ");
    printArray(passFail,bar_length);
    


    waitForButton();
    //*/

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
    


    //-------START OF FINAL PRODUCT CODE----------------

    //*

    //temporary vibration fix
    digitalWrite(VIBRATION_PIN,LOW);

    //generate pattern & check to make sure it has a 1
    generateRhythm();

    //Output & play bar to match to serial
    //playRhythm();

    //CHECK FOR ALL 0's IN PATTERN

    //play rhythm
    playRhythm();
    digitalWrite(VIBRATION_PIN,LOW);

    //enter wait state (WORKS)
    printedWaiting=false;
    waitForButton();
    Serial.println("READING INPUT");

    //Take user input (WORKS)
    getUserInput();

    //check rhythm
    rhythmCheck();

    displayResults();

    delay(5000);  // Wait before repeating the pattern
    //*/

    /*
    playRhythm();
    delay(1000);  // Wait before repeating the pattern
    generateRhythm();
    //*/
}