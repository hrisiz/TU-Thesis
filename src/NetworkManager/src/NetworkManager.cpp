#include "NetworkManager.hpp"
#include "Logger.hpp"
#include "EthernetController.hpp"

#include <experimental/filesystem>
#include <chrono>

NetworkManager::NetworkManager()
:dnsmasq_controller(std::make_shared<DNSMasqController>()),
dhcp_server(std::make_shared<DHCPServer>(dnsmasq_controller))
{

}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::start()
{
    /*this->checker_running = true;
    this->checker_thread = std::thread(&NetworkManager::checkerThread, this);
    std::unique_lock<std::mutex> lk(checker_thread_start_mutex);
    checker_thread_start_cond.wait(lk);*/
    this->checkerThread();
}

void NetworkManager::stop()
{
    /*this->checker_running = false;
    this->checker_thread_timer_cond.notify_one();
    this->checker_thread.join();*/
}

void NetworkManager::checkerThread()
{
    //bool notify_start = false;
    //while(this->checker_running) // for now use only on start
    //{
        //bool change = false;
        std::vector<std::string> interfaces_list = this->getSystemInterfacesList();
        /*std::map<std::string, std::shared_ptr<InterfaceController>> current_nm_interfaces = this->interfaces;
        
        for(auto interface : current_nm_interfaces)
        {
            auto found = std::find(interfaces_list.begin(), interfaces_list.end(), interface.first);
            if(interfaces_list.end() == found)
            {
                std::lock_guard<std::mutex> interfaces_mutex_lock(this->interfaces_mutex);
                this->interfaces.erase(interface.first);
                //change = true;
            }
        }*/

        for(auto interface_name : interfaces_list)
        {
            try
            {
                this->addInterface(interface_name);
                //change = true;
            }
            catch(std::runtime_error& e)
            {
                WarningLogger << "Failed to add " << interface_name << ": " << e.what() << std::endl;
            }
        }

        /*if(change)
        {
            //ToDo: Notify configurators for the change.
        }

        if(!notify_start)
        {
            this->checker_thread_start_cond.notify_one();
            notify_start = true;
        }
        if(this->checker_running)
        {
            std::unique_lock<std::mutex> lk(this->checker_thread_timer_mutex);
            this->checker_thread_timer_cond.wait_for(lk,std::chrono::seconds(5));
        }*/
    //}
}

std::vector<std::string> NetworkManager::getSystemInterfacesList()
{
    std::vector<std::string> result;
    std::string interfaces_path = "/sys/class/net";
    for (const auto & interface : std::experimental::filesystem::directory_iterator(interfaces_path))
    {
        std::string interface_name = interface.path().filename();
        result.push_back(interface_name);
    }
    return result;
}

void NetworkManager::addInterface(std::string interface_name) // add callback for in case of removal
{
    std::string interface_type = interface_name.substr(0,2);
    if(interface_type == "en" || interface_type == "et" || interface_type == "us")
    {
        std::lock_guard<std::mutex> interfaces_mutex_lock(this->interfaces_mutex);
        this->interfaces[interface_name] = std::make_shared<EthernetController>(interface_name, dhcp_server);
    }
    else
    {
        throw std::runtime_error("Unknown interface type");
    }
}

std::shared_ptr<Route> NetworkManager::addRoute(std::string destination, std::string gateway, std::string interface_name, int metric)
{
    std::lock_guard<std::mutex> routes_mutex_lock(this->routes_mutex);
    std::shared_ptr<Route> result;
    std::remove_if(this->routes.begin(),this->routes.end(), [](auto elem){
        return elem.expired();
    });

    auto found = std::find_if(this->routes.begin(),this->routes.end(),[this,destination,metric](auto elem){
        auto element = elem.lock();
        return element->getDestination() == destination && element->getMetric() == metric;
    });

    if(this->routes.end() != found)
    {
        result = found->lock();
    }
    else
    {
        std::shared_ptr<Route> route = std::make_shared<Route>(destination, gateway, interface_name, metric);
        this->routes.push_back(route);
        result = route;
    }
    return result;
}

std::shared_ptr<InterfaceController> NetworkManager::getInterface(std::string interfaces_name)
{
    std::lock_guard<std::mutex> interfaces_mutex_lock(this->interfaces_mutex);
    return this->interfaces.at(interfaces_name);
}

std::shared_ptr<XTables::Chain> NetworkManager::getXTablesChain(std::string name)
{
    std::lock_guard<std::mutex> chain_rules_mutex_lock(this->chain_rules_mutex);
    std::shared_ptr<XTables::Chain> result;
    std::remove_if(this->chain_rules.begin(),this->chain_rules.end(), [](auto elem){
        return elem.expired();
    });

    auto found = std::find_if(this->chain_rules.begin(),this->chain_rules.end(),[name](auto elem){
        auto element = elem.lock();
        return element->getName() == name;
    });

    if(this->chain_rules.end() != found)
    {
        result = found->lock();
    }
    else
    {
        std::shared_ptr<XTables::Chain> chain_rule = std::make_shared<XTables::Chain>(name);
        this->chain_rules.push_back(chain_rule);
        result = chain_rule;
    }
    return result;
}