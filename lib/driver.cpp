////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "jack++/driver.hpp"
#include "jack++/error.hpp"

#include <jack/control.h>

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
void driver::set_params(jackctl_server* server, jackctl_driver** driver)
{
    for (auto node = jackctl_server_get_drivers_list(server); node; node = node->next)
    {
        *driver = static_cast<jackctl_driver*>(node->data);
        if (name_ == jackctl_driver_get_name(*driver))
        {
            auto params = jackctl_driver_get_parameters(*driver);
            while (options_.size())
            {
                auto node = options_.extract(options_.begin());
                param::from_list(params, node.key()).set(node.mapped());
            }
            return;
        }
    }
    throw jack::error{invalid_driver, name_};
}

////////////////////////////////////////////////////////////////////////////////
alsa_driver::alsa_driver(const alsa_options& options) : driver{"alsa"}
{
    if (options.device) options_.emplace("device", *options.device);
    if (options.chan_in) options_.emplace("inchannels", *options.chan_in);
    if (options.chan_out) options_.emplace("outchannels", *options.chan_out);
    if (options.rate) options_.emplace("rate", *options.rate);
    if (options.period) options_.emplace("period", *options.period);
    if (options.periods) options_.emplace("nperiods", *options.periods);
}

////////////////////////////////////////////////////////////////////////////////
void alsa_driver::get_params(jackctl_server* server, jackctl_driver* driver)
{
    auto params = jackctl_driver_get_parameters(driver);
    device_  = param::from_list(params, "device").get_as<std::string>();
    chan_in_ = static_cast<jack::chans>(param::from_list(params, "inchannels").get_as<unsigned>());
    chan_out_= static_cast<jack::chans>(param::from_list(params, "outchannels").get_as<unsigned>());
    rate_    = static_cast<jack::rate >(param::from_list(params, "rate").get_as<unsigned>());
    period_  = param::from_list(params, "period").get_as<unsigned>();
    periods_ = param::from_list(params, "nperiods").get_as<unsigned>();
}

////////////////////////////////////////////////////////////////////////////////
}
