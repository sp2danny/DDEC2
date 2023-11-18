
#include <iostream>
#include <cstdio>

#include "portaudio.h"


int main()
{
	std::freopen("/dev/null", "a", stderr);
	
	auto err = Pa_Initialize();
	if (err != paNoError) {
		std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
	}

	
	
	std::cout << "Hello PA World\n";
	
	
	err = Pa_Terminate();
	if (err != paNoError) {
		std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
	}

}