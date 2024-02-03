//#pragma once
#include "WareHouse.h"
#include "Volunteer.h"
#include "Action.h"
#include "Customer.h"
#include "Order.h"
#include <iostream>
Customer::Customer(int id, const string& name, int locationDistance, int maxOrders):id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId() {}

const string& Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}

int Customer::getCustomerDistance() const
{
    return locationDistance;
}

int Customer::getMaxOrders() const //Returns maxOrders
{
    return maxOrders;
}

int Customer::getNumOrders() const //Returns num of orders the customer has made so far
{
    return ordersId.size();
}

bool Customer::canMakeOrder() const //Returns true if the customer didn't reach max orders
{
    return getNumOrders()<maxOrders;
}

const vector<int>& Customer::getOrdersIds() const
{
   return ordersId; 
}

int Customer::addOrder(int orderId) //return OrderId if order was added successfully, -1 otherwise
{
    if (!canMakeOrder())
    {
     return -1;
    }
    ordersId.push_back(orderId);
    return orderId;
}

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders):
Customer(id,name,locationDistance,maxOrders){}

SoldierCustomer* SoldierCustomer::clone() const 
{
    return new SoldierCustomer(*this);
}

bool SoldierCustomer:: isSoldier() const{
    return true;
}

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders):
Customer(id,name,locationDistance,maxOrders){}

CivilianCustomer* CivilianCustomer::clone() const 
{
    return new CivilianCustomer(*this);
}

bool CivilianCustomer:: isSoldier() const{
    return false;
}