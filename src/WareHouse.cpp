#include "WareHouse.h"
#include "Volunteer.h"
#include "Action.h"
#include "Customer.h"
#include "Order.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

WareHouse::WareHouse(const string& configFilePath):isOpen(false), customerCounter(0), volunteerCounter(0), 
orderCounter(0), actionsLog({}), volunteers({}), pendingOrders({}), inProcessOrders({}), 
completedOrders({}), customers({}) {
    ifstream configFile(configFilePath);
    std::string line;
    while (getline(configFile, line)) 
    {
        if(line.empty())
        {
            continue;            
        }
        vector<string> words=split(line);
        if (words[0]=="customer"){ //is a customer
            if(words[2]=="soldier"){ //is a slodier customer
                SoldierCustomer* c= new SoldierCustomer(customerCounter,words[1],std::stoi(words[3]),std::stoi(words[4]));
                customers.push_back(c);
                customerCounter++;
            }
            else if(words[2]=="civilian"){ //is a civilian customer{
                CivilianCustomer* c= new CivilianCustomer(customerCounter,words[1],std::stoi(words[3]),std::stoi(words[4]));
                customers.push_back(c);
                customerCounter++;
            }

        }
        else if(words[0]=="volunteer") //is a volunteer
        {
            if(words[2]=="collector")// is a collector volunteer
            {
                CollectorVolunteer* v= new CollectorVolunteer(volunteerCounter, words[1], std::stoi(words[3]));
                volunteers.push_back(v);
                volunteerCounter++;

            }
            else if(words[2]=="limited_collector"){
                LimitedCollectorVolunteer* v= new LimitedCollectorVolunteer(volunteerCounter, words[1],std::stoi(words[3]) ,std::stoi(words[4]));
                volunteers.push_back(v);
                volunteerCounter++;

            }
            else if(words[2]=="driver"){
                DriverVolunteer* v=new DriverVolunteer(volunteerCounter, words[1], std::stoi(words[3]), std::stoi(words[4]));
                volunteers.push_back(v);    
                volunteerCounter++;

            }
            else if(words[2]=="limited_driver"){
                LimitedDriverVolunteer* v=new LimitedDriverVolunteer(volunteerCounter, words[1], std::stoi(words[3]), std::stoi(words[4]), std::stoi(words[5]));
                volunteers.push_back(v);
                volunteerCounter++;
            }
        }
    }
}

void WareHouse::start()
{
    open();
    std:: string action;
    std::cout << "Warehouse is open!" <<std::endl;
    std::cout << "Please enter action to execute" <<std::endl;
    std::getline(std::cin, action);
    std::vector<std::string> splitString= split(action);
    
    while(action!="close")
    {   
    std::string firstWord = splitString[0];
        if("step" == firstWord){
            SimulateStep* act1 = new SimulateStep(std::stoi(splitString[1]));  //stoi convert string into int
            (*act1).act(*this); //implement the action on *this
            addAction(act1); 
        }
        else if("order" == firstWord){
            AddOrder* act2 = new AddOrder(std::stoi(splitString[1])); 
            (*act2).act(*this); 
            addAction(act2); 
        } 
        else if("customer" == firstWord){
            AddCustomer* act3 = new AddCustomer(splitString[1],splitString[2],std::stoi(splitString[3]),std::stoi(splitString[4])); 
            (*act3).act(*this); 
            addAction(act3); 
        }
        else if ("orderStatus" == firstWord){
            PrintOrderStatus* act4 = new PrintOrderStatus(std::stoi(splitString[1])); 
            (*act4).act(*this);
            addAction(act4); 
        }
        else if ("customerStatus" == firstWord){
            PrintCustomerStatus* act5 = new PrintCustomerStatus(std::stoi(splitString[1])); 
            (*act5).act(*this);
            addAction(act5); 
        }
        else if ("volunteerStatus" == firstWord){
            PrintVolunteerStatus* act6 = new PrintVolunteerStatus(std::stoi(splitString[1]));
            (*act6).act(*this);
            addAction(act6); 
        }
        else if ("log" == firstWord){
            PrintActionsLog* act7 = new PrintActionsLog(); 
            (*act7).act(*this);
            addAction(act7); 
        }
        else if ("backup" == firstWord){
            BackupWareHouse* act8 = new BackupWareHouse(); 
            (*act8).act(*this);
            addAction(act8); 
        }
        else if ("restore" == firstWord){
            RestoreWareHouse* act9 = new RestoreWareHouse();
            (*act9).act(*this);
            addAction(act9); 
        }
        std::cout << "Please enter action to execute" <<std::endl;
        std::getline(std::cin, action);
        splitString= split(action);
    } //finish while loop because we need to close the warehouse
    Close* act10= new Close();
    (*act10).act(*this);
    addAction(act10);
}//finish method

vector<string> WareHouse:: split(string action) //This method we add in order to split String to words by tabs and insert them to a vector
{
    vector <string> v;
    string s="";
    for (auto i=0u; i<action.size(); i++)
    {
        if (action[i]==' ')
        {
            v.push_back(s);
            s="";
        }
        else
        {
            s+=action[i];
        }
    }
     if (!s.empty()) 
    {
        v.push_back(s);
    }
    return v;
}

const vector<Action*>& WareHouse::getActions() const
{
    return actionsLog;
}

void WareHouse::addOrder(Order* order) //TODO: check if it is only need to be pushed
{
    pendingOrders.push_back(order);
}

void WareHouse::addAction(Action* action)
{
    actionsLog.push_back(action);
}

Customer& WareHouse::getCustomer(int customerId) const
{
    for(auto i=0u; i<customers.size(); i++)
    {
        if((*customers[i]).getId()==customerId)
        {
            return *(customers[i]);
        }
    }
    return *nullCustomer;
}
       
Volunteer& WareHouse::getVolunteer(int volunteerId) const
{
    for(auto i=0u; i<volunteers.size(); i++)
    {
        if((*volunteers[i]).getId()==volunteerId)
        {
            return *(volunteers[i]);
        }
    }
    return *nullVolunteer;
}

Order& WareHouse :: getOrder(int orderId) const
{
    //Searching the order in pendingOrders vector
    for(auto i=0u; i<pendingOrders.size(); i++)
    {
        if((*(pendingOrders[i])).getId()==orderId)
        {
            return *(pendingOrders[i]);
        }
    }
    
    //Searching the order in inProcessOrders vector
    for(auto i=0u; i<inProcessOrders.size(); i++)
    {
        if((*(inProcessOrders[i])).getId()==orderId)
        {
            return *(inProcessOrders[i]);
        }
    }
    
    //Searching the order in completedOrders vector
    for(auto i=0u; i<completedOrders.size(); i++)
    {
        if((*(completedOrders[i])).getId()==orderId)
        {
            return *(completedOrders[i]);
        }
    }

    return *nullOrder; //null equivelent
}

void WareHouse::close()
{
    isOpen=false;
}

void WareHouse::open()
{
    isOpen=true;
}

// void WareHouse::deleteVul(int id) // we add this method which delete a volunteer from the vector according its id
// {
//    for (auto i=0u; i<volunteers.size(); i++)
//    {
//     if ((*(volunteers[i])).getId()==id)
//     {
//         delete volunteers[i];
//         volunteers.erase(volunteers.begin() + i);
//         break;
//     }
//    }
// }

void WareHouse::addCustomer(string customerName, string customerType, int distance, int maxOrders) //get dustomer details, create a new one with those details and add it to customers vector
{
    if(customerType=="civilian")//is a civilian
    {
        Customer* c1=new CivilianCustomer(customerCounter, customerName, distance, maxOrders);
        customers.push_back(c1);
    }
    else //is a soldier
    {
        Customer* c2=new SoldierCustomer(customerCounter, customerName, distance, maxOrders);
        customers.push_back(c2);
    }
    customerCounter++;
}

bool WareHouse::addOrderUsingCustomerId(int id){
    for (auto i = 0u; i < customers.size(); i++)
    {
        if((*customers[i]).getId() == id && (*customers[i]).canMakeOrder()){
            (*customers[i]).addOrder(orderCounter);
            Order* o1 = new Order(orderCounter, id, (*customers[i]).getCustomerDistance());
            orderCounter ++;
            addOrder(o1);
            return true;
        }
    }
    return false;
}

void WareHouse::step1(){   
    for (auto i = 0u; i < pendingOrders.size(); i++){
        if((*pendingOrders[i]).stringStatus()=="PENDING"){
            for (auto j = 0u; j < volunteers.size(); j++){ //search for volunteer
                if((*volunteers[j]).getType() == "CollectorVolunteer" || (*volunteers[j]).getType() == "LimitedCollectorVolunteer"){
                    if((*volunteers[j]).canTakeOrder(*pendingOrders[i])){
                        (*volunteers[j]).acceptOrder(*pendingOrders[i]);
                        (*pendingOrders[i]).setStatus(OrderStatus::COLLECTING);
                        (*pendingOrders[i]).setCollectorId((*volunteers[j]).getId());
                        inProcessOrders.push_back(pendingOrders[i]);
                        pendingOrders.erase(pendingOrders.begin()+i);
                        i--;
                        break;
                    }
                }
            }
        }
        else if((*pendingOrders[i]).stringStatus()=="COLLECTING"){
            for (auto j = 0u; j < volunteers.size(); j++){
                if((*volunteers[j]).getType() == "DriverVolunteer" || (*volunteers[j]).getType() == "LimitedDriverVolunteer"){
                    if((*volunteers[j]).canTakeOrder(*pendingOrders[i])){
                        (*volunteers[j]).acceptOrder(*pendingOrders[i]);
                        (*pendingOrders[i]).setStatus(OrderStatus:: DELIVERING);
                        (*pendingOrders[i]).setDriverId((*volunteers[j]).getId());
                        inProcessOrders.push_back(pendingOrders[i]);
                        pendingOrders.erase(pendingOrders.begin()+i);
                        i--;
                        break;
                    }
                }
            }
        }        
    }
}

void WareHouse::step2(){
    for (auto i = 0u; i < volunteers.size(); i++){
        if((*volunteers[i]).isBusy()){
            (*volunteers[i]).step();
        }
    }
}

void WareHouse::step3(){
    for(auto i=0u; i<volunteers.size(); i++){
        if((*volunteers[i]).isBusy() && (*volunteers[i]).isFinished()){ // the volunteer finished his current order
            int activeOrderId=(*volunteers[i]).getActiveOrderId(); //the id order that was finished by volunteer
            for (auto j=0u; j<inProcessOrders.size(); j++) // volunteer's Order must be in inProcessOrders vector
            {
                if((*inProcessOrders[j]).getId()==activeOrderId) // we found the order we need
                {
                    if ((*inProcessOrders[j]).getStatus() == OrderStatus::COLLECTING){
                        pendingOrders.push_back(inProcessOrders[j]);
                    }
                    else { //OrderStatus==DELIVERING
                        (*inProcessOrders[j]).setStatus(OrderStatus::COMPLETED);
                        completedOrders.push_back(inProcessOrders[j]);
                    }
                    deleteOrderFromInProcess((*inProcessOrders[j]).getId());//removing order from inProcessOrders vector
                    (*volunteers[i]).finishCurrentOrder(); //the current order beacome the completed order and the current order become no order
                    break; //There is no reason to keep looking for the order
                }
            }
        }
    }    
}


void WareHouse::step4(){
    for (auto i=0u; i<volunteers.size(); i++){
        string curType = (*volunteers[i]).getType();
        if((curType == "LimitedCollectorVolunteer" || curType == "LimitedDriverVolunteer") && (!(*volunteers[i]).hasOrdersLeft()) && ((*volunteers[i]).isFinished())){
            delete volunteers[i];
            volunteers.erase(volunteers.begin()+i);
            i--;
        }
    }
}


void WareHouse::deleteOrderFromInProcess(int id){
    for (auto i = 0u; i < inProcessOrders.size(); i++)
    {
        if((*inProcessOrders[i]).getId() == id){
            inProcessOrders.erase(inProcessOrders.begin() + i);
            break;
        }
    }   
}

void WareHouse::printOrders(){
    for (auto i = 0u; i < pendingOrders.size(); i++){
        std::cout << (*pendingOrders[i]).toString() << std::endl;
    }   
    for (auto i = 0u; i < inProcessOrders.size(); i++){
        std::cout << (*inProcessOrders[i]).toString() << std::endl;
    }   
    for (auto i = 0u; i < completedOrders.size(); i++){
        std::cout << (*completedOrders[i]).toString() << std::endl;
    }   
}

WareHouse :: ~WareHouse(){
    for (auto i = 0u; i < actionsLog.size(); i++){
        delete(actionsLog[i]);
    }
    actionsLog.clear();
    for (auto i = 0u; i < volunteers.size(); i++){
        delete(volunteers[i]);
    }
    volunteers.clear();
    for (auto i = 0u; i < pendingOrders.size(); i++){
        delete(pendingOrders[i]);
    }
    pendingOrders.clear();
    for (auto i = 0u; i < inProcessOrders.size(); i++){
        delete(inProcessOrders[i]);
    }
    inProcessOrders.clear();
    for (auto i = 0u; i < completedOrders.size(); i++){
        delete(completedOrders[i]);
    }
    completedOrders.clear();
    for (auto i = 0u; i < customers.size(); i++){
        delete(customers[i]);
    }   
    customers.clear();
    delete(nullCustomer);
    delete(nullOrder);
    delete(nullVolunteer);
}

WareHouse* WareHouse::clone() const{
    return new WareHouse(*this);
}

WareHouse::WareHouse(const WareHouse& other): // copy constructor
isOpen(other.isOpen), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
orderCounter(other.orderCounter),
actionsLog({}), volunteers({}), pendingOrders({}), inProcessOrders({}), completedOrders({}), customers({}) 
{
    for (auto i = 0u; i < other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog.at(i)->clone());
    }
    for (auto i = 0u; i < other.volunteers.size(); i++){
        volunteers.push_back(other.volunteers.at(i)->clone());
    }
    for (auto i = 0u; i < other.pendingOrders.size(); i++){
        pendingOrders.push_back(other.pendingOrders.at(i)->clone());
    }
    for (auto i = 0u; i < other.inProcessOrders.size(); i++){
        inProcessOrders.push_back(other.inProcessOrders.at(i)->clone());
    }
    for (auto i = 0u; i < other.completedOrders.size(); i++){
        completedOrders.push_back(other.completedOrders.at(i)->clone());
    }
    for (auto i = 0u; i < other.customers.size(); i++){
        customers.push_back(other.customers.at(i)->clone());
    }
}

WareHouse& WareHouse::operator=(const WareHouse& other){ //copy assignment
    if(this != &other){
        isOpen = other.isOpen;
        customerCounter= other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        
        for (auto i = 0u; i < actionsLog.size(); i++){
            delete(actionsLog[i]);
        }
        actionsLog.clear();
        for (auto i= 0u; i < other.actionsLog.size(); i++){
            actionsLog.push_back(other.actionsLog.at(i)->clone());
        }

        for (auto i = 0u; i < volunteers.size(); i++){
            delete(volunteers[i]);
        }
        volunteers.clear();
        for (auto i = 0u; i < other.volunteers.size(); i++){
            volunteers.push_back(other.volunteers.at(i)->clone());
        }

        for (auto i = 0u; i < pendingOrders.size(); i++){
            delete(pendingOrders[i]);
        }
        pendingOrders.clear();
        for (auto i = 0u; i < other.pendingOrders.size(); i++){
            pendingOrders.push_back(other.pendingOrders.at(i)->clone());
        }
        
        for (auto i = 0u; i < inProcessOrders.size(); i++){
            delete(inProcessOrders[i]);
        }
        inProcessOrders.clear();
        for (auto i = 0u; i < other.inProcessOrders.size(); i++){
            inProcessOrders.push_back(other.inProcessOrders.at(i)->clone());
        } 

        for (auto i = 0u; i < completedOrders.size(); i++){
            delete(completedOrders[i]);
        }
        completedOrders.clear();
        for (auto i = 0u; i < other.completedOrders.size(); i++){
            completedOrders.push_back(other.completedOrders.at(i)->clone());
        }        

        for (auto i = 0u; i < customers.size(); i++){
            delete(customers[i]);
        }
        customers.clear();
        for (auto i = 0u; i < other.customers.size(); i++){
            customers.push_back(other.customers.at(i)->clone());
        }  
    }
    return *this;
} 


WareHouse :: WareHouse(WareHouse&&other):// move constructor
    isOpen(other.isOpen), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
    orderCounter(other.orderCounter), nullOrder(other.nullOrder), nullVolunteer(other.nullVolunteer), nullCustomer(other.nullCustomer),
    actionsLog(std::move(other.actionsLog)), volunteers(std::move(other.volunteers)), pendingOrders(std::move(other.pendingOrders)),
    inProcessOrders(std::move(other.inProcessOrders)), completedOrders(std::move(other.completedOrders)), customers(std::move(other.customers))
{ 
    other.nullOrder = nullptr;
    other.nullCustomer = nullptr;
    other.nullVolunteer = nullptr;  
}

WareHouse& WareHouse::operator=(WareHouse&& other){ //move assignment
    if(this != &other){
        delete(this);
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        nullOrder = other.nullOrder;
        other.nullOrder = nullptr;
        nullVolunteer = other.nullVolunteer;
        other.nullVolunteer = nullptr;
        nullCustomer = other.nullCustomer;
        other.nullCustomer = nullptr;
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
    }
    return (*this);
}
