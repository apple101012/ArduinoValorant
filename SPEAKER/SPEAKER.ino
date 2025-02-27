void setup() {
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define SPEAKER 3 // important shit
#define POT 18 //important shit
}
int NOTE_DUR = 58;

void loop() {
  Serial.begin(9600);
  Serial.println(analogRead(POT));
  if(analogRead(POT) < 400){
    TaylorSwiftSong();
  }
  else if(analogRead(POT) < 800){
    Despacito();
  }
  else{
    RansomSong();
  }
  }

void play(int note, int dur){
  tone(SPEAKER, note);
  delay(dur * NOTE_DUR);
  noTone(SPEAKER);
  delay(dur * NOTE_DUR / 3);
}

void Despacito() {
    NOTE_DUR = 58;  // Set tempo to be faster
    play(NOTE_D4, 1);
    play(NOTE_FS4, 1);
    play(NOTE_B4, 1);
    play(NOTE_D5, 1);
    play(NOTE_CS5, 1);
    play(NOTE_B4, 1);
    play(NOTE_AS4, 1);
    play(NOTE_B4, 16); //m2
    delay(NOTE_DUR*6);
    play(NOTE_B4, 6);
    play(NOTE_CS5, 6);
    play(NOTE_D5, 6);
    play(NOTE_E5, 6); //m3
    play(NOTE_FS5, 7);
    play(NOTE_D5, 7);
    play(NOTE_FS5, 8);
    play(NOTE_D5, 8);
    play(NOTE_FS5, 22);
    delay(NOTE_DUR*4);
    play(NOTE_FS4, 2); //m4
    play(NOTE_D5, 6);
    play(NOTE_E4, 2);
    play(NOTE_CS5, 6);
    play(NOTE_D4, 2);
    play(NOTE_B4, 4); //m5
    play(NOTE_D4, 2);
    play(NOTE_FS4, 4);
    play(NOTE_B4, 2);
    play(NOTE_D4, 4);
    play(NOTE_B4, 4);
    play(NOTE_D4, 2);
    play(NOTE_FS4, 2);
    play(NOTE_D5, 2);
    play(NOTE_CS5, 2);
    play(NOTE_B4, 2);
    play(NOTE_A4, 2);
    play(NOTE_B4, 4); //m6
    play(NOTE_D4, 2);
    play(NOTE_G4, 4);
    play(NOTE_B4, 2);
    play(NOTE_D4, 4);
    play(NOTE_B4, 4);
    play(NOTE_D4, 2);
    play(NOTE_G4, 2);
    play(NOTE_D5, 2);
    play(NOTE_CS5, 2);
    play(NOTE_B4, 2);
    play(NOTE_A4, 2);
    play(NOTE_D5, 4); //m7
    play(NOTE_FS4, 2);
    play(NOTE_A4, 4);
    play(NOTE_D5, 2);
    play(NOTE_FS4, 4);
    play(NOTE_D5, 4);
    play(NOTE_FS4, 2);
    play(NOTE_A4, 2);
    play(NOTE_D5, 2);
    play(NOTE_CS5, 2);
    play(NOTE_B4, 2);
    play(NOTE_A4, 2);
    play(NOTE_E5, 12); //m8
    delay(NOTE_DUR*2);
    play(NOTE_FS4, 2);
    play(NOTE_D5, 6);
    play(NOTE_E4, 2);
    play(NOTE_CS5, 6);
    play(NOTE_D4, 2);
    play(NOTE_B4, 10); //m9
}
void TaylorSwiftSong() {
  NOTE_DUR = 100;  // Set tempo to be moderate
  play(NOTE_E5, 1); // Verse 1
  play(NOTE_D5, 1);
  play(NOTE_B4, 2);
  play(NOTE_D5, 2);
  play(NOTE_E5, 1);
  play(NOTE_E5, 1);
  play(NOTE_E5, 2);
  play(NOTE_F5, 2);
  play(NOTE_E5, 4);
  delay(NOTE_DUR * 4); // Chorus
  play(NOTE_E5, 1);
  play(NOTE_D5, 1);
  play(NOTE_B4, 2);
  play(NOTE_D5, 2);
  play(NOTE_E5, 1);
  play(NOTE_E5, 1);
  play(NOTE_E5, 2);
  play(NOTE_F5, 2);
  play(NOTE_E5, 4);
  delay(NOTE_DUR * 4); // Verse 2
  play(NOTE_E5, 1);
  play(NOTE_D5, 1);
  play(NOTE_B4, 2);
  play(NOTE_D5, 2);
  play(NOTE_E5, 1);
  play(NOTE_E5, 1);
  play(NOTE_E5, 2);
  play(NOTE_F5, 2);
  play(NOTE_E5, 4);
  delay(NOTE_DUR * 4); // Chorus
  // ... More verses and choruses can be added here
}

void RansomSong() {
  NOTE_DUR = 200;  // Set tempo to be moderate
  play(NOTE_D5, 2); // Verse 1
  play(NOTE_D5, 2);
  play(NOTE_E5, 2);
  play(NOTE_D5, 2);
  play(NOTE_A4, 1);
  play(NOTE_A4, 1);
  play(NOTE_B4, 2);
  play(NOTE_A4, 1);
  play(NOTE_G4, 1);
  play(NOTE_F4, 2);
  play(NOTE_G4, 2);
  play(NOTE_D5, 2); // Chorus
  play(NOTE_D5, 2);
  play(NOTE_E5, 2);
  play(NOTE_D5, 2);
  play(NOTE_A4, 1);
  play(NOTE_A4, 1);
  play(NOTE_B4, 2);
  play(NOTE_A4, 1);
  play(NOTE_G4, 1);
  play(NOTE_F4, 2);
  play(NOTE_G4, 2);
  play(NOTE_B4, 2); // Verse 2
  play(NOTE_B4, 2);
  play(NOTE_C5, 2);
  play(NOTE_B4, 2);
  play(NOTE_G4, 1);
  play(NOTE_G4, 1);
  play(NOTE_A4, 2);
  play(NOTE_G4, 1);
  play(NOTE_F4, 1);
  play(NOTE_E4, 2);
  play(NOTE_F4, 2);
  play(NOTE_D5, 2); // Chorus
  // ... More verses and choruses can be added here
}