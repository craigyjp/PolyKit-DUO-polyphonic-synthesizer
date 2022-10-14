#define SETTINGSOPTIONSNO 6
#define SETTINGSVALUESNO 18  //Maximum number of settings option values needed
int settingsValueIndex = 0;  //currently selected settings option value index

struct SettingsOption {
  char *option;                   //Settings option string
  char *value[SETTINGSVALUESNO];  //Array of strings of settings option values
  int handler;                    //Function to handle the values for this settings option
  int currentIndex;               //Function to array index of current value for this settings option
};

void settingsMIDICh(char *value);
void settingsAfterTouch(char *value);
//void settingsPitchBend(char * value);
void settingsModWheelDepth(char *value);
void settingsEncoderDir(char *value);
void settingsFilterEnv(char *value);
void settingsAmpEnv(char *value);
void settingsHandler(char *s, void (*f)(char *));

int currentIndexMIDICh();
int currentIndexAfterTouch();
//int currentIndexPitchBend();
int currentIndexModWheelDepth();
int currentIndexEncoderDir();
int currentIndexFilterEnv();
int currentIndexAmpEnv();
int getCurrentIndex(int (*f)());


void settingsMIDICh(char *value) {
  if (strcmp(value, "ALL") == 0) {
    midiChannel = MIDI_CHANNEL_OMNI;
  } else {
    midiChannel = atoi(value);
  }
  storeMidiChannel(midiChannel);
}

void settingsAfterTouch(char *value) {
  if (strcmp(value, "Off") == 0) AfterTouchDest = 0;
  if (strcmp(value, "DCO Mod") == 0) AfterTouchDest = 1;
  if (strcmp(value, "CutOff Freq") == 0) AfterTouchDest = 2;
  if (strcmp(value, "VCF Mod") == 0) AfterTouchDest = 3;
  storeAfterTouch(AfterTouchDest);
}

//
//void settingsPitchBend(char * value) {
//  pitchBendRange = atoi(value);
//  storePitchBendRange(pitchBendRange);
//}
//
void settingsModWheelDepth(char *value) {
  modWheelDepth = atoi(value);
  storeModWheelDepth(modWheelDepth);
}

void settingsEncoderDir(char *value) {
  if (strcmp(value, "Type 1") == 0) {
    encCW = true;
  } else {
    encCW = false;
  }
  storeEncoderDir(encCW ? 1 : 0);
}

void settingsFilterEnv(char *value) {
  if (strcmp(value, "Log") == 0) {
    filterLogLin = true;
  } else {
    filterLogLin = false;
  }
  storeFilterEnv(filterLogLin ? 1 : 0);
  
}

void settingsAmpEnv(char *value) {
  if (strcmp(value, "Log") == 0) {
    ampLogLin = true;
  } else {
    ampLogLin = false;
  }
  storeAmpEnv(ampLogLin ? 1 : 0);
  
}

//Takes a pointer to a specific method for the settings option and invokes it.
void settingsHandler(char *s, void (*f)(char *)) {
  f(s);
}

int currentIndexMIDICh() {
  return getMIDIChannel();
}

int currentIndexAfterTouch() {
  return getAfterTouch();
}

//
//int currentIndexPitchBend() {
//  return  getPitchBendRange() - 1;
//}
//
int currentIndexModWheelDepth() {
  return (getModWheelDepth()) - 1;
}

int currentIndexEncoderDir() {
  return getEncoderDir() ? 0 : 1;
}

int currentIndexFilterEnv() {
  return getFilterEnv() ? 0 : 1;
}

int currentIndexAmpEnv() {
  return getAmpEnv() ? 0 : 1;
}

//Takes a pointer to a specific method for the current settings option value and invokes it.
int getCurrentIndex(int (*f)()) {
  return f();
}

CircularBuffer<SettingsOption, SETTINGSOPTIONSNO> settingsOptions;

// add settings to the circular buffer
void setUpSettings() {
  settingsOptions.push(SettingsOption{ "MIDI Ch.", { "All", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", '\0' }, settingsMIDICh, currentIndexMIDICh });
  settingsOptions.push(SettingsOption{ "AfterTouch.", { "Off", "DCO Mod", "CutOff Freq", "VCF Mod", '\0' }, settingsAfterTouch, currentIndexAfterTouch });
  //  settingsOptions.push(SettingsOption{"Pitch Bend", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", '\0'}, settingsPitchBend, currentIndexPitchBend});
  settingsOptions.push(SettingsOption{ "MW Depth", { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", '\0' }, settingsModWheelDepth, currentIndexModWheelDepth });
  settingsOptions.push(SettingsOption{ "Encoder", { "Type 1", "Type 2", '\0' }, settingsEncoderDir, currentIndexEncoderDir });
  settingsOptions.push(SettingsOption{ "Filter Env", { "Log", "Lin", '\0' }, settingsFilterEnv, currentIndexFilterEnv });
  settingsOptions.push(SettingsOption{ "Amplifier Env", { "Log", "Lin", '\0' }, settingsAmpEnv, currentIndexAmpEnv });
}