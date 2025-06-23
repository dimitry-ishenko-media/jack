////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef JACK_SERVER_HPP
#define JACK_SERVER_HPP

////////////////////////////////////////////////////////////////////////////////
#include "jack++/driver.hpp"

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
    auto&& name() const noexcept { return name_; }
    auto&& realtime() const noexcept { return realtime_; }
    auto&& priority() const noexcept { return priority_; }

    void wait_for_signal();

private:
    ////////////////////
    std::unique_ptr<jackctl_server, void(*)(jackctl_server*)> server_;

    std::string name_;
    bool realtime_;
    std::optional<int> priority_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
