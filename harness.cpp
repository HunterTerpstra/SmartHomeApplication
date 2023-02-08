//
// Created by Ryan Chan on 2022-11-03.
//

#include <thread>
#include "Connections/Wifi.h"
#include "Central.h"

int main (int argc, char ** argv) {
    std::cerr << "Harness started." << std::endl;
    logger::instance();
    logger::instance().write("Harness","Logger initialization success");

    Wifi wifi_obj;
    std::thread t1 (&Wifi::wifi_run);

    std::cerr << "Wifi constructor completed." << std::endl;

    Central* test = new Central();
    std::cerr << "Central system unexpectedly exit." << std::endl;

    while(1);

}