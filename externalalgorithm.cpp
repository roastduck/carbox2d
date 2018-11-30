#include <csignal>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include "externalalgorithm.h"

ExternalAlgorithm::ExternalAlgorithm() : Algorithm(1) {
    for (int j = 0; j < 16; j++) {
        colors[0][j][RED] = 0;
        colors[0][j][GREEN] = 0;
        colors[0][j][BLUE] = 0;
    }

	if (pipe(inpipefd) == -1) {
        fputs("Error createing in pipe\n", stderr);
        exit(-1);
    }
	if (pipe(outpipefd) == -1) {
        fputs("Error createing out pipe\n", stderr);
        exit(-1);
    }
	pid = fork();
	if (pid == 0) { // Child
		dup2(outpipefd[0], STDIN_FILENO);
		dup2(inpipefd[1], STDOUT_FILENO);

		// ask kernel to deliver SIGTERM in case the parent dies
		prctl(PR_SET_PDEATHSIG, SIGTERM);

        fputs("External program ready\n", stderr);
		// replace tee with your process
		execlp("python3", "python3" /* arg0 */, "main.py" /* arg1 */, (char*) NULL);
		// Nothing below this line should be executed by child process. If so,
		// it means that the execl function wasn't successfull, so lets exit:
        fputs("External program returned\n", stderr);
		exit(-1);
	}
    // Parent
	// close unused pipe ends
	close(outpipefd[0]);
	close(inpipefd[1]);

    readChromos();
}

ExternalAlgorithm::~ExternalAlgorithm() {
    close(outpipefd[1]);
    close(inpipefd[0]);
	kill(pid, SIGKILL); // send SIGKILL signal to the child process
    int status;
	waitpid(pid, &status, 0);
}

void ExternalAlgorithm::nextGeneration() {
    writeScores();
    readChromos();
}

void ExternalAlgorithm::readChromos() {
    auto bytes = read(inpipefd[0], chromos[0], sizeof(float) * CHROMOS_SIZE);
    if (bytes != CHROMOS_SIZE * sizeof(float)) {
        fputs("Error reading chromosome\n", stderr);
        exit(-1);
    }
}

void ExternalAlgorithm::writeScores() {
    if (write(outpipefd[1], &scores[0], sizeof(float)) != sizeof(float)) {
        fputs("Error writeing score\n", stderr);
        exit(-1);
    }
    if (write(outpipefd[1], &times[0], sizeof(float)) != sizeof(float)) {
        fputs("Error writeing time\n", stderr);
        exit(-1);
    }
    // No need to flush here?
}

