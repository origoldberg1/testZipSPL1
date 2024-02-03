#include "WareHouse.h"
#include "Volunteer.h"
#include "Action.h"
#include "Customer.h"
#include "Order.h"
#include <iostream>

using namespace std;

WareHouse* backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        std::cout << "usage: warehouse <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    WareHouse wareHouse(configurationFile);
    wareHouse.start();
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    return 0;
}


// using namespace std;

// WareHouse* backup = nullptr;

// int main(int argc, char** argv)
// {
//     if(argc!=2){
//         std::cout << "usage: warehouse <config_path>" << std::endl;
//         return 0;
//     }
//      string configurationFile = argv[1];
//      WareHouse *wareHouse = new WareHouse();
//      (*wareHouse).init(configurationFile);
//      (*wareHouse).start();
//      if(backup!=nullptr){
//    	     delete backup;
//    	 backup = nullptr;
//      }
//      delete(wareHouse);
//     return 0;
// }