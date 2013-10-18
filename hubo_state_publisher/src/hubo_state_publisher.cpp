//ros-based header files
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>

//Network based header files. 
//Please do not change this order of loading the header  files.
#include <stdio.h>      
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<iostream>
#include<string>
#include <unistd.h>
#include<iomanip>
#include <sys/ioctl.h>
#include <sstream>

//THE DEFAULT PORT TO RECEIVE THE ENCODER VALUES
#define SERVER_PORT     8088

using namespace std;

//declaring the global variables
stringstream ss;
int sd,rc,i;
float joint_values[27];
float a=0.0;
float b=0.0;
char buffer[350];
string encoder,encoder_1;
int s;

//define a non-blocking socket,hence not waiting to recieve data.
unsigned long nonblocking=1;

//declare objects for publishing.
std::string enco_read;
ros::Publisher joint_pub;
sensor_msgs::JointState joint_state;




void hubo_callback()
{

joint_state.header.stamp = ros::Time::now();
a=0;

//Assign the encoder variable of the actual robot to the virtual model's joint value.
joint_state.name[0] ="LSR";
joint_state.position[0]=joint_values[8];

joint_state.name[1] ="LSY";
joint_state.position[1]=joint_values[9];

joint_state.name[2] ="LEP";
joint_state.position[2]=joint_values[10];

joint_state.name[3] ="LWY";
joint_state.position[3]=joint_values[11];

joint_state.name[4] ="LWP";
joint_state.position[4]=joint_values[12];


joint_state.name[5] ="LAP";
joint_state.position[5]=joint_values[22];


joint_state.name[6] ="LAR";
joint_state.position[6]=joint_values[23];


joint_state.name[7] ="LHP";
joint_state.position[7]=joint_values[14];

joint_state.name[8] ="LHR";
joint_state.position[8]=joint_values[15];

joint_state.name[9] ="LHY";
joint_state.position[9]=joint_values[18];

joint_state.name[10] ="LKP";
joint_state.position[10]=joint_values[21];

joint_state.name[11] ="LWR";
joint_state.position[11]=a;

joint_state.name[12] ="NKY";
joint_state.position[12]=a;

joint_state.name[13] ="RAP";
joint_state.position[13]=joint_values[24];


joint_state.name[14] ="RAR";
joint_state.position[14]=joint_values[25];

joint_state.name[15] ="REP";
joint_state.position[15]=joint_values[3];

joint_state.name[16] ="RHP";
joint_state.position[16]=joint_values[16];

joint_state.name[17] ="RHR";
joint_state.position[17]=joint_values[17];

joint_state.name[18] ="RHY";
joint_state.position[18]=joint_values[19];

joint_state.name[19] ="RKP";
joint_state.position[19]=joint_values[20];

joint_state.name[20] ="RSP";
joint_state.position[20]=joint_values[0];

joint_state.name[21] ="RSR";
joint_state.position[21]=joint_values[1];

joint_state.name[22] ="RSY";
joint_state.position[22]=joint_values[2];

joint_state.name[23] ="RWP";
joint_state.position[23]=joint_values[5];

joint_state.name[24] ="RWR";
joint_state.position[24]=a;

joint_state.name[25] ="RWY";
joint_state.position[25]=joint_values[4];


joint_state.name[26] ="LF32";
joint_state.position[26]=a;

joint_state.name[27] ="NK1";
joint_state.position[27]=a;

joint_state.name[28] ="NK2";
joint_state.position[28]=a;

joint_state.name[29] ="RF13";
joint_state.position[29]=a;

joint_state.name[30] ="RF12";
joint_state.position[30]=a;

joint_state.name[31] ="TSY";
joint_state.position[31]=a;

joint_state.name[32] ="RF31";
joint_state.position[32]=a;


joint_state.name[33] ="RF32";
joint_state.position[33]=a;

joint_state.name[34] ="RF2";
joint_state.position[34]=a;

joint_state.name[35] ="RF1";
joint_state.position[35]=a;

joint_state.name[36] ="RF22";
joint_state.position[36]=a;

joint_state.name[37] ="RF23";
joint_state.position[37]=a;

joint_state.name[38] ="RF21";
joint_state.position[38]=a;


joint_state.name[39] ="RF42";
joint_state.position[39]=a;

joint_state.name[40] ="RF43";
joint_state.position[40]=a;


joint_state.name[41] ="RF2";
joint_state.position[41]=a;

joint_state.name[42] ="RF1";
joint_state.position[42]=a;

joint_state.name[43] ="LF21";
joint_state.position[43]=a;

joint_state.name[44] ="LF22";
joint_state.position[44]=a;

joint_state.name[45] ="LF23";
joint_state.position[45]=a;

joint_state.name[46] ="LF1";
joint_state.position[46]=a;

joint_state.name[47] ="LF31";
joint_state.position[47]=a;

joint_state.name[48] ="LF12";
joint_state.position[48]=a;


joint_state.name[49] ="LF33";
joint_state.position[49]=a;

joint_state.name[50] ="LF11";
joint_state.position[50]=a;


joint_state.name[51] ="LF13";
joint_state.position[51]=a;

joint_state.name[52] ="RF33";
joint_state.position[52]=a;

joint_state.name[53] ="LSP";
joint_state.position[53]=joint_values[7];



joint_pub.publish(joint_state);



}



int main(int argc, char** argv) {

//define your ros node,publisher,subscriber etc.
ros::init(argc, argv, "hubo_state_publisher");
ros::NodeHandle n;
joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
tf::TransformBroadcaster broadcaster;
ros::Rate loop_rate(100);

//define your socket related objects
struct sockaddr_in serveraddr;
serveraddr.sin_family      = AF_INET;
serveraddr.sin_port        = htons(SERVER_PORT);
serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); //make sure to provide this rather than a particular ip-address(which never works).
int n_0=0,n_1=0;
int size=sizeof(serveraddr);   

//define your socket
sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
ioctl(sd,FIONBIO,&nonblocking);
rc = bind(sd, (struct sockaddr *)&serveraddr, size);
socklen_t serveraddrlen =sizeof(serveraddr);


// message declarations
geometry_msgs::TransformStamped odom_trans;
    

   
joint_state.name.resize(54);
joint_state.position.resize(54);
string sub;
int init=1;
int diff;
    while (ros::ok()) {


rc = recvfrom(sd, buffer, sizeof(buffer), 0,
                    (struct sockaddr *)&serveraddr, 
                    &serveraddrlen);
encoder=(string)buffer;
init=1;
n_0=0;
n_1=0;
if(encoder.length()!=0)
{
//Parse the recieved encoder data.
for(i=0;i<27;i++)
{
n_1=encoder.find('*',n_0);
diff=n_1-init;
encoder_1=encoder.substr(init,diff);
init=n_1+1;
joint_values[i]=std::atof(encoder_1.c_str());
n_0=n_1+1;
encoder_1=" ";

//update the position
odom_trans.header.frame_id = "odom";
odom_trans.child_frame_id = "Body_Torso";
odom_trans.transform.translation.x=1;
odom_trans.transform.translation.y=1;
odom_trans.transform.translation.z=1;
odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(0);
broadcaster.sendTransform(odom_trans);
std::cout<<"\n encoder:"<<encoder;
hubo_callback();  
}
encoder=" ";
}
    
ros::spinOnce();
loop_rate.sleep();
}


    return 0;
}
