
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

struct message {
	char source[50];
	char target[50]; 
	char msg[200]; // message body
};

void terminate(int sig) {
	printf("Exiting....\n");
	fflush(stdout);
	exit(0);
}

int main() {
	int server;
	int target;
	struct message req;
	signal(SIGPIPE,SIG_IGN);
	signal(SIGINT,terminate);
	server = open("serverFIFO",O_RDONLY);

	while (1) {
        if ( read(server,&req,sizeof(struct message)) != sizeof(struct message) ) {
            continue;
        }

        struct message resp;
        strcpy(resp.target, req.target);
        strcpy(resp.source, req.source);
        strcpy(resp.msg, req.msg);

		printf("Received a request from %s to send the message %s to %s.\n",req.source,req.msg,req.target);

        target = open(req.target, O_WRONLY);
        write(target, &req, sizeof (struct message));
        close(target);

        memcpy(resp.target, req.target, 50);
        memcpy(resp.source, req.source, 50);
        memcpy(resp.msg, req.msg, 200);

	}

	close(server);
	return 0;
}

