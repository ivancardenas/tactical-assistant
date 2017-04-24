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

		unsigned long long stime = chrono::duration_cast
									<chrono::nanoseconds>(ended - start).count();

		printf("Sent: %llu \n", stime);

		radio.stopListening();

		if (radio.available())
			if (radio.write(&stime, sizeof(unsigned long long)))
				printf("Sent \n");

		fflush(stdout);

		radio.startListening();

		if (radio.available()) {

			unsigned long long init_time_r;
			radio.read(&init_time_r, sizeof(unsigned long long));
			printf("Received: %llu \n", init_time_r);
			fflush(stdout);
		}
		delay(5);
	}
	return 0;
}
