#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"

class Action;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
       // void init(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(Action* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order& getOrder(int orderId) const;
        const vector<Action*> &getActions() const;       
        void close();
        void open();
        std::vector<std::string> split(std::string str); //we add this method which split string into vector by spaces
        void addCustomer(string customerName, string customerType, int distance, int maxOrders); //we add this method which get a new customer and add ot to customers vector
        bool addOrderUsingCustomerId(int id);
        void deleteOrderFromInProcess(int id);
        void step1();
        void step2();
        void step3();
        void step4();
        void printOrders();
        WareHouse* clone() const;

        //rule of 5:
        WareHouse(const WareHouse& other); // copy constructor
        WareHouse& operator=(const WareHouse& other); // copy assignment
        ~WareHouse(); //destructor
        WareHouse(WareHouse&&other); // move constructor
        WareHouse& operator=(WareHouse&& other); // move assignment



    private:
        bool isOpen;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //Nir Sorani told it will be useful
        Order* nullOrder = new Order(-1, -1 ,-1);
        Volunteer* nullVolunteer = new CollectorVolunteer(-1, "null", -1);
        Customer* nullCustomer = new SoldierCustomer(-1, "null",-1 ,-1);
        vector<Action*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
};