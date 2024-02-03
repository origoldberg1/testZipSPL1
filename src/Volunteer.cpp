//#pragma once
#include "WareHouse.h"
#include "Volunteer.h"
#include "Action.h"
#include "Customer.h"
#include "Order.h"
#include <iostream>

Volunteer::Volunteer(int id, const string &name) :
    completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

int Volunteer:: getId() const{
    return id;
}

const std::string& Volunteer:: getName() const {
    return name;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer:: isBusy() const{ //TODO: if a volunteer finishes an order and does not take on a new one, assign NO_ORDER to activeOrderId
    return !(activeOrderId == NO_ORDER);
}

bool Volunteer :: hasOrdersLeft() const{
    return true;
}

bool Volunteer ::canTakeOrder(const Order &order) const{
    return !isBusy();
}

void Volunteer :: acceptOrder(const Order &order){
    activeOrderId = order.getId();
}

string Volunteer:: toString() const{
    std::string res=  std::string("VolunteerID: ") + std::to_string(getId()) + '\n' + std::string("isBusy: ");
    if(isBusy() == 1) {
        res += std::string("true\nOrderID: ") + std::to_string(activeOrderId) + std::string("\ntimeLeft: ");
    }
    else{
        res += "false\nOrderID: None\ntimeLeft: ";
    }
    return res;
}

void Volunteer::finishCurrentOrder(){
    completedOrderId = activeOrderId;
    activeOrderId = NO_ORDER;
}




CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown)
    : Volunteer(id, name), coolDown(coolDown), timeLeft(coolDown) {}

CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this); 
}

void CollectorVolunteer :: step(){
    timeLeft --;
}

int CollectorVolunteer :: getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer :: getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer:: decreaseCoolDown(){//Decrease timeLeft by 1,return true if timeLeft=0,false otherwise 
    timeLeft --;
    return timeLeft == 0;
}

bool CollectorVolunteer :: hasOrdersLeft() const{
    return Volunteer :: hasOrdersLeft();
}

bool CollectorVolunteer :: canTakeOrder(const Order &order) const{
    return Volunteer :: canTakeOrder(order);
}

void CollectorVolunteer::acceptOrder(const Order &order){
    Volunteer::acceptOrder(order);
    timeLeft = coolDown;
}

string CollectorVolunteer::baseToString() const{
    std::string res =  Volunteer::toString();
    if(isBusy() == 1) {
        res += std::to_string(timeLeft);
    }
    else{
        res += "None";
    }
    return res;
}

string CollectorVolunteer::toString() const{
    std::string res = baseToString();
    res += "\nordersLeft: No Limit";
    return res;
}

string CollectorVolunteer::getType() const{
    return "CollectorVolunteer";
}

bool CollectorVolunteer:: isFinished(){
    return timeLeft == 0;
}


LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders):
    CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this); 
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const{
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    return Volunteer::canTakeOrder(order) && hasOrdersLeft();
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    CollectorVolunteer::acceptOrder(order);
    ordersLeft --;
}

int LimitedCollectorVolunteer:: getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer:: getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const{
    std::string res = CollectorVolunteer::baseToString();
    res += "\nordersLeft: " +std::to_string(ordersLeft);
    return res;
}

string LimitedCollectorVolunteer::getType() const{
    return "LimitedCollectorVolunteer";
}

bool LimitedCollectorVolunteer:: isFinished(){
    return CollectorVolunteer:: isFinished();
}




DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep):
    Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0){}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this); 
}

int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft() {
    distanceLeft -= distancePerStep;
    if(distanceLeft <= 0){
        distanceLeft = 0;
        return true;
    }
    return false;
}

bool DriverVolunteer:: hasOrdersLeft() const{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const{
    return Volunteer::canTakeOrder(order) && order.getDistance() <= maxDistance;
}

void DriverVolunteer::acceptOrder(const Order &order) {
    Volunteer::acceptOrder(order);
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step(){
    decreaseDistanceLeft();
}

string DriverVolunteer::baseToString() const{
    std::string res =  Volunteer::toString();
    if(isBusy() == 1) {
        res += std::to_string(distanceLeft);
    }
    else{
        res += "None";
    }  
    return res;
}

string DriverVolunteer::toString() const{
    std::string res = DriverVolunteer::baseToString();
    res += std::string("\nordersLeft: No Limit");
    return res;
}

string DriverVolunteer::getType() const{
    return "DriverVolunteer";
}

bool DriverVolunteer:: isFinished(){
    return distanceLeft == 0;
}


LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
    DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this); 
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const{
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{
    return DriverVolunteer::canTakeOrder(order) && hasOrdersLeft();
}

void LimitedDriverVolunteer::acceptOrder(const Order &order){
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}

string LimitedDriverVolunteer::toString() const{
    std::string res = DriverVolunteer::baseToString();
    res += "\nordersLeft: " + std::to_string(ordersLeft);
    return res;   
}

string LimitedDriverVolunteer::getType() const{
    return "LimitedDriverVolunteer";
}

bool LimitedDriverVolunteer :: isFinished(){
    return DriverVolunteer :: isFinished();
}
