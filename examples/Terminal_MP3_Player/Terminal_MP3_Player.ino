/*
 WT2003 S Terminal_MP3_Player

 This example makes an MP3 player that can be controlled from the terminal window on a computer.
 It exposes the full set of commands that are available for the WT2003S
 
 Using the WT2003S MP3 Decoder IC on the SparkFun breakout board with Arduino Uno

 Hardware Connections:
 WT2003S Breakout Pin ---> MoonBot Port
 -------------------------------------
 WT2003S              --->   Port7(Serial1)
 WT2003S              --->   Port2(Serial2)
 WT2003S              --->   Port9(Serial3)
 */

#include "MoonBot.h"

// Defining some status codes from the WT2003S
#define STATUS_PLAY 0x01
#define STATUS_STOP 0x02
#define STATUS_PAUSE 0x03

bool autoplay = true;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  };
  speaker.begin(Serial2);                                             // Beginning the MP3 player requires a serial port (either hardware or software!!!)

  Serial.println("MP3 Breakout - Example1 Serial Control");
  Serial.println();

  printMenu();

  Serial.print("Number of tracks on SD card: ");
  Serial.println(speaker.getSongCount());

  if (speaker.getSongCount() == 0) {
    Serial.println(
        "Oh no! No songs found... try adding songs or plugging in the sd card then try again");
    while (1);                                                        // Hold here
  }

  speaker.setVolume(4);
  speaker.playTrackNumber(1);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == '+') {
      Serial.print("Volume up: ");
      uint8_t volume = speaker.getVolume();
      speaker.setVolume(++volume);
      Serial.print(volume);
      Serial.println("/31");
    } else if (cmd == '-') {
      Serial.print("Volume down: ");
      uint8_t volume = speaker.getVolume();
      if (--volume > 31) {
        volume = 0;
      }
      speaker.setVolume(volume);
      Serial.print(volume);
      Serial.println("/31");
    } else if (cmd == 'n') {
      Serial.println("Next track:");
      speaker.playNext();
      delay(200);                                                     // It takes a moment for the device to start playing the next song
      speaker.getSongName();
      Serial.print("Playing: ");
      Serial.write(speaker.songName, 8);
      Serial.println();
    } else if (cmd == 'l') {
      Serial.println("Last track:");
      speaker.playPrevious();
      delay(200);                                                     // It takes a moment for the device to know the next song name
      speaker.getSongName();
      Serial.print("Playing: ");
      Serial.write(speaker.songName, 8);
      Serial.println();
    } else if (cmd == 'p') {
      uint8_t status = speaker.getPlayStatus();
      if (status == STATUS_PLAY) {
        Serial.println("Paused: ('p' to resume)");
        autoplay = false;
        speaker.pause();                                              // Should pause when currently playing
      } else if (status == STATUS_PAUSE) {
        speaker.pause();                                              // Should play when already paused
        delay(200);                                                   // It takes a moment for the device to know the next song name
        Serial.print("Resuming: ");
        speaker.getSongName();
        Serial.write(speaker.songName, 8);
        Serial.println();
      } else if (status == STATUS_STOP) {
        speaker.pause();                // Plays from beginning of current track
        delay(200);  // It takes a moment for the device to know the next song name
        speaker.getSongName();
        Serial.print("Playing: ");
        Serial.write(speaker.songName, 8);
        Serial.println();
      }
    } else if (cmd == 's') {
      Serial.println("Stopping: ('p' to play)");
      autoplay = false;
      speaker.stop();
    } else if (cmd == 'a') {
      autoplay = !autoplay;
      Serial.print("Autoplay is ");
      if (autoplay) {
        Serial.println(" on");
      } else {
        Serial.println(" off");
      }
    } else if (cmd == '?') {
      uint8_t status = speaker.getPlayStatus();
      if (status == STATUS_PLAY) {
        speaker.getSongName();
        autoplay = true;
        Serial.print("Playing: ");
        Serial.write(speaker.songName, 8);
        Serial.println();
      } else if (status == STATUS_STOP) {
        Serial.println("MP3 player is stopped");
      } else {
        Serial.println("MP3 player is paused");
      }
    } else if (cmd == 'w') {
      autoplay = false;
      speaker.setPlayMode(0);                                         // set play mode: Single No Loop
    } else if (cmd == 'x') {
      autoplay = false;
      speaker.setPlayMode(1);                                         // set play mode: Single Loop
    } else if (cmd == 'y') {
      autoplay = true;                                                // set play mode: Loop All
      speaker.setPlayMode(2);
    } else if (cmd == 'z') {
      autoplay = false;
      speaker.setPlayMode(3);                                         // set play mode: Random
    } else {
      printMenu();
    }
  }

  // Autoplay controller
  uint8_t playStatus = speaker.getPlayStatus();
  if ((playStatus == STATUS_STOP) && (autoplay == true)) {
    speaker.playNext();
    delay(250);   // It takes a moment for the device to know the next song name
    speaker.getSongName();
    Serial.print("Playing: ");
    Serial.write(speaker.songName, 8);
    Serial.println();
  }
}

void printMenu(void) {
  Serial.println("MP3 Command List:");
  Serial.println("-----------------");
  Serial.println("'+' or '-'  : raise/lower volume");
  Serial.println("'n' or 'l'  : next/last song");
  Serial.println("'p'         : play or pause");
  Serial.println("'s'         : stop playing");
  Serial.println("'a'         : toggle autoplay");
  Serial.println("'?'         : what is playing?");
  Serial.println("'w'         : set playmode single no loop");
  Serial.println("'x'         : set playmode single loop");
  Serial.println("'y'         : set playmode paly all");
  Serial.println("'z'         : set playmode random");
  Serial.println();
}

