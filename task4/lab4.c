#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

pid_t pid_arr[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

void print_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	printf(" %ld\n", tv.tv_usec);
}

void handler0(int nsig) {
	static int processes_ready = 0;
	if (nsig == SIGUSR1) {
		processes_ready++;
		if (processes_ready >= 8) {
			kill(pid_arr[1], SIGUSR2);
			wait(NULL);
			exit(0);	
		}
	}
}

void handler1(int nsig) {
	static int received_signals = 0;
	static int send_usr1 = 0;
	static int send_usr2 = 0;
	if (nsig == SIGUSR2) {
		printf("1 %d %d получил USR2", getpid(), getppid());
		print_time();
		if (++received_signals == 101) {
			kill(-1 * pid_arr[2], SIGTERM);
			waitpid(pid_arr[2], NULL, 0);	
			waitpid(pid_arr[3], NULL, 0);
			waitpid(pid_arr[4], NULL, 0);
			printf("1 %d %d завершил работу после %d USR1, %d USR2\n", getpid(), getppid(), send_usr1, send_usr2);
			exit(0);
		}	
		kill(-1 * pid_arr[2], SIGUSR1);
		send_usr1++;
		printf("1 %d %d послал USR1", getpid(), getppid());
		print_time();
	}
}

void handler2(int nsig) {
	static int send_usr1 = 0;
	static int send_usr2 = 0;
	if (nsig == SIGUSR1) {
		printf("2 %d %d получил USR1", getpid(), getppid());
		print_time();
		kill(-1 *pid_arr[5], SIGUSR2);	
		send_usr2++;
		printf("2 %d %d послал USR2", getpid(), getppid());
		print_time();
	}
	else if (nsig == SIGTERM) {
		kill(-1 * pid_arr[5], SIGTERM);
		waitpid(pid_arr[5], NULL, 0);
		waitpid(pid_arr[5], NULL, 0);
		printf("2 %d %d завершил работу после %d USR1, %d USR2\n", getpid(), getppid(), send_usr1, send_usr2);
		exit(0);
	}
}

void handler3(int nsig) {
	static int send_usr1 = 0;
	static int send_usr2 = 0;
	if (nsig == SIGUSR1) {
		printf("3 %d %d получил USR1", getpid(), getppid());	
		print_time();
	}
	else if (nsig == SIGTERM) {
		printf("3 %d %d завершил работу после %d USR1, %d USR2\n", getpid(), getppid(), send_usr1, send_usr2);
		exit(0);
	}
}

void handler4(int nsig) {
	static int send_usr1 = 0;
	static int send_usr2 = 0;
	if (nsig == SIGUSR1)  {
		printf("4 %d %d получил USR1", getpid(), getppid());
		print_time();
	}
	else if (nsig == SIGTERM) {
		printf("4 %d %d завершил работу после %d USR1, %d USR2\n", getpid(), getppid(), send_usr1, send_usr2);
		exit(0);
	}
}

void handler5(int nsig) {
	static int send_usr1 = 0;
	static int send_usr2 = 0;
	if (nsig == SIGUSR2)  {
		printf("5 %d %d получил USR2", getpid(), getppid());	
		print_time();
	}
	else if (nsig == SIGTERM) {
		printf("5 %d %d завершил работу после %d USR1, %d USR2\n", getpid(), getppid(), send_usr1, send_usr2);
		exit(0);
	}
}

void handler6(int nsig) {
	static int send_usr1 = 0;
	static int send_usr2 = 0;
	if (nsig == SIGUSR2) { 
		printf("6 %d %d получил USR2", getpid(), getppid());
		print_time();
		kill(pid_arr[7], SIGUSR1);	
		send_usr1++;
		printf("6 %d %d послал USR1", getpid(), getppid());
		print_time();
	}
	else if (nsig == SIGTERM) {
		kill(pid_arr[7], SIGTERM);
		waitpid(pid_arr[7], NULL, 0);
		printf("6 %d %d завершил работу после %d USR1, %d USR2\n", getpid(), getppid(), send_usr1, send_usr2);
		exit(0);
	}
}

void handler7(int nsig) {
	static int send_usr1 = 0;
	static int send_usr2 = 0;
	if (nsig == SIGUSR1) {
		printf("7 %d %d получил USR1", getpid(), getppid());
		print_time();
		kill(pid_arr[8], SIGUSR1);
		send_usr1++;
		printf("7 %d %d послал USR1", getpid(), getppid());
		print_time();
	}
	else if (nsig == SIGTERM) {
		kill(pid_arr[8], SIGTERM);
		waitpid(pid_arr[8], NULL, 0);
		printf("7 %d %d завершил работу после %d USR1, %d USR2\n", getpid(), getppid(), send_usr1, send_usr2);
		exit(0);
	}
}

void handler8(int nsig) {
	static int send_usr1 = 0;
	static int send_usr2 = 0;
	if (nsig == SIGUSR1)  {
		printf("8 %d %d получил USR1", getpid(), getppid());
		print_time();
		kill(pid_arr[1], SIGUSR2);
		send_usr2++;
		printf("8 %d %d послал USR2", getpid(), getppid());
		print_time();
	}
	else if (nsig == SIGTERM) {
		printf("8 %d %d завершил работу после %d USR1, %d USR2\n", getpid(), getppid(), send_usr1, send_usr2);
		exit(0);
	}
}

int main() {

	pid_t pid;
	pid_arr[0] = getpid();
	if (getpid() == pid_arr[0]) {
		pid = fork();	
	    if (pid == 0)
			pid_arr[1] = getpid();
		else if (pid > 0)
			pid_arr[1] = pid;
	}
	if (getpid() == pid_arr[1]) {
		pid = fork();
		if (pid == 0)
			pid_arr[2] = getpid();
		else if (pid > 0)
			pid_arr[2] = pid;
	}
	if (getpid() == pid_arr[1]) {
		pid = fork();
		if (pid == 0)
			pid_arr[3] = getpid();
		else if (pid > 0)
			pid_arr[3] = pid;
	}
	if (getpid() == pid_arr[1]) {
		pid = fork();
		if (pid == 0)
			pid_arr[4] = getpid();
		else if (pid > 0)
			pid_arr[4] = pid;
	}
	if (getpid() == pid_arr[2]) {
		pid = fork();
		if (pid == 0)
			pid_arr[5] = getpid();
		else if (pid > 0)
			pid_arr[5] = pid;
	}
	if (getpid() == pid_arr[2]) {
		pid = fork();
		if (pid == 0)
			pid_arr[6] = getpid();
		else if (pid > 0)
			pid_arr[6] = pid;
	}
	if (getpid() == pid_arr[6])	{
		pid = fork();
		if (pid == 0)
			pid_arr[7] = getpid();
		else if (pid > 0)
			pid_arr[7] = pid;
	}
	if (getpid() == pid_arr[7])	{
		pid = fork();
		if (pid == 0)
			pid_arr[8] = getpid();
		else if (pid > 0)
			pid_arr[8] = pid;
	}
	
	if (getpid() == pid_arr[0]) {
		signal(SIGUSR1, handler0);
	}
	if (getpid() == pid_arr[1]) {
		signal(SIGUSR2, handler1);
		setpgid(pid_arr[1], 0);
		kill(pid_arr[0], SIGUSR1);
	}
	if (getpid() == pid_arr[2]) {
		signal(SIGUSR1, handler2);
		signal(SIGTERM, handler2);
		setpgid(0, pid_arr[2]);
		kill(pid_arr[0], SIGUSR1);
	}
	if (getpid() == pid_arr[3]) {
		signal(SIGUSR1, handler3);
		signal(SIGTERM, handler3);
		while (setpgid(0, pid_arr[2]) == -1)
			;
		kill(pid_arr[0], SIGUSR1);
	}
	if (getpid() == pid_arr[4]) {
		signal(SIGUSR1, handler4);
		signal(SIGTERM, handler4);
		while (setpgid(0, pid_arr[2]) == -1)
			;
		kill(pid_arr[0], SIGUSR1);
	}
	if (getpid() == pid_arr[5]) {
		signal(SIGUSR2, handler5);		
		signal(SIGTERM, handler5);
		setpgid(0, pid_arr[5]);
		kill(pid_arr[0], SIGUSR1);
	}
	if (getpid() == pid_arr[6]) {
		signal(SIGUSR2, handler6);
		signal(SIGTERM, handler6);
		while (setpgid(0, pid_arr[5]) == -1)
			;
		kill(pid_arr[0], SIGUSR1);
	}
	if (getpid() == pid_arr[7]) {
		signal(SIGUSR1, handler7);
		signal(SIGTERM, handler7);
		setpgid(pid_arr[7], 0);
		kill(pid_arr[0], SIGUSR1);
	}
	if (getpid() == pid_arr[8]) {
		signal(SIGUSR1, handler8);
		signal(SIGTERM, handler8);
		setpgid(pid_arr[8], 0);
		kill(pid_arr[0], SIGUSR1);
	}

	for (;;) { }
	
	return 0;
}
