////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef JACK_SERVER_HPP
#define JACK_SERVER_HPP

////////////////////////////////////////////////////////////////////////////////
#include "driver.hpp"
#include "param.hpp"

#include <memory>
#include <optional>
#include <string>

struct jackctl_server;

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
struct server_options
{
    std::optional<bool> realtime;
    std::optional<int> priority;
};

class server
{
public:
    ////////////////////
    server(const std::string& name, jack::driver& driver, const server_options& options = { }) :
        server{name, std::move(driver), options}
    { }
    server(const std::string& name, driver&&, const server_options& = { });
    ~server();

    ////////////////////
    std::string name() const;
    bool realtime() const;
    std::optional<int> priority() const;

    void wait_for_signal();

private:
    ////////////////////
    std::unique_ptr<jackctl_server, void(*)(jackctl_server*)> server_;
    params params_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
