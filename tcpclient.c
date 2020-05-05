#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define PORT 5000
#define MAXLINE 1024
int main()
{
    int sockfd, sockfd2;
    char input[MAXLINE];
    char input2[MAXLINE];
    char output[MAXLINE];
    struct sockaddr_in servaddr;

    int n, len;
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket creation failed");
        exit(0);
    }
    if ((sockfd2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket2 creation failed");
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
/*std::cout << htons(server_storage.sin_port) << std::endl;
            inet_ntop(AF_INET, &(server_storage.sin_addr), ip, INET_ADDRSTRLEN);
            std::cout << ip << std::endl;*/
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&servaddr,
                sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
    }
    len = sizeof(servaddr);
    memset(input, 0, sizeof(input));
    strcpy(input, "9");
    memset(input2, 0, sizeof(input2));
    strcpy(input2, "12");
    memset(output, 0, sizeof(output));
    strcpy(output, "");
    int i = 0;
    n = recv(sockfd, output, MAXLINE, 0);
    puts(output);
    n = sendto(sockfd2, output, strlen(output), 0,
                   (struct sockaddr *)&servaddr, len);
    for (i = 0; i <= 50; ++i)
    {
        puts(input);
        write(sockfd, input, strlen(input));
        n = sendto(sockfd2, input2, strlen(input2), 0,
                   (struct sockaddr *)&servaddr, len);
        n = recvfrom(sockfd2, output, MAXLINE,
                    0, (struct sockaddr *)&servaddr,
                    &len);
        puts(output);
    }
    for (;;)
        ;
    close(sockfd);
}