#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "../RF24/RF24.h"

using namespace std;

RF24 radio(22,0);

bool radioNumber = 0;
unsigned long timeoutPeriod = 3000;  

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   
char data[32] = {"_A message from RPi w/ NRF24L+!"};   

void showData(void) {
	printf("Data: ");
	for(int i = 0; i < 32; i++) {
		if (isprint(data[i])) printf("%c", data[i]);
		else printf(".");
	}
	printf("\n\r");
}

int main(int argc, char** argv) {
	const int role_rx = 0, role_tx = 1;
	int role = role_rx;

	printf("\n ************ Role Setup ***********\n");
	string input = "";
	char myChar = {0};

	cout << "Choose a role: Enter 0 for Rx, 1 for Tx (CTRL+C to exit) \n>";
	getline(cin,input);

	if (input.length() == 1) {
		myChar = input[0];
		if (myChar == '0') {
			cout << "Role: Rx " << endl << endl;
		} else {
			cout << "Role: Tx " << endl << endl;
		        role = role_tx;
	        }
	}
	
	switch(role) {
		case role_rx :
			radioNumber = 0;
			break;
		case role_tx :
			radioNumber=1;
			 break;								}

	radio.begin();
	radio.setChannel(110);
	radio.setDataRate(RF24_250KBPS);
	radio.setPALevel(RF24_PA_MIN);

	if (!radioNumber) {
		radio.openWritingPipe(pipes[0]);
		radio.openReadingPipe(1,pipes[1]);
		memset(&data,'\0',sizeof(data));
		radio.startListening();
	} else {
		radio.openWritingPipe(pipes[1]);
		radio.openReadingPipe(1,pipes[0]);
		radio.stopListening();							}

	radio.printDetails();
	printf("Start loop:\n");
	
	while (1) {
		if (radioNumber) {
			if (radio.writeBlocking(&data,sizeof(data),timeoutPeriod)) {
				printf(".");
			} else {
				printf("?");
			}
			
			fflush(stdout);
		} else {
			if (radio.available()) {
				radio.read(&data,32);
				showData();
				fflush(stdout);
			}
		}
		
		delay(5);
	}
	return 0;
}
