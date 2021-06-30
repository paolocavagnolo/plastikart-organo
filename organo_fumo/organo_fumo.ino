#include <usbh_midi.h>
#include <usbhub.h>

USB Usb;
USBHub Hub(&Usb);
USBH_MIDI  Midi(&Usb);

void MIDI_poll();

bool rS[12] = {1,1,1,1,1,1,1,1,1,1,1,1};

void onInit()
{
  char buf[20];
  uint16_t vid = Midi.idVendor();
  uint16_t pid = Midi.idProduct();
  sprintf(buf, "VID:%04X, PID:%04X", vid, pid);
  Serial.println(buf); 
}

void setup() {

  Serial.begin(115200);

  pinMode(13,OUTPUT);

  while (Usb.Init() == -1) {
    delay(1000);
    digitalWrite(13,HIGH);
  }
  digitalWrite(13,LOW);

  // Register onInit() function
  Midi.attachOnInit(onInit);
  
  // put your setup code here, to run once:
  for (int i=48;i>25;i=i-2) {
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
  }
}

uint8_t i,j;

void loop() {
  // put your main code here, to run repeatedly:

  Usb.Task();
  if ( Midi ) {
    MIDI_poll();
  }

  j = 0;
  
  for (i=26;i<49;i=i+2) {
    
    digitalWrite(i,rS[j]);
    j++;
    
  }

  
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  char buf[16];
  uint8_t bufMidi[MIDI_EVENT_PACKET_SIZE];
  uint16_t  rcvd;

  if (Midi.RecvData( &rcvd,  bufMidi) == 0 ) {

    if (bufMidi[0] == 9) {
      rS[bufMidi[2]%12] = 0;
    }
    else {
      rS[bufMidi[2]%12] = 1;
    }
 
  }
}
