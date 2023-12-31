# RK-002-NTS1-Preset  
Version 0.7  
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
- **ENABLE_PROGRAM_CHANGE:** (Default 0) If 1, allow to change preset with Midi Program Change message
- **CC_AFTER_TOUCH:** (Default 2) Midi CC control of the after touch, allow to send 127 value to CC 43 (Filter CutOff), then restore previous value on release aftertouch
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
- Configure the After touch CC
  - Control CC: {CC_AFTER_TOUCH}
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