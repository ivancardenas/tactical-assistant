#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <chrono>

#include "../RF24/RF24.h"

using namespace std;
using namespace std::chrono;

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

		/* auto start = high_resolution_clock::now();
		auto ended = high_resolution_clock::now();

		unsigned long long stime = chrono::duration_cast
									<chrono::nanoseconds>(ended - start).count(); */

		radio.stopListening();

		auto start = high_resolution_clock::now();
		uint64_t stn = duration_cast // Possibly "unsigned long long"
			<nanoseconds>(start.time_since_epoch()).count() /* - 1493014497000000000*/;

		printf(" Mande: %llu\n", stn);

		if (!radio.write(&stn, sizeof(unsigned long long))) {}

		// fflush(stdout);

		radio.startListening();

		while (!radio.available() && timeout < 1000) timeout++;

		radio.read(&rtime, sizeof(unsigned long long));

		auto ended = high_resolution_clock::now();
		uint64_t etn = duration_cast // Possibly "unsigned long long"
			<nanoseconds>(ended.time_since_epoch()).count() /* - 1493014497000000000*/;

		unsigned long long res = etn - stn;
		printf("Received: %llu \n", etn);

		// fflush(stdout);

		timeout = 0;
	}
	return 0;
}
