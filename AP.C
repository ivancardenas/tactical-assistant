#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <chrono>

#include "../RF24/RF24.h"

using namespace std;

RF24 radio(22, 0);

unsigned int timeout = 0;
unsigned long long rtime;

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
		
		radio.stopListening();

		if (!radio.write(&stime, sizeof(unsigned long long))) {} // OK.
		auto start = chrono::high_resolution_clock::now(); // Take first time.

		// fflush(stdout);

		radio.startListening();

		while (!radio.available() && timeout < 1000) timeout++; // OK.

		radio.read(&rtime, sizeof(unsigned long long));
		auto ended = chrono::high_resolution_clock::now(); // Take final time.

		unsigned long long stime = chrono::duration_cast
									<chrono::nanoseconds>(ended - start).count();

		printf("Received: %llu \n", rtime);

		// fflush(stdout);

		timeout = 0;
	}
	return 0;
}
