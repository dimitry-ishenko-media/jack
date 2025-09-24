////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef JACK_PORT_HPP
#define JACK_PORT_HPP

////////////////////////////////////////////////////////////////////////////////
#include "jack++/types.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

struct _jack_client;
using jack_client = _jack_client;

struct _jack_port;
using jack_port = _jack_port;

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
class port
{
public:
    ////////////////////
    std::string name() const;
    bool is_physical() const;

    constexpr auto&& format() const noexcept { return fmt_; }

protected:
    ////////////////////
    port(jack_client*, std::string_view name, jack::format, jack::dir);

    std::span<sample> buffer(std::size_t) const;

private:
    ////////////////////
    std::unique_ptr<jack_port, std::function<void(jack_port*)>> port_;
    jack::format fmt_;
};

////////////////////////////////////////////////////////////////////////////////
class input_port : public port
{
public:
    ////////////////////
    input_port(jack_client* client, std::string_view name, jack::format fmt) :
        port{client, name, fmt, jack::in}
    { }

    auto recv(std::size_t size) const { return buffer(size); }
};

class output_port : public port
{
public:
    ////////////////////
    output_port(jack_client* client, std::string_view name, jack::format fmt) :
        port{client, name, fmt, jack::out}
    { }

    auto request(std::size_t size) const { return buffer(size); }
};

using input_ports = std::vector<input_port>;
using output_ports = std::vector<output_port>;

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
