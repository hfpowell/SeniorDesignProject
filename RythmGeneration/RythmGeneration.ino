#define LED_PIN D10  // Pin for LED output

const int bar_length = 32;
int bar[bar_length];  // Rhythm pattern

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

//takes the user input and checks it against the generated array
void rhythmCheck(){

  

}

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    generateRhythm();
    Serial.println("TEST");
}

void loop()
{
    playRhythm();
    delay(1000);  // Wait before repeating the pattern
    generateRhythm();
}
