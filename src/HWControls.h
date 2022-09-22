// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <Bounce.h>
#include <ADC.h>
#include <ADC_util.h>

ADC *adc = new ADC();

//Teensy 3.6 - Mux Pins
#define MUX_0 28
#define MUX_1 27
#define MUX_2 26
#define MUX_3 25

#define MUX1_S A19
#define MUX2_S A22
#define MUX3_S A14

#define DEMUX_0 40
#define DEMUX_1 41
#define DEMUX_2 43
#define DEMUX_3 42

#define DEMUX_EN_1 6
#define DEMUX_EN_2 23
#define DEMUX_EN_3 22

//Note DAC
#define DAC_NOTE1 7
#define NOTE_DAC(CH) (CH==0 ? DAC_NOTE1)
#define NOTE_AB(CH)  (CH==1 ? 1 : 0)

//Mux 1 Connections
#define MUX1_pwLFO 0
#define MUX1_fmDepth 1
#define MUX1_osc2PW 2
#define MUX1_osc2PWM 3
#define MUX1_osc1PW 4
#define MUX1_osc1PWM 5
#define MUX1_osc1Range 6
#define MUX1_osc2Range 7
#define MUX1_stack 8
#define MUX1_glideTime 9
#define MUX1_osc2Detune 10
#define MUX1_noiseLevel 11
#define MUX1_osc1SawLevel 12
#define MUX1_osc2SawLevel 13
#define MUX1_osc1PulseLevel 14
#define MUX1_osc2PulseLevel 15

//Mux 2 Connections
#define MUX2_filterCutoff 0
#define MUX2_filterLFO 1
#define MUX2_filterRes 2
#define MUX2_filterType 3
#define MUX2_filterEGlevel 4
#define MUX2_LFORate 5 // spare mux output - decouple from DAC
#define MUX2_LFOWaveform 6
#define MUX2_volumeControl 7 // spare mux output
#define MUX2_filterAttack 8
#define MUX2_filterDecay 9
#define MUX2_filterSustain 10
#define MUX2_filterRelease 11
#define MUX2_ampAttack 12 // spare mux output - decouple from DAC
#define MUX2_ampDecay 13
#define MUX2_ampSustain 14
#define MUX2_ampRelease 15

//Mux 3 Connections
#define MUX3_modwheel 0
#define MUX3_1 1
#define MUX3_2 2
#define MUX3_3 3
#define MUX3_4 4
#define MUX3_5 5
#define MUX3_6 6
#define MUX3_7 7
#define MUX3_8 8
#define MUX3_9 9
#define MUX3_10 10
#define MUX3_11 11
#define MUX3_12 12
#define MUX3_13 13
#define MUX3_14 14
#define MUX3_15 15

//DeMux 1 Connections
#define DEMUX1_noiseLevel 0                 // 0-2v
#define DEMUX1_osc2PulseLevel 1             // 0-2v
#define DEMUX1_osc2Level 2                  // 0-2v
#define DEMUX1_LfoDepth 3                   // 0-2v
#define DEMUX1_osc1PWM 4                    // 0-2v
#define DEMUX1_PitchBend 5                  // 0-2v
#define DEMUX1_osc1PulseLevel 6             // 0-2v
#define DEMUX1_osc1Level 7                  // 0-2v
#define DEMUX1_modwheel 8                   // 0-2v
#define DEMUX1_volumeControl 9              // 0-2v
#define DEMUX1_osc2PWM 10                   // 0-2v
#define DEMUX1_spare 11                     // spare VCA 0-2v
#define DEMUX1_AmpEGLevel 12                // Amplevel 0-5v
#define DEMUX1_MasterTune 13                // -15v to +15v  control 12   +/-13v
#define DEMUX1_osc2Detune 14                // -15v to +15v control 17 +/-13v
#define DEMUX1_pitchEGLevel 15              // PitchEGlevel 0-5v

//DeMux 2 Connections
#define DEMUX2_Amprelease 0                 // 0-5v
#define DEMUX2_Ampsustain 1                 // 0-5v
#define DEMUX2_Ampdecay 2                   // 0-5v
#define DEMUX2_Ampattack 3                  // 0-5v
#define DEMUX2_LfoRate 4                    // 0-5v
#define DEMUX2_LFOMULTI 5                   // 0-5v
#define DEMUX2_LFOWaveform 6                // 0-5v
#define DEMUX2_LfoDelay 7                   // 0-5v
#define DEMUX2_osc2PW 8                     // 0-5v
#define DEMUX2_osc1PW 9                     // 0-5v
#define DEMUX2_filterEGlevel 10             // FilterEGlevel 0-5v  
#define DEMUX2_ADSRMode 11                  // 0-5v
#define DEMUX2_pwLFO 12                     // 0-5v 
#define DEMUX2_LfoSlope 13                  // 0-5v 
#define DEMUX2_filterCutoff 14              // 0-10v
#define DEMUX2_filterRes15                  // 0-10v

//DeMux 3 Connections
#define DEMUX3_filterA 0                    // filterA 0-5v switched
#define DEMUX3_filterB 1                    // filterB 0-5v switched
#define DEMUX3_filterC 2                    // filterC 0-5v switched
#define DEMUX3_filterLoopA0 3                     // syncA0 0-5v switched
#define DEMUX3_filterLoopA1 4                     // syncA1 0-5v switched    
#define DEMUX3_LFOalt 5                     // lFOalt 0-5v switched
#define DEMUX3_spare 6                      // spare 0-5v switched
#define DEMUX3_spare1 7                      // spare 0-5v switched
#define DEMUX3_pitchAttack 8                // Pitchattack 0-5v
#define DEMUX3_pitchDecay 9                 // Pitchdecay 0-5v
#define DEMUX3_pitchSustain 10              // Pitchsustain  0-5v
#define DEMUX3_PitchRelease 11              // Pitchrelease 0-5v
#define DEMUX3_filterAttack 12              // FilterAattack 0-5v
#define DEMUX3_filterDecay 13               // Filterdecay 0-5v
#define DEMUX3_filterSustain 14             // Filtersustain 0-5v
#define DEMUX3_filterEelease 14             // Filterrelease 0-5v

// 595 outputs

#define FILTER_EG_INV 10
#define FILTER_POLE 11
#define FILTER_KEYTRACK 12
#define AMP_VELOCITY 13
#define FILTER_VELOCITY 14
#define AMP_LIN_LOG 15
#define FILTER_LIN_LOG 16
#define LFO_ALT 17
#define AMP_LOOP 20
#define FILTER_LOOP 21
#define CHORUS1_OUT 22 
#define CHORUS2_OUT 23

// 595 LEDs

#define LFO_ALT_LED 9 
#define CHORUS1_LED 18 
#define CHORUS2_LED 19

//Switches
//Teensy 3.6 Pins

#define GLIDE_SW 34
#define FILTERKEYTRACK_SW 35
#define FILTERPOLE_SW 52
#define FILTERLOOP_SW 37
#define FILTERINV_SW 14
#define FILTERVEL_SW 15
#define VCALOOP_SW 51
#define VCAVEL_SW 18
#define VCAGATE_SW 49
#define LFO_ALT_SW 50
#define CHORUS2_SW 19
#define CHORUS1_SW 9

#define RECALL_SW 17
#define SAVE_SW 24
#define SETTINGS_SW 12
#define BACK_SW 10

#define ENCODER_PINA 4
#define ENCODER_PINB 5

#define MUXCHANNELS 16
#define DEMUXCHANNELS 16
#define QUANTISE_FACTOR 10

#define DEBOUNCE 30

static byte muxInput = 0;
static byte muxOutput = 0;
static int mux1ValuesPrev[MUXCHANNELS] = {};
static int mux2ValuesPrev[MUXCHANNELS] = {};
static int mux3ValuesPrev[MUXCHANNELS] = {};

static int mux1Read = 0;
static int mux2Read = 0;
static int mux3Read = 0;

static long encPrevious = 0;

//These are pushbuttons and require debouncing
Bounce glideSwitch = Bounce(GLIDE_SW, DEBOUNCE);
Bounce keytrackSwitch = Bounce(FILTERKEYTRACK_SW, DEBOUNCE);
Bounce filterPoleSwitch = Bounce(FILTERPOLE_SW, DEBOUNCE);
Bounce filterLoopSwitch = Bounce(FILTERLOOP_SW, DEBOUNCE);
Bounce filterVelSwitch = Bounce(FILTERVEL_SW, DEBOUNCE);
Bounce filterInvSwitch = Bounce(FILTERINV_SW, DEBOUNCE);
Bounce vcaVelSwitch = Bounce(VCAVEL_SW, DEBOUNCE);
Bounce vcaLoopSwitch = Bounce(VCALOOP_SW, DEBOUNCE);
Bounce vcaGateSwitch = Bounce(VCAGATE_SW, DEBOUNCE);
Bounce lfoAltSwitch = Bounce(LFO_ALT_SW, DEBOUNCE);
Bounce chorus1Switch = Bounce(CHORUS1_SW, DEBOUNCE);
Bounce chorus2Switch = Bounce(CHORUS2_SW, DEBOUNCE);

Bounce recallButton = Bounce(RECALL_SW, DEBOUNCE); //On encoder
boolean recall = true; //Hack for recall button
Bounce saveButton = Bounce(SAVE_SW, DEBOUNCE);
boolean del = true; //Hack for save button
Bounce settingsButton = Bounce(SETTINGS_SW, DEBOUNCE);
boolean reini = true; //Hack for settings button
Bounce backButton = Bounce(BACK_SW, DEBOUNCE);
boolean panic = true; //Hack for back button
Encoder encoder(ENCODER_PINB, ENCODER_PINA);//This often needs the pins swapping depending on the encoder

void setupHardware()
{
  //Volume Pot is on ADC0
  adc->adc0->setAveraging(16); // set number of averages 0, 4, 8, 16 or 32.
  adc->adc0->setResolution(10); // set bits of resolution  8, 10, 12 or 16 bits.
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED); // change the conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED); // change the sampling speed

  //MUXs on ADC1
  adc->adc1->setAveraging(16); // set number of averages 0, 4, 8, 16 or 32.
  adc->adc1->setResolution(10); // set bits of resolution  8, 10, 12 or 16 bits.
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED); // change the conversion speed
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED); // change the sampling speed


  //Mux address pins

  pinMode(DAC_NOTE1, OUTPUT);
  digitalWrite(DAC_NOTE1, HIGH);

  pinMode(MUX_0, OUTPUT);
  pinMode(MUX_1, OUTPUT);
  pinMode(MUX_2, OUTPUT);
  pinMode(MUX_3, OUTPUT);

  pinMode(DEMUX_0, OUTPUT);
  pinMode(DEMUX_1, OUTPUT);
  pinMode(DEMUX_2, OUTPUT);
  pinMode(DEMUX_3, OUTPUT);

  digitalWrite(MUX_0, LOW);
  digitalWrite(MUX_1, LOW);
  digitalWrite(MUX_2, LOW);
  digitalWrite(MUX_3, LOW);

  digitalWrite(DEMUX_0, LOW);
  digitalWrite(DEMUX_1, LOW);
  digitalWrite(DEMUX_2, LOW);
  digitalWrite(DEMUX_3, LOW);

  pinMode(DEMUX_EN_1, OUTPUT);
  pinMode(DEMUX_EN_2, OUTPUT);
  pinMode(DEMUX_EN_3, OUTPUT);

  digitalWrite(DEMUX_EN_1, HIGH);
  digitalWrite(DEMUX_EN_2, HIGH);
  digitalWrite(DEMUX_EN_3, HIGH);

  analogWriteResolution(10);
  analogReadResolution(10);


  //Switches

  pinMode(RECALL_SW, INPUT_PULLUP); //On encoder
  pinMode(SAVE_SW, INPUT_PULLUP);
  pinMode(SETTINGS_SW, INPUT_PULLUP);
  pinMode(BACK_SW, INPUT_PULLUP);

  pinMode(GLIDE_SW, INPUT);
  pinMode(FILTERKEYTRACK_SW, INPUT);
  pinMode(FILTERPOLE_SW, INPUT);
  pinMode(FILTERLOOP_SW, INPUT);
  pinMode(FILTERINV_SW, INPUT);
  pinMode(FILTERVEL_SW, INPUT);
  pinMode(VCALOOP_SW, INPUT);
  pinMode(VCAVEL_SW, INPUT);
  pinMode(VCAGATE_SW, INPUT);
  pinMode(LFO_ALT_SW, INPUT);
  pinMode(CHORUS2_SW, INPUT);
  pinMode(CHORUS1_SW, INPUT);
  
}
