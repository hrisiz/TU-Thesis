#ifndef DNSMASQ_DHCP_SERVER_CONFIGURATION_HPP
#define DNSMASQ_DHCP_SERVER_CONFIGURATION_HPP

#include "DHCPServerConfiguration.hpp"
#include "DNSMasqController.hpp"

#include <string>
#include <memory>
#include <mutex>

class DNSMasqDHCPServerConfiguration : public DHCPServerConfiguration
{
private:
    std::shared_ptr<DNSMasqController> dnsmasq_controller;
    std::shared_ptr<DNSMasqConfiguration> conf;
public:
    DNSMasqDHCPServerConfiguration(std::shared_ptr<DNSMasqController> dnsmasq_controller, std::string interface_name, std::string ip_range_start, std::string ip_range_end, uint lease);
    ~DNSMasqDHCPServerConfiguration() = default;
};
#endif