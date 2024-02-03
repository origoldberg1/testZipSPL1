//#pragma once
#include "Order.h"
#include "WareHouse.h"
#include "Volunteer.h"
#include "Action.h"
#include "Customer.h"
#include <iostream>

Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), 
distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), 
driverId(NO_VOLUNTEER) {}

int Order:: getId() const
{
    return id;
}

int Order:: getCustomerId() const
{
    return customerId; 
}

void Order:: setStatus(OrderStatus status)
{
    this->status=status;
}

void Order:: setCollectorId(int collectorId)
{
    this->collectorId=collectorId;
}

void Order:: setDriverId(int driverId)
{
    this->driverId=driverId;
}

int Order:: getCollectorId() const
{
    return collectorId;
}

int Order:: getDriverId() const
{
    return driverId;
}

OrderStatus Order:: getStatus() const
{
    return status;
}

const int Order::getDistance() const //we add this getter
{
    return distance;
}

const string Order:: toString() const
{
    std::string strStatus=stringStatus();
    return "OrderID:"+ std::to_string(id)+" , CustomerID:"+ std::to_string(customerId)+" , OrderStatus:"+strStatus;
}

string Order::stringStatus() const
{
    string strStatus="";
    switch(status)
    {
        case OrderStatus::PENDING:
            strStatus="PENDING";
            break;
        case OrderStatus::COLLECTING:
            strStatus="COLLECTING";
            break;
        case OrderStatus::DELIVERING:
            strStatus="DELIVERING";
            break;
        case OrderStatus::COMPLETED:
            strStatus="COMPLETED";
            break;
    }
    return strStatus;
}

Order* Order :: clone() const{
    return new Order(*this);
}