////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "jack++/error.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

class error_category_ : public std::error_category
{
public:
    ////////////////////
    virtual const char* name() const noexcept override { return "jack"; }

    virtual std::string message(int ev) const override
    {
        switch (ev)
        {
            case server_startup_error:  return "Server start-up error";
            case connection_failed:     return "Connection failed";
            case connection_exists:     return "Connection already exists";
            case invalid_port:          return "Invalid port";
            case client_create_error:   return "Client creation error";
            case port_register_error:   return "Port registration error";
            case invalid_driver:        return "Invalid driver";
            case invalid_param:         return "Invalid parameter";
            case invalid_value_type:    return "Invalid parameter value type";
            case value_set_error:       return "Error setting parameter value";
            default:                    return "Unknown error";
        }
    }

    virtual std::error_condition default_error_condition(int ev) const noexcept override
    {
        switch (ev)
        {
            case server_startup_error:  return std::errc::io_error;
            case connection_failed:     return std::errc::connection_refused;
            case connection_exists:     return std::errc::already_connected;
            case invalid_port:          return std::errc::invalid_argument;
            case client_create_error:   return std::errc::io_error;
            case port_register_error:   return std::errc::io_error;
            case invalid_driver:        return std::errc::invalid_argument;
            case invalid_param:         return std::errc::invalid_argument;
            case invalid_value_type:    return std::errc::invalid_argument;
            case value_set_error:       return std::errc::io_error;
            default: return {ev, jack::error_category()};
        }
    }
};

}

////////////////////////////////////////////////////////////////////////////////
const std::error_category& error_category()
{
    static error_category_ instance;
    return instance;
}

////////////////////////////////////////////////////////////////////////////////
}
