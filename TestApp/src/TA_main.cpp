//TODO: This file is pretty ugly : )

#include "miscStdHeaders.h"
#include "miscDefines.hpp"
#include "fileHelpers.h"
#include "core.hpp"

#include "timeHelpers.hpp"
#include "tests.hpp"

//***vvv TEST BATTERY DEFINITION AREA: KEPP CONSISTENT vvv****

//TODO: You should be able to just edit the first two ENUMs (maybe make into struct)

enum batteryIDs {SYSTEM, DATA, PRNG, TOTAL};
enum batterySizes { SYSTEM_SZ = 1, DATA_SZ = 1, PRNG_SZ = 2 };

int batterySizes[batteryIDs::TOTAL] = {SYSTEM_SZ, DATA_SZ, PRNG_SZ};

bool resultsBattery0[batterySizes::SYSTEM_SZ];
bool resultsBattery1[batterySizes::DATA_SZ];
bool resultsBattery2[batterySizes::PRNG_SZ];

bool* batteryResults_ptrs[batteryIDs::TOTAL] = { &resultsBattery0[0],  &resultsBattery1[0],
                                                                    &resultsBattery2[0] };

int finalResults[batteryIDs::TOTAL];

//***^^^ TEST BATTERY DEFINITION AREA: KEPP CONSISTENT ^^^****

void printFailedTests(void);
void printOptions(void);
int runTestsBattery(void);
constexpr int getTotalTests(void);

int main(int argc, char **argv) {

	if (argc == 1) {
		LOG_INFO("No arguments entered, will run the tests battery\n\n");
		return runTestsBattery();
	}
	else {
		LOG_ERROR("Bad number of arguments");
		printOptions();
		GETCHAR_FORCE_PAUSE;
		return 1;
	}
}

constexpr int getTotalTests() {
	int total = 0;
	for (int i = 0; i < batteryIDs::TOTAL; i++) {
		total += batterySizes[i];
	}
	return total;
}

void printFailedTests() {
	for (int i = 0; i < batteryIDs::TOTAL; i++) {
		
		auto results_arr = batteryResults_ptrs[i];
		int failed = 0;

		printf("Battery %d Failed tests: ", i);
		for (int j = 0; j < batterySizes[i]; j++) {
			if (results_arr[j] == false) {
				printf("%d; ",j);
				failed++;
			}
		}
		if (!failed) {
			printf("none");
		}
		printf("\n");
	}
}

void printOptions(){

	printf("Program expects zero arguments (default test)\n\n");
	return;
}

void accumulateAndPrintPartialResults(const int battery){
	
	const int batterySize = batterySizes[battery];
	auto batteryResults_arr = &(batteryResults_ptrs[battery][0]);
	
	finalResults[battery] = 0;
	for(int i = 0; i < batterySize; i++) { 
		finalResults[battery] += (int)batteryResults_arr[i];
	}

	if (finalResults[battery] != batterySize) {
		LOG_CRITICAL("Not all of these tests passed:", 1);
		printf("%d out of %d failed", batterySize - finalResults[battery], batterySize);
		GETCHAR_PAUSE;
	}
	else {
		LOG_INFO("All of these tests passed!", 1); 
		GETCHAR_PAUSE;
	}
}

//This is the main payload:

int runTestsBattery(void) {

	bool printSteps = false;
	#if ( (defined AS_DEBUG) || VERBOSE_RELEASE )
		printSteps = true;
	#endif

	LOG_DEBUG("This is a battery of tests for Aux0\n"); GETCHAR_PAUSE;

	LOG_INFO("\tBATTERY 0 - SYSTEM\n", 1); GETCHAR_PAUSE;

	resultsBattery0[0] = AZ::testSnooze(printSteps);

	accumulateAndPrintPartialResults(batteryIDs::SYSTEM);

	LOG_INFO("\tBATTERY 1 - DATA\n", 1); GETCHAR_PAUSE;

	resultsBattery1[0] = AZ::testFlagFields(printSteps);

	accumulateAndPrintPartialResults(batteryIDs::DATA);

	LOG_INFO("\tBATTERY 2 - PRNG\n", 1); GETCHAR_PAUSE;

	resultsBattery2[0] = AZ::testDraw1spcg32();

	resultsBattery2[1] = AZ::testDraw4spcg32s();

	accumulateAndPrintPartialResults(batteryIDs::PRNG);
	
	LOG_DEBUG("Tests ended...\n", 1); GETCHAR_PAUSE;

	int totalPassed = 0;
	for (int i = 0; i < batteryIDs::TOTAL; i++) {
		totalPassed += finalResults[i];
	}
	
	if (totalPassed == getTotalTests()) {
		LOG_INFO("All automatically checked tests passed!", 1); GETCHAR_PAUSE;
	}
	else {
		LOG_CRITICAL("Not all tests were passed!", 1);
		printf("%d out of %d failed\n", getTotalTests() - totalPassed, getTotalTests());

		printFailedTests();

		GETCHAR_PAUSE;
	}
	
	GETCHAR_FORCE_PAUSE;

	AZ::hybridBusySleepForMicros(std::chrono::microseconds(MICROS_IN_A_SECOND));

	LOG_INFO("Done! Enter to exit", 1); GETCHAR_FORCE_PAUSE;
	return (getTotalTests() - totalPassed);
}