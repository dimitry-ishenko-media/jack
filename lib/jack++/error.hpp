////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef JACK_ERROR_HPP
#define JACK_ERROR_HPP

#include <cerrno>
#include <system_error>

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

const std::error_category& error_category();

class error : public std::system_error
{
public:
    ////////////////////
    explicit error(int ev) : std::system_error{ev, jack::error_category()} { }
    error(int ev, const char* msg) : std::system_error{ev, jack::error_category(), msg} { }
    error(int ev, const std::string& msg) : std::system_error{ev, jack::error_category(), msg} { }
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
