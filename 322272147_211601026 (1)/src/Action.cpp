//#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "WareHouse.h"
#include "Volunteer.h"
#include "Action.h"
#include "Customer.h"
#include "Order.h"
using std::string;
using std::vector;

extern WareHouse *backup;

//implmenting Action class
Action::Action(): errorMsg(), status() {}  

void Action:: complete()
{
    status=ActionStatus::COMPLETED;
}

void Action::error(string errorMsg)
{
    status=ActionStatus::ERROR;
    this->errorMsg=errorMsg;
}

string Action::getErrorMsg() const
{
    return errorMsg;
}

ActionStatus Action::getStatus() const
{
    return status;
}

//implementing SimulateStep class
SimulateStep::SimulateStep(int numOfSteps):Action(),numOfSteps(numOfSteps){}

void SimulateStep::act(WareHouse& warehouse)
{
    for (int i=1; i<=numOfSteps; i++){
        warehouse.step1();
        warehouse.step2();
        warehouse.step3();
        warehouse.step4();
    }
    complete();
}

string SimulateStep::toString() const
{
    return "simulateStep "+std::to_string(numOfSteps) + " COMPLETED";
}

SimulateStep* SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

//implementing AddOrder class
AddOrder::AddOrder(int id):
    Action(),customerId(id){}

void AddOrder::act(WareHouse &wareHouse) 
{
    if (wareHouse.addOrderUsingCustomerId(customerId))
    {
        Action::complete();
    }
    else
    {
        error("Cannot place this order");
        std::cout<<"Error: "+ getErrorMsg() <<std::endl;
    }
}

string AddOrder::toString() const
{
    string str= "order "+std::to_string(customerId);
    if(getStatus()==ActionStatus::COMPLETED)
    {
        str += " COMPLETED";
    }
    else //status is Error
    {
        str += " ERROR";
    }
    return str;
}

AddOrder* AddOrder::clone() const
{
    return new AddOrder(*this);
}

//implementing AddCustomer class
AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders):
    Action(),customerName(customerName), customerType(stringToEnumCT(customerType)),distance(distance), maxOrders(maxOrders){
        
    }

void AddCustomer::act(WareHouse& warehouse)
{
    string customerTypeStr = enumToStringCT();
    warehouse.addCustomer(customerName, customerTypeStr, distance, maxOrders);
    Action::complete(); //This Action never results in an error according instructions
}

AddCustomer* AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const
{
    string str = "customer "+customerName+" "+ enumToStringCT() + " " + std::to_string(distance) + " " + std::to_string(maxOrders);
       if(getStatus()==ActionStatus::COMPLETED)
    {
        str += "COMPLETED";
    }
    else //status is Error
    {
        str += " ERROR: ";
    }
    return str;
}

CustomerType AddCustomer::stringToEnumCT(string customerType) const{
    if(customerType == "Soldier"){
        return CustomerType::Soldier;
    }
    return CustomerType::Civilian;
}

string AddCustomer :: enumToStringCT() const{
    if(customerType == CustomerType::Soldier){
        return "Soldier";
    }
    return "Civilian";
}

//implementing PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id):Action(),orderId(id){}

void PrintOrderStatus::act(WareHouse& wareHouse)
{
    Order o1=wareHouse.getOrder(orderId);
    if(o1.getId() >= 0){
        std::cout<<"OrderId: "+std::to_string(orderId)<<std::endl;    
        std::cout<<"OrderStatus: "+o1.stringStatus()<<std::endl;
        string customerId=std::to_string(o1.getCustomerId());
        std::cout<<"CustomerId: "+customerId<<std::endl;
        int collectorId=o1.getCollectorId();
        if(collectorId == -1){
            std::cout<<"Collector: None"<<std::endl;
        }
        else{
            std::cout<<"Collector: "+std::to_string(collectorId)<<std::endl;
        }

        int driverId=o1.getDriverId();
        if(driverId == -1){
            std::cout<<"Driver: None"<<std::endl;
        }
        else{
            std::cout<<"Driver: "+std::to_string(driverId)<<std::endl;
        }
        Action::complete();
    }
    else{
        error("Order doesn't exist");
        std::cout<< "ERROR: " << getErrorMsg() << std::endl;
    }
}

PrintOrderStatus* PrintOrderStatus :: clone() const{
    return new PrintOrderStatus(*this);

}

string PrintOrderStatus::toString() const{
    string str= "orderStatus "+std::to_string(orderId);
    if(getStatus()==ActionStatus::COMPLETED){
        str += " COMPLETED";
    }
    else{ //status is Error
        str += " ERROR";
    }
    return str;
}

//implementing PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus(int customerId):customerId(customerId){}

void PrintCustomerStatus::act(WareHouse& warehouse){
    if(warehouse.getCustomer(customerId).getId() >= 0){
        std::cout<<"CustomerID: "+std::to_string(customerId)<<std::endl;
        // If we reach this point, the customer was found
        for(auto i=0u; i<warehouse.getCustomer(customerId).getOrdersIds().size(); i++) //passing each customer's orders 
        {     
                int orderId=warehouse.getCustomer(customerId).getOrdersIds()[i]; 
                Order o1=warehouse.getOrder(orderId); //find the order itself in warehouse
                std::cout<<"OrderID: "+ std::to_string(orderId) <<std::endl; 
                std::cout<<"OrderStatus: "+o1.stringStatus()<<std::endl;
        }
        std::cout<<"numOrdersLeft: "+std::to_string(warehouse.getCustomer(customerId).getMaxOrders()-warehouse.getCustomer(customerId).getNumOrders())<<std::endl;
        complete();
    } 
    else{ //no such customer was found in warehouse
        error("Customer doesn't exist");
        std::cout<< "Error: " << getErrorMsg()<<std::endl;
    }
}

PrintCustomerStatus* PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    string str= "customerStatus "+std::to_string(customerId);
    if(getStatus()==ActionStatus::COMPLETED)
    {
        str += " COMPLETED";
    }
    else //status is Error
    {
        str+= " ERROR";
    }
    return str;
}

//implementing PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int id):
    volunteerId(id){}

void PrintVolunteerStatus::act(WareHouse& wareHouse) {
    if ((wareHouse.getVolunteer(volunteerId)).getId()==-1){
        error("Volunteer doesn't exist");
        std::cout<<"Error: Volunteer doesn't exist"<<std::endl;
    }
    else{
        std::cout<<(wareHouse.getVolunteer(volunteerId)).toString()<<std::endl;
        complete();
    }
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
    string str= "volunteerStatus "+std::to_string(volunteerId);
    if(getStatus()==ActionStatus::COMPLETED){
        str += " COMPLETED";
    }
    else{ //status is Error
        str += " ERROR";
    }
    return str;
}

// implementing printActionsLog
PrintActionsLog ::PrintActionsLog(){}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    vector <Action*> logVector=wareHouse.getActions();
    for (auto i=0u; i<logVector.size(); i++)
    {
        std::cout<<(*(logVector[i])).toString()<<std::endl;
    }
    complete(); //this method never results in error
}

PrintActionsLog* PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const
{
    return "log COMPLETED";
}

// implementing Close
Close::Close(){}

void Close::act(WareHouse &wareHouse)
{
    wareHouse.close();
    wareHouse.printOrders();
    complete(); //This action never resutls in error
}

Close* Close::clone() const
{
    return new Close(*this);
}

string Close::toString() const
{
    return "close COMPLETED";
}

BackupWareHouse::BackupWareHouse(){}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    if(backup!=nullptr){
        delete backup;
    }
    backup=wareHouse.clone();
    complete();
}

BackupWareHouse* BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const
{
    return "backup COMPLETED";
}

RestoreWareHouse::RestoreWareHouse(){}

void RestoreWareHouse::act(WareHouse& warehouse){
    if(backup==nullptr){
        error("No backup available");
        std::cout<<"Error: No backup available" <<endl;
    }
    else{
        warehouse=*backup; //copy assignment
        complete(); 
    }
}

RestoreWareHouse* RestoreWareHouse::clone() const{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const{
    std::string str= "restore";
    if(getStatus()==ActionStatus::COMPLETED){
        str += " COMPLETED";
    }
    else{
        str+= " ERROR";
    }
    return str;
}
