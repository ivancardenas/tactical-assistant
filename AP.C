#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <chrono>

#include "../RF24/RF24.h"

using namespace std;

RF24 radio(22,0);

unsigned long timeoutPeriod = 3000000;

const uint64_t pipes[2] = { 0x7878787878LL, 0xB3B4B5B6F1LL };



int main(int argc, char** argv) {

	radio.begin();
	radio.setChannel(110);
	radio.setPALevel(RF24_PA_MIN);
	radio.setDataRate(RF24_250KBPS);

	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1, pipes[0]);

	radio.printDetails();

	while (1) {

		unsigned long data = 10101010;

		radio.stopListening();
		//if (radioNumber) {
		// auto init_time = chrono::high_resolution_clock::now();

		/* if (!radio.write(&data, sizeof(unsigned long)))
			printf("Failed, could not send.");

		fflush(stdout); // Cleaning output.*/

		if (radio.write(&data, sizeof(unsigned long))) {
			printf(".");
		} else {
			printf("?");
		}

			fflush(stdout);
		// } else {*/


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
				unsigned long init_time_r;
				radio.read(&init_time_r, sizeof(unsigned long));
				//auto end_time = chrono::high_resolution_clock::now();
				printf(init_time_r);
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
