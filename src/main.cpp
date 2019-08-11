#include "main.h"

uint32_t last_drive = 0;
uint32_t drive_period = 5000; 

struct EHPS jeep_ehps; 

void handleCAN1CB(CAN_FRAME *frame)
{
	if(frame->id == 553 ||
     frame->id == 1359 || 
     frame->id == 585 ||
     frame->id == 297 || 
     frame->id == 816
  ) {
    log_msg(frame);
  }
  
  /* 
  if(frame->id == 553) {
    int message_counter = frame->data.uint8[1]; 
    frame->data.uint8[0] = M3_SHIFTER_DRIVE[message_counter][0];
    frame->data.uint8[1] = M3_SHIFTER_DRIVE[message_counter][1];
    frame->data.uint8[2] = M3_SHIFTER_DRIVE[message_counter][2];
    log_msg(frame);
  }
  */
  // CAN0.sendFrame(*frame);
}

void handleCAN0CB(CAN_FRAME *frame) {
  // CAN1.sendFrame(*frame);
}

void printFrame(CAN_FRAME &frame)
{
	// Print message
	Serial.print("ID: ");
	Serial.println(frame.id,HEX);
	Serial.print("Ext: ");
	if(frame.extended) {
		Serial.println("Y");
	} else {
		Serial.println("N");
	}
	Serial.print("Len: ");
	Serial.println(frame.length,DEC);
	for(int i = 0;i < frame.length; i++) {
		Serial.print(frame.data.uint8[i],HEX);
		Serial.print(" ");
	}
	Serial.println();
}


// prints the message to the console with the bus 
void log_msg(CAN_FRAME* printMsg, uint8_t origin_bus) {
    // Serial.print(float(micros()/1000000));
    Serial.println(micros());
    Serial.print(",");
    Serial.print(printMsg->id); 
    // Serial.print(printMsg->id, HEX);  // for printing in hex
    Serial.print(",");  
    Serial.print(origin_bus);
    Serial.print(","); 
    uint8_t count = printMsg->length;
    for(size_t i = 0; i < count; i++) {    
        if(printMsg->data.uint8[i] < 16) {   // Need to check for the 0X case.
            Serial.print("0"); Serial.print(printMsg->data.uint8[i], HEX);
        } else {
            Serial.print(printMsg->data.uint8[i], HEX); 
        }
    }
    Serial.println();
}

// prints the message to the console with the bus 
void log_msg(CAN_FRAME*  printMsg) {
    //Serial.print((float(micros()/1000000)));
    Serial.print(micros());
    Serial.print(",");
    Serial.print(printMsg->id); 
    // Serial.print(printMsg->id, HEX);  // for printing in hex
    Serial.print(",");  
    uint8_t count = printMsg->length;
    for(size_t i = 0; i < count; i++) {     
        if(printMsg->data.uint8[i] < 16) {   // Need to check for the 0X case.
            Serial.print("0"); Serial.print(printMsg->data.uint8[i], HEX);
        } else {
            Serial.print(printMsg->data.uint8[i], HEX); 
        }
    }
    Serial.println();
}

void call_me_back(CAN_FRAME *frame) {
  //log_msg(frame);
  //Serial.println("CALL BACK!");
}


// prints the message payloads in a form that can be converted to C code directly
// only works for a given can ID  
void print_in_c_form(uint32_t target_id, CAN_FRAME incoming) {
  if (incoming.id == target_id) {
    
    Serial.print("{");
    
    uint8_t count = incoming.length;
    
    for(size_t i = 0; i < count; i++) {     
    
        Serial.print("0x");
    
        if(incoming.data.uint8[i] < 16) {   // Need to check for the 0X case.
            Serial.print("0"); Serial.print(incoming.data.uint8[i], HEX);
        } else {
            Serial.print(incoming.data.uint8[i], HEX); 
        }
    
        if (count - i > 1) {              // print the ", " until the last one 
          Serial.print(", ");
        } 
    
    }
    
    Serial.println("}");
  }
}

void setup() {
	Serial.begin(115200);
	
	Serial.println("Initializing ...");

	// Initialize builtin CAN controller at the specified speed
	if(CAN0.begin(500000))
	{
		Serial.println("Builtin CAN Init OK ...");
	} else {
		Serial.println("BuiltIn CAN Init Failed ...");
	}
	
  //shift_stalk.init(); 

  //jeep_ehps.init();
  //jeep_ehps.init_feedback();

	// Initialize MCP2517FD CAN controller at the specified speed
	/*
  if(CAN1.begin(500000))
	{
		Serial.println("MCP2517FD Init OK ...");
	} else {
		Serial.println("MCP2517FD Init Failed ...");
	}
  */ 

 	// Initialize MCP2515 CAN controller at the specified speed
  if(CAN1.begin(500000))
	{
		Serial.println("MCP2515 Init OK ...");
	} else {
		Serial.println("MCP2515 Init Failed ...");
	} 

	CAN0.setRXFilter(0, 0x100, 0x7FF, false);
  CAN0.setCallback(0, handleCAN0CB);
	CAN0.watchFor(); //allow everything else through

	CAN1.setRXFilter(0, 0x100, 0x7FF, false);
	CAN1.watchFor(); //allow everything else through
	CAN1.setCallback(0, handleCAN1CB);

	Serial.println("Ready ...!");
}


void loop() {
  
  
  
  
  // shift_stalk.transmit(shift_posiiton);
  CAN_FRAME message;

  /* 
  if (millis() - last_drive > drive_period) {
    if (shift_posiiton == DRIVE) {
      shift_posiiton = IDLE;
    } else {
      shift_posiiton = DRIVE; 
    }
    last_drive = millis();
  }
  */ 
  
  
  if(CAN0.read(message)) {    // can1 will be the shift stalk side
      // print_in_c_form(816, message);
    log_msg(&message);
     //  CAN1.sendFrame(message);  // automatically put it right back out. 
  }
    



  /* 
  if(CAN0.read(message)) {
    if( // message.id == 297 ||
        message.id == 297 //  ||
        // message.id == 585 ||
        // message.id == 1359 ||
        // message.id == 816 ||
        // message.id == 904 ||
        // message.id == 808
      ) {
       log_msg(&message);
    } 
    // log_msg(&message);
  }
  */
    
    //printFrame(message);
    //Serial.println("TESTING");


	/* Commenting out for now, not using this 
  if (CAN1.read(message)) {
		printFrame(message);
	}
  */
}



