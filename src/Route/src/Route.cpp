#include "Route.hpp"
#include "Logger.hpp"
#include "System.hpp"

Route::Route(IP destination, IP gateway, std::string interface_name, int metric, std::string table_name)
:destination(destination), gateway(gateway), metric(metric), interface_name(interface_name), table_name(table_name)
{
    this->routeConfiguration("add");
}

Route::~Route()
{
    try
    {
        this->routeConfiguration("del");
    }
    catch(std::exception& e)
    {
        WarningLogger << e.what() << std::endl;
    }
}

void Route::routeConfiguration(std::string option)
{
    std::string interface_string = "";
    if(!this->interface_name.empty())
    {
        interface_string = " dev " + this->interface_name; 
    }
    std::string table_string = "";
    if(!this->table_name.empty())
    {
        table_string = " table " + this->table_name;
    }
    std::string metric_string = " metric " + std::to_string(this->metric);
    std::string cmd = "ip route " + option + " " + this->destination + " via " + this->gateway + interface_string + metric_string + table_string;
    if( 0 != System::call(cmd))
    {
        throw std::runtime_error("Failed to execute: " + cmd);
    }
}

IP Route::getDestination()
{
    return this->destination;
}

IP Route::getGateway()
{
    return this->gateway;
}

int Route::getMetric()
{
    return this->metric;
}

std::string Route::getInterfaceName()
{
    return this->interface_name;
}

std::string Route::getTable()
{
    return this->table_name;
}

bool Route::operator==(const Route& route)
{
    return ((this->destination == route.destination)&&
        (this->metric == route.metric));
}