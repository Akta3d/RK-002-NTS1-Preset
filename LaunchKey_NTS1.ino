/* DUY Code:  LaunchKey NTS1
/* CAUTION : GENERATED FROM ARDUINO IDE CODE: REMOVE RK002_DECLARE_INFO / APP_[DEFINES] */



/* 
 *  WORK IN PROGRESS!
 *  firmware 0.3 fixes start/stop for record now novation has changed the CC117 midi channel
 * The NTS1 can be controlled via a series of CC messages. With this sketch you can define any MIDI controller to send out NTS1 CC values and the RK002 keeps track of these and can store them in it's own memory. We have tailored it for the  Novation LaunchKey Mini MK3  (LKM3) to have a nice editing experience with the NTS1
 * A simple MIDI Program Change Message can playback these patches and set your NTS1 back to your likings at once. You can store 11 patches in the cable (just because 32 is such a BTDT number :-p )
 * 
 * You can download the LKM3 template here: https://retrokits.com/rk002/exchange/nts1remote.zip
 * Demo video at https://www.youtube.com/watch?v=-j6-zb1vQds
 * 
 * 
 * PARAMETERS:
 * CHANNEL MIDI channel of the NTS1
 * BOOTPATCH = startup patch to send to the NTS1 on boot (updated when you do a patch read/write operation wich will be the startup one for your next session)
 * VELO_CC = maps extra CC under as well, by default set to 45, which makes filter EG available under velocity
 * MOD_CC = remap modulation wheel on other CC, default set to 26, LFO_DP to be able to make tremolo's
 * these remap effects can be muted with the Stop/Solo/Mute button on the LKM3 or sending CC #0 value 0 or 1, they are stored with the preset
 * 
 * Note that your custom NTS1 oscilliators/FX are not part of the standardized CC messages so your NTS1 with expansions will not necessarily sound the same on the NTS1 from someone else! This DUY only saves your NTS1 patches, it does not store/copy custom oscilliators/FX or such.
 * 
 * CONTROL MESSAGES FOR THE RK002:
 * LMK3 pots send out on midi channel 1, CC60-67
 * The LMK3 pads sends notes 0-4 on MIDI channel 10 to change the dial mapping, notes 5-15 will do quick preset changes
 * velocity FX on/off is CC 115
 * record button is CC 117, keep record pressed and press one of the pads to store a preset
 * 
 * 
 */
 
// ******************************************
// Compile with RK002 lib v1.9+
// Select at least 4K flash data option in RK002 compile options (Arduino IDE -> tools -> memory layout )
// ******************************************

RK002_DECLARE_PARAM(CHANNEL,1,0,16,1)
RK002_DECLARE_PARAM(BOOTPATCH,1,0,31,0)
RK002_DECLARE_PARAM(VELO_CC,1,0,127,45)
RK002_DECLARE_PARAM(MOD_CC,1,0,127,26)

RK002_DECLARE_INFO("LaunchKey NTS1","Retrokits","0.3","34b498da-a40b-44ec-958d-ac4ca6437566")
//https://www.guidgenerator.com/online-guid-generator.aspx

#define PATCHLEN 30
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

byte midichn=0; // receives nrpn settings
byte activepatch=0;
byte activeboard=0;
bool recording=false;
bool velofx=false;

byte workmem[PATCHLEN];
static byte boards[5][8] = {
                              {OSC_TP,OSC_SP,OSC_AT,EG_TYP,EG_ATK,EG_REL,FLT_CT,FLT_RS},
                              {LFO_DP,LFO_RT,TRM_RT,TRM_DP,EG_ATK,EG_REL,FLT_CT,FLT_RS},
                              {MOD_TP,MOD_DP,MOD_RT,FLT_TP,FLT_DP,FLT_RT,FLT_CT,FLT_RS},
                              {DLY_TP,DLY_TM,DLY_DP,DLY_MX,RVB_TP,RVB_TM,RVB_DP,RVB_MX},
                              {ARP_PT,ARP_IT,ARP_LN,MOD_DP,DLY_MX,RVB_MX,FLT_CT,FLT_RS}
                            };
static byte patchmatch[PATCHLEN] = {FX_002,EG_TYP,EG_ATK,EG_REL,TRM_DP,TRM_RT,LFO_DP,LFO_RT,MOD_DP,MOD_RT,DLY_TM,DLY_DP,DLY_MX,RVB_TM,RVB_DP,RVB_MX,FLT_TP,FLT_CT,FLT_RS,FLT_DP,FLT_RT,OSC_TP,OSC_SP,OSC_AT,MOD_TP,DLY_TP,RVB_TP,ARP_PT,ARP_IT,ARP_LN};
//                                     0 .    1 .   2 .    3 .     4 .    5 .    6 .   7 .    8 .    9 .    10 .   11 .   12 .   13 .   14 .   15 .   16 .   17 .   18 .   19 .   20 .   21 .   22 .   23 .   24 .   25 .   26 .    27 .   28 .   29 
// define memory table structure
struct
{
  uint32_t signature;
  byte patches[TOTALPATCHES][PATCHLEN];  
} flashdata;



//reads / initializes memory with lookup tables
void recallMapsFromFlash()
{
  int res = RK002_readFlash(0,sizeof(flashdata),(byte*)&flashdata);

  // init flash if readback failed:
  if ((res!=0) || (flashdata.signature != FLASH_SIGNATURE))
  {
    RK002_printf("init patches");

    // initialize
    for (byte i=0; i<TOTALPATCHES; i++)
    {
      for (byte j=0; j<PATCHLEN; j++)
      {
        flashdata.patches[i][j] = 0;
        if(j==17) flashdata.patches[i][j] = 127; // filter cutoff full
        if(j==19) flashdata.patches[i][j] = 64; // filter depth 0
        if(j==4) flashdata.patches[i][j] = 64; // tremolo depth 0
        if(j==6) flashdata.patches[i][j] = 64; // lfo int 0
        if(j==7) flashdata.patches[i][j] = 77; // lfo rate
      }
    }

    flashdata.signature = FLASH_SIGNATURE;    
    RK002_writeFlash(0,sizeof(flashdata),(byte*)&flashdata);
  }else{
    //RK002_printf("recall patches");
    getMemPreset(RK002_paramGet(BOOTPATCH));
  }
}

void getMemPreset(byte nr){
  activepatch=nr;
  RK002_paramSet(BOOTPATCH,activepatch);
  for(byte i=0;i<PATCHLEN;i++){
     workmem[i]=flashdata.patches[nr][i];
     if(i==0){
      velofx = (workmem[FX_002]==1);
     }else{
       RK002_sendControlChange(midichn,patchmatch[i],flashdata.patches[nr][i]);
     }
  }
   
}

//writes table data to RK002 memory
void storeMemPreset(byte nr)
{
  activepatch=nr;
  for(byte i=0;i<PATCHLEN;i++){
    flashdata.patches[activepatch][i]=workmem[i];
  }
  RK002_paramSet(BOOTPATCH,activepatch);
  RK002_writeFlash(0,sizeof(flashdata),(byte*)&flashdata);
  //RK002_writeFlash(activepatch*PATCHLEN,PATCHLEN,(byte*)&workmem);
}

void setBoard(byte nr){
  activeboard=nr;
}

bool RK002_onControlChange(byte channel, byte nr, byte val)
{
  bool retval=true;
  // novation fixed cc 117 to ch 16 now, so we just check it on any channel now...
  if(nr==117){//save (record button)
      retval=false;
      if(val==127){ // pressed
        recording=true;
      }else{
        recording=false;
      }
    }
  if((channel==16) or (channel==midichn)){
    if(nr==1){
      // modwheel
      if(RK002_paramGet(MOD_CC)>0){
        if(velofx) RK002_sendControlChange(midichn,RK002_paramGet(MOD_CC),64-(val/20));
        retval=false;
      }
    }
    
    if(nr==105){// solo mute button sends all notes off
      retval=false;
      if(val==127){ // pressed
        for(byte noff=123;noff<128;noff++){
          RK002_sendControlChange(midichn,noff,0);
        }
      }
    }
    if(nr==104){// > button toggles note velocity/cc fx
      retval=false;
      if(val==127){ // pressed
        velofx=!velofx;
        workmem[FX_002]=velofx;
      }
    }
    if(retval){
      for(byte i=0;i<PATCHLEN;i++){
        if(patchmatch[i]==boards[activeboard][nr-60]){
          workmem[i]=val;
          RK002_sendControlChange(midichn,patchmatch[i],val);
          retval=false;
          break;
        }
      }
    }
  }
  return retval;
}

bool RK002_onProgramChange(byte channel, byte nr){
  bool retval=true;
  if((channel==16) or (channel==midichn)){
    if(nr<TOTALPATCHES){
      getMemPreset(nr);
      retval=false;
    }
  }
  return retval;
}

bool RK002_onNoteOff(byte ch, byte nr, byte vel){
  if(ch==9){
    if(nr<5){
      setBoard(nr);
      //return false;
    }
    if((nr>4) and (nr<16)){
      if(recording==true){ // not record pressed
        recording=false;
        storeMemPreset(nr-5);
      }else{
        getMemPreset(nr-5);
      }
      //return false;
    }
  }
  return true;  
}
bool RK002_onNoteOn(byte ch, byte nr, byte vel){
  if((ch==16) or (ch==midichn)){
    if(RK002_paramGet(VELO_CC)>0){
      if(velofx) RK002_sendControlChange(midichn,RK002_paramGet(VELO_CC),vel);
    }
  }
  if(ch==9){
    if(nr<16) return false;
  }
  return true;  
}
/* Parameter handlers */
void RK002_onParamChange(unsigned param_nr, int val){
  updateParams();
}
void updateParams(){
    midichn = (RK002_paramGet(CHANNEL) == 0) ? 16 : RK002_paramGet(CHANNEL)-1;
    activepatch = RK002_paramGet(BOOTPATCH);
}
void setup() 
{
  updateParams();
  recallMapsFromFlash();
}

void loop() 
{
}
