#include "window.h"
#include "draw.h"
#include "timing.h"
#include "input.h"
#include "common.h"
#include "mathlib.h"
#include "ui.h"
#include "linked_list.h"

#include <stdio.h>

struct DemoHeader {
	char header[8];
	int demoProtocol;
	int networkProtocol;
	char serverName[260];
	char clientName[260];
	char mapName[260];
	char gameDirectory[260];
	float playbackTime;
	int ticks;
	int frames;
	int signOnLength;
};

int main(int argc, char *argv[]) {
	void *demoFile = ReadFileContent("demo.dem");

	if(!demoFile) {
		printf("error reading demo file\n");
		return 0;
	}
	else {
		printf("read demo file\n");
	}

	struct DemoHeader *demoHeader = demoFile;
	printf("header: %s\n", demoHeader->header);
	printf("demo protocol: %i\n", demoHeader->demoProtocol);
	printf("network protocol: %i\n", demoHeader->networkProtocol);
	printf("server name: %s\n", demoHeader->serverName);
	printf("client name: %s\n", demoHeader->clientName);
	printf("map name: %s\n", demoHeader->mapName);
	printf("game directory: %s\n", demoHeader->gameDirectory);
	printf("playback time: %f\n", demoHeader->playbackTime);
	printf("ticks: %i\n", demoHeader->ticks);
	printf("frames: %i\n", demoHeader->frames);
	printf("sign on length: %i\n", demoHeader->signOnLength);

	free(demoFile);
	
	return 0;
}
