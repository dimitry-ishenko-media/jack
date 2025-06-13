////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef JACK_ERROR_HPP
#define JACK_ERROR_HPP

#include <system_error>

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

enum errc
{
    server_startup_error = 1,
    connection_failed,
    connection_exists,
    invalid_port,
    client_create_error,
    port_register_error,
    invalid_driver,
    invalid_param,
    invalid_value_type,
    value_set_error,
};

const std::error_category& error_category();

inline auto make_error_code(jack::errc e) noexcept { return std::error_code{e, jack::error_category()}; }

}

////////////////////////////////////////////////////////////////////////////////
namespace std
{

template<>
struct is_error_code_enum<jack::errc> : public true_type { };

}

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

class error : public std::system_error
{
public:
    ////////////////////
    explicit error(errc e) : std::system_error{e} { }
    error(errc e, const char* msg) : std::system_error{e, msg} { }
    error(errc e, const std::string& msg) : std::system_error{e, msg} { }
};

}

////////////////////////////////////////////////////////////////////////////////
#endif
