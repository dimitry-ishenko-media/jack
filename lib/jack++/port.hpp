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

#include <audio++.hpp>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
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

    audio::span buffer(std::size_t) const;

protected:
    ////////////////////
    port(jack_client*, const std::string& name, audio::format, jack::dir);

private:
    ////////////////////
    std::unique_ptr<jack_port, std::function<void(jack_port*)>> port_;
    audio::format fmt_;
};

////////////////////////////////////////////////////////////////////////////////
class input_port : public port
{
public:
    ////////////////////
    input_port(jack_client* client, const std::string& name, audio::format fmt) :
        port{client, name, fmt, jack::in}
    { }
};

class output_port : public port
{
public:
    ////////////////////
    output_port(jack_client* client, const std::string& name, audio::format fmt) :
        port{client, name, fmt, jack::out}
    { }
};

using input_ports = std::vector<input_port>;
using output_ports = std::vector<output_port>;

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
