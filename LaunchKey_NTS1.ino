/* DUY Code:  LaunchKey NTS1 Akta3d */
/* CAUTION : GENERATED FROM ARDUINO IDE CODE: REMOVE RK002_DECLARE_INFO / APP_[DEFINES] */

/* 
 * Use the RK002/DUY editor : https://duy.retrokits.com/coder.php?guid=2dbeab7a-b6fd-4ae0-a45f-6cdcff27a418 
 * 
 * Select :
 * Hardware layout : 10K code / 4K flashmem
 * MIDI Only
 */


/* 
# RK-002-NTS1-Preset  
Version 0.5  
Author: Akta3d : https://github.com/Akta3d
  
**INSPIRED FROM:** Retrokits LaunchKey NTS1 version 0.3
https://duy.retrokits.com/#

# Goals  
**Korg NTS-1**  
[Website](https://www.korg.com/fr/products/dj/nts_1/index.php)  
Is a programmable synthetizer

As the NTS1 has no internal memory, any changes are lost after switch-off.  
The NTS1 can be controlled via a series of 29 CC messages.  
[Midi implementation](https://cdn.korg.com/us/support/download/files/8b4225f3349d609272fcf7ac868212cc.pdf)  

**Midi cable RK-002**
[Website](https://www.korg.com/fr/products/dj/nts_1/index.php)  
  
The Retrokits RK-002 cable is a Midi cable incorporating an (arduino-type) chip for interaction with midi events.  
  
**Purpose**
With this sketch you can define any MIDI controller to send out NTS1 CC values and the RK002 keeps track of these and can store them in it's own memory.  
A simple MIDI command can playback these presets and set your NTS1 back to your likings at once. You can store 11 presets in the cable (just because 32 is such a BTDT number :-p )  
  
Note that your custom NTS1 oscilliators/FX are not part of the standardized CC messages so your NTS1 with expansions will not necessarily sound the same on the NTS1 from someone else! This sketch only saves your NTS1 presets, it does not store/copy custom oscilliators/FX or such.  
  
# History
Initially, this script was designed to work with the Novation LaunchKey Mini MK3 (LKM3) Midi controller with a specific controller configuration.  
  
Without this controller or without this configuration, the script is not very practical. The original Retrokits code (LaunchKey NTS1) has been modified to work with any controller and any configuration.  
  
# How it works
The RK-002 cable memorizes all changes in value of the 29 NTS1 controllers.  
  
**Preset = 29 CC control values of the NTS1**  
  
You can save the control value of 29 CC in the cable by sending a specific combination of MIDI controls.  
  
Then you can recall your presets by sending a midi note.  
You can store 11 presets in the RK-002 cable.  
  
# How to use it
  
**Step 1: Midi controller configuration to control the NTS1**
- Configure your controller so that it can send values to the NTS1's 29 controls.
See the [NTS1 Midi implementation](https://cdn.korg.com/us/support/download/files/8b4225f3349d609272fcf7ac868212cc.pdf)  
  
**Step 2: Loading firmware into the RK-002 cable**  
- Go to the [DUY retrokit website](https://duy.retrokits.com/coder.php)
- Connect your cable to your computer's Midi interface
- Copy the code into the editor
- Select: Hardware layout: 10K code / 4K flashmem
- Select: MIDI Only
- Click on "Upload to RK-002"
  
**Step 3: Set firmware parameters**  
- **CHANNEL:** Midi channel to use with your NTS1
- **CC_SAVE:** (Default 117) Midi CC control to save a preset.
- **PRESET_CHANNEL:** (Default 10) Midi channel to use to read/write preset.
- **PRESET_MIN_NOTE:** (Default 1) Midi note of preset 1.
- **PRESET_MAX_NOTE:** (Default 11) Midi note of preset 11.  (CAUTION: PRESET_MAX_NOTE - PRESET_MIN_NOTE must equal to 10)
- **CC_VELO_ON_OFF:** (Default 115) Midi CC control to turn On/Off velocity mode which send value to CC 45 (filter depth EG). Maps extra CC under as well, which makes available under velocity
- **CC_MOD:** (Default 0) Midi CC control of the Mod wheel. If greater than 0, remap modulation wheel on other CC 26 (LFO depth) to be able to make tremolo's.
- **CC_MUTE:** (Default 105) Midi CC control mute all notes
- **BOOTPATCH:** Startup preset to send to the NTS1 on boot (updated when you do a preset read/write operation which will be the startup one for your next session).
  
**Step 4: Midi controller configuration to control the RK-002 cable**
- Configure a button/pad for to save preset
  - Control CC: {CC_SAVE}
  - Value: 127 when pressed
  - Channel: {CHANNEL} of the NTS1
- Configure 11 buttons/pads to select/save preset (Called NOTE_PRESET_X in this documentation)
  - Note: {PRESET_MIN_NOTE} to {PRESET_MAX_NOTE}
  - Value: 127 when pressed
  - Channel: {PRESET_CHANNEL}
- Configure a button/pad to switch On/off velocity mode
  - Control CC: {CC_VELO_ON_OFF}
  - Value: 127 when pressed
  - Channel: {CHANNEL} of the NTS1
- Configure the Mod Wheel CC
  - Control CC: {CC_MOD}
  - Channel: {CHANNEL} of the NTS1
- Configure the Mute CC
  - Control CC: {CC_MUTE}
  - Value: 127 when pressed
  - Channel: {CHANNEL} of the NTS1

**Step 5: Connect**
- Connect the cable between your controller and the NTS1 (orange plug to NTS1)
  
**Step 6: Saving your preset**
- Use your controller to set up the NTS1 to obtain the desired sound.
- Press CC_SAVE and NOTE_PRESET_X of the preset to be saved
- Release CC_SAVE
  
**Step 7: Loading a preset**
- Press NOTE_PRESET_X corresponding to the desired preset
  
# TODO
- Enable/Disable program change
 */
 
// ******************************************
// Compile with RK002 lib v1.9+
// Select at least 4K flash data option in RK002 compile options (Arduino IDE -> tools -> memory layout )
// ******************************************

RK002_DECLARE_PARAM(CHANNEL, 1, 0, 16, 1)
RK002_DECLARE_PARAM(CC_SAVE, 1, 0, 127, 117)
RK002_DECLARE_PARAM(PRESET_CHANNEL, 1, 0, 16, 1)
RK002_DECLARE_PARAM(PRESET_MIN_NOTE, 1, 0, 127, 5)
RK002_DECLARE_PARAM(PRESET_MAX_NOTE, 1, 0, 127, 15)
RK002_DECLARE_PARAM(CC_VELO_ON_OFF, 1, 0, 127, 115)
RK002_DECLARE_PARAM(CC_MOD, 1, 0, 127, 0)
RK002_DECLARE_PARAM(CC_MUTE, 1, 0, 127, 105)
RK002_DECLARE_PARAM(BOOTPATCH, 1, 0, 31, 0)

RK002_DECLARE_INFO("LaunchKey NTS1 Akta3D","Akta3D","0.1","80812b8f-7b9e-4c81-a143-43eaa8681c4a")
// https://www.guidgenerator.com/online-guid-generator.aspx

#define PATCHLEN 30 /* 29 NTS1 CC control + 1 for FX_002 to control velocity On/Off */
#define TOTALPATCHES 11

#define FX_002  0
#define EG_TYP  14
#define EG_ATK  16
#define EG_REL  19

#define TRM_DP  20
#define TRM_RT  21

#define LFO_RT  24
#define LFO_DP  26

#define MOD_TP  88
#define MOD_RT  28
#define MOD_DP  29

#define DLY_TM  30
#define DLY_DP  31
#define DLY_MX  33

#define RVB_TM  34
#define RVB_DP  35
#define RVB_MX  36

#define FLT_TP  42
#define FLT_CT  43
#define FLT_RS  44
#define FLT_DP  45
#define FLT_RT  46

#define OSC_TP  53
#define OSC_SP  54
#define OSC_AT  55

#define DLY_TP  89
#define RVB_TP  90

#define ARP_PT  117
#define ARP_IT  118
#define ARP_LN  119


// define specific value to detect if tables are already present in RK002 memory
#define FLASH_SIGNATURE     0xDEADBEAD

byte _defaultChannel = 0; // receives nrpn settings
byte _ccSave = 117;
byte _presetChannel = 9; // default channel 10 (nts1 - 1) 
byte _presetMinNote = 5;
byte _presetMaxNote = 15;
byte _ccVeloOnOff = 115;
byte _ccVeloChangeNts1 = 45; // Filter Depth
byte _ccMod = 1;
byte _ccModChangeNts1 = 26; // LFO Depth
byte _ccMute = 105;

byte _activepatch = 0;
bool _recording = false;
bool _velofx = false;

byte _workmem[PATCHLEN];

static byte _patchmatch[PATCHLEN] = {FX_002,EG_TYP,EG_ATK,EG_REL,TRM_DP,TRM_RT,LFO_DP,LFO_RT,MOD_DP,MOD_RT,DLY_TM,DLY_DP,DLY_MX,RVB_TM,RVB_DP,RVB_MX,FLT_TP,FLT_CT,FLT_RS,FLT_DP,FLT_RT,OSC_TP,OSC_SP,OSC_AT,MOD_TP,DLY_TP,RVB_TP,ARP_PT,ARP_IT,ARP_LN};
//                                     0 .    1 .   2 .    3 .     4 .    5 .    6 .   7 .    8 .    9 .    10 .   11 .   12 .   13 .   14 .   15 .   16 .   17 .   18 .   19 .   20 .   21 .   22 .   23 .   24 .   25 .   26 .    27 .   28 .   29 

// define memory table structure
struct
{
    uint32_t signature;
    byte patches[TOTALPATCHES][PATCHLEN];  
} flashdata;


void getMemPreset(byte nr) {
    _activepatch = nr;
    RK002_paramSet(BOOTPATCH, _activepatch);
    for(byte i = 0 ; i < PATCHLEN ; i++) {
        _workmem[i] = flashdata.patches[nr][i];
        if(i == 0) {
            _velofx = (_workmem[FX_002] == 1);
        } else {
            RK002_sendControlChange(_defaultChannel, _patchmatch[i], flashdata.patches[nr][i]);
        }
    }
}

//reads / initializes memory with lookup tables
void recallMapsFromFlash() {
    int res = RK002_readFlash(0, sizeof(flashdata), (byte*)&flashdata);

    // init flash if readback failed:
    if ((res != 0) || (flashdata.signature != FLASH_SIGNATURE)) {
        RK002_printf("init patches");

        // initialize
        for (byte i = 0; i < TOTALPATCHES; i++) {
            for (byte j = 0; j < PATCHLEN; j++) {
                flashdata.patches[i][j] = 0;
                if(j==17) flashdata.patches[i][j] = 127; // filter cutoff full
                if(j==19) flashdata.patches[i][j] = 64; // filter depth 0
                if(j==4) flashdata.patches[i][j] = 64; // tremolo depth 0
                if(j==6) flashdata.patches[i][j] = 64; // lfo int 0
                if(j==7) flashdata.patches[i][j] = 77; // lfo rate
            }
        }

        flashdata.signature = FLASH_SIGNATURE;    
        RK002_writeFlash(0, sizeof(flashdata), (byte*)&flashdata);
    } else {
        getMemPreset(RK002_paramGet(BOOTPATCH));
    }
}

//writes table data to RK002 memory
void storeMemPreset(byte nr) {
    _activepatch = nr;
    for(byte i = 0 ; i < PATCHLEN ; i++) {
        flashdata.patches[_activepatch][i] = _workmem[i];
    }

    RK002_paramSet(BOOTPATCH, _activepatch);
    RK002_writeFlash(0, sizeof(flashdata), (byte*)&flashdata);
}

bool RK002_onControlChange(byte channel, byte nr, byte val) {
    if(channel != _defaultChannel) {
        return true;
    }

    // save (record button)
    if(nr == _ccSave) { 
        if(val == 127) { // pressed
            _recording = true;
        } else {
            _recording = false;
        }
        return false;
    }

    
    if(nr == _ccMod) {
        // modwheel
        if(_ccModChangeNts1 > 0) {
            RK002_sendControlChange(_defaultChannel, _ccModChangeNts1, 64 - (val / 20));
        }
        
        return false;
    }

    if(nr == _ccMute) { // solo mute button sends all notes off
        if(val == 127) { // pressed
            for(byte noff = 123 ; noff < 128 ; noff++) {
                RK002_sendControlChange(_defaultChannel, noff, 0);
            }
        }
        return false;
    }

    if(nr == _ccVeloOnOff) { // > button toggles note velocity/cc fx
        if(val == 127) { // pressed
            _velofx =! _velofx;
            _workmem[FX_002] = _velofx;
        }
        return false;
    }    

    // save new CC value in _workmem
    for(byte i = 0 ; i < PATCHLEN ; i++) {
        if(_patchmatch[i] == nr){
            _workmem[i] = val;
            break;
        }
    }

    return true;
}
/*
bool RK002_onProgramChange(byte channel, byte nr) {
    bool retval = true;
    if(channel ==_defaultChannel) {
        if(nr < TOTALPATCHES) {
            getMemPreset(nr);
            retval = false;
        }
    }
    return retval;
}
*/
bool RK002_onNoteOff(byte channel, byte nr, byte vel) {
    if(channel == _presetChannel) {    
        if((nr >= _presetMinNote) and (nr <= _presetMaxNote)) {
            if(_recording == true) { // not record pressed
                _recording = false;
                storeMemPreset(nr - _presetMinNote);
            } else {
                getMemPreset(nr - _presetMinNote);
            }
        }
    }
    return true;  
}


bool RK002_onNoteOn(byte channel, byte nr, byte vel) {
    
    if(channel == _defaultChannel) {
        if(_ccVeloChangeNts1 > 0) {
            if(_velofx) RK002_sendControlChange(_defaultChannel, _ccVeloChangeNts1, vel);
        }
    }
    
    if(channel == _presetChannel) {
        if((nr >= _presetMinNote) and (nr <= _presetMaxNote)) return false;
    }

    return true;  
}


void updateParams() {
    _defaultChannel = RK002_paramGet(CHANNEL) - 1;
    _ccSave = RK002_paramGet(CC_SAVE);
    _presetChannel = RK002_paramGet(PRESET_CHANNEL) - 1;
    _presetMinNote = RK002_paramGet(PRESET_MIN_NOTE);
    _presetMaxNote = RK002_paramGet(PRESET_MAX_NOTE);
    _ccVeloOnOff = RK002_paramGet(CC_VELO_ON_OFF);
    _ccMod = RK002_paramGet(CC_MOD);
    _ccMute = RK002_paramGet(CC_MUTE);
    _activepatch = RK002_paramGet(BOOTPATCH);
}

/* Parameter handlers */
void RK002_onParamChange(unsigned param_nr, int val) {
    updateParams();
}

void setup() {
    updateParams();
    recallMapsFromFlash();
}

void loop() {
}
