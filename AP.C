#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <chrono>

#include "../RF24/RF24.h"

using namespace std;

RF24 radio(22,0);

unsigned long timeoutPeriod = 3000;

const uint64_t pipes[2] = { 0x7878787878LL, 0xB3B4B5B6F1LL };

unsigned long data = 10101010;

/*void showData(void) {
	printf("Data: ");
	for(int i = 0; i < 32; i++) {
		if (isprint(data[i])) printf("%c", data[i]);
		else printf(".");
	}
	printf("\n\r");
}*/

int main(int argc, char** argv) {
	/*const int role_rx = 0, role_tx = 1;
	int role = role_rx;*/

	/*printf("\n ************ Role Setup ***********\n");
	string input = "";
	char myChar = {0};*/

	/*cout << "Choose a role: Enter 0 for Rx, 1 for Tx (CTRL+C to exit) \n>";
	getline(cin,input);*/

	/*if (input.length() == 1) {
		myChar = input[0];
		if (myChar == '0') {
			cout << "Role: Rx " << endl << endl;
		} else {
			cout << "Role: Tx " << endl << endl;
		        role = role_tx;
	        }
	}*/

	/*switch(role) {
		case role_rx :
			radioNumber = 0;
			break;
		case role_tx :
			radioNumber=1;
			 break;								}*/

	radio.begin();
	radio.setChannel(110);
	radio.setPALevel(RF24_PA_MIN);
	radio.setDataRate(RF24_250KBPS);

	/*if (!radioNumber) {
		radio.openWritingPipe(pipes[0]);
		radio.openReadingPipe(1,pipes[1]);
		memset(&data,'\0',sizeof(data));
		radio.startListening();
	} else {*/
								/*}*/

	radio.printDetails();


	while (1) {
		radio.openWritingPipe(pipes[1]);
		radio.stopListening();
		//if (radioNumber) {
		auto init_time = chrono::high_resolution_clock::now();

		/* if (!radio.write(&data, sizeof(unsigned long)))
			printf("Failed, could not send.");

		fflush(stdout); // Cleaning output.*/

		if (radio.writeBlocking(&data, sizeof(unsigned long), timeoutPeriod)) {
			printf(".");
		} else {
			printf("?");
		}

			fflush(stdout);
		// } else {*/

		radio.openReadingPipe(1, pipes[0]);
		//memset(&data, '\0', sizeof(data)); // ?
		radio.startListening();

		// finalTime
		//auto waiting = chrono::high_resolution_clock::now();
		//bool time_out = false;

		/*while (!radio.available()) {

			if (chrono::duration_cast<chrono::nanoseconds>
				(chrono::high_resolution_clock::now() - waiting).count() > 200000) {
				time_out = true;
				break;
			}
			printf("No answer.");
		}*/

		if (radio.available()) {
				//auto init_time_r = chrono::high_resolution_clock::now();
				radio.read(&data, sizeof(unsigned long));
				//auto end_time = chrono::high_resolution_clock::now();

				//printf("Elapsed time: ");
				//cout << chrono::duration_cast<chrono::nanoseconds>(end_time - init_time).count() << endl;
				//printf(" nanoseconds");

				//showData();
				fflush(stdout); // Cleaning output.
			}

			delay(5);

		}
		return 0;
}
