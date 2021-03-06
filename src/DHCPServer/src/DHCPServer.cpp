#include "DHCPServer.hpp"

DHCPServer::DHCPServer(std::shared_ptr<DNSMasqController> dnsmasq_controller)
:dnsmasq_controller(dnsmasq_controller)
{

}

std::shared_ptr<DHCPServerConfiguration> DHCPServer::createConfiguration(std::string interface_name, std::string ip_range_start, std::string ip_range_end, uint lease)
{
    std::shared_ptr<DHCPServerConfiguration> result = std::make_shared<DNSMasqDHCPServerConfiguration>(this->dnsmasq_controller, interface_name, ip_range_start, ip_range_end, lease);
    return result;
}
