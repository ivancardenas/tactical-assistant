#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <chrono>

#include "../RF24/RF24.h"

using namespace std;

RF24 radio(22, 0);

unsigned long timeoutPeriod = 3000000;

const uint64_t pipes[2] = { 0x7878787878LL, 0xB3B4B5B6F1LL };

int main(int argc, char** argv) {

	// Configuring nRF24L01+
	radio.begin();
	radio.setChannel(110);
	radio.setPALevel(RF24_PA_MIN);
	radio.setDataRate(RF24_250KBPS);
	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1, pipes[0]);

	// radio.printDetails();

	while (true) {

		auto start = chrono::high_resolution_clock::now();
		auto ended = chrono::high_resolution_clock::now();
		unsigned long stime = chrono::duration_cast<chrono::nanoseconds>(ended - start).count();

		// unsigned long data = 10101010;
		radio.stopListening();

		if (radio.write(&stime, sizeof(unsigned long)))
			printf(".");
		else printf("?");

		fflush(stdout);

		radio.startListening();

		if (radio.available()) {

			unsigned long init_time_r;
			radio.read(&init_time_r, sizeof(unsigned long));
			printf("%lu",init_time_r);
			fflush(stdout);
		}
		delay(5);
	}
	return 0;
}
