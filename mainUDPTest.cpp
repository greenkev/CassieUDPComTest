#include "cassie_out_t.h"
#include "cassie_user_in_t.h"
#include "udp.h"
#include <iostream>

int main() {

  cassie_out_t sensors;
  cassie_user_in_t command;

  int sock_robot;
  
  unsigned char sendrobotbuf[PACKET_HEADER_LEN + CASSIE_USER_IN_T_PACKED_LEN];
  unsigned char recvrobotbuf[PACKET_HEADER_LEN + CASSIE_OUT_T_PACKED_LEN];


  sock_robot = udp_init_client("10.10.10.3", "25000","10.10.10.100", "25001");
  printf("Cassie.cpp FB_Stiff using remote 10.10.10.3 25000 remote\n");

  if (sock_robot == -1)
  {
    printf("Failed to connect to robot... returning\n");
    return false;
  }


  int choice = -1;
  while(choice < 0 || choice > 2){
    std::cout << "What would you like to view?\n"
              << " 0: etherCatStatus\n"
              << " 1: Left Leg Positions\n"
              << " 2: Left Leg Drive statusWords\n";

    std::cin >> choice;
  }


  while (true)
  {
    int inputSize = wait_for_packet(sock_robot, recvrobotbuf, sizeof recvrobotbuf, NULL, NULL);
    //std::cout << "Message Size: " << inputSize << " Message Des size: " << sizeof recvrobotbuf << std::endl;
    // Both are 699
    unpack_cassie_out_t(recvrobotbuf, &sensors);

    //Switch the different display choices
    switch(choice){
      case 0:
        std::cout << "etherCatStatus ";
        for(int i = 0; i < 6; ++i){
          std::cout << sensors.pelvis.targetPc.etherCatStatus[i] << ", ";
        }
        for(int i = 0; i < 21; ++i){
          std::cout << sensors.pelvis.targetPc.etherCatNotifications[i] << ", ";
        }
        std::cout << std::endl;
      break;

      case 1:
        std::cout << "Left Leg Position " << sensors.leftLeg.hipRollDrive.position << ", " << sensors.leftLeg.hipYawDrive.position << ", " << sensors.leftLeg.hipPitchDrive.position << ", " << sensors.leftLeg.kneeDrive.position << ", " << sensors.leftLeg.footDrive.position << std::endl;
      break;

      case 2:
        std::cout << "Left Leg statusWords " << sensors.leftLeg.hipRollDrive.statusWord << ", " << sensors.leftLeg.hipYawDrive.statusWord << ", " << sensors.leftLeg.hipPitchDrive.statusWord << ", " << sensors.leftLeg.kneeDrive.statusWord << ", " << sensors.leftLeg.footDrive.statusWord << std::endl;
      break;

      default:
      break;
    } 

    for (int i = 0; i < 10; i++)
      command.torque[i] = 0.0;

    pack_cassie_user_in_t(&command, sendrobotbuf);
    send_packet(sock_robot, sendrobotbuf, sizeof sendrobotbuf, NULL, 0);
  }
  return 0;
}
