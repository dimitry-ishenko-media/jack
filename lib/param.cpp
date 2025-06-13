////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "jack++/error.hpp"
#include "jack++/param.hpp"

#include <algorithm> // std::copy
#include <cerrno>
#include <jack/control.h>

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
param::param(jackctl_parameter* p) : param_{p} { }

////////////////////////////////////////////////////////////////////////////////
void param::set(const jack::value& val)
{
    auto type = jackctl_parameter_get_type(param_);
    jackctl_parameter_value value;
    switch (type)
    {
        case JackParamInt: value.i = std::get<int>(val); break;
        case JackParamUInt: value.ui = std::get<unsigned>(val); break;
        case JackParamChar: value.c = std::get<char>(val); break;
        case JackParamString:
        {
            auto s = std::get<std::string>(val).substr(0, sizeof value.str - 1);
            std::copy(s.begin(), s.end() + 1, value.str);
            break;
        }
        case JackParamBool: value.b = std::get<bool>(val); break;
        default: throw jack::error{invalid_value_type, "jackctl_parameter_get_type()"};
    }

    auto success = jackctl_parameter_set_value(param_, &value);
    if (!success) throw jack::error{value_set_error, "jackctl_parameter_set_value()"}; 
}

////////////////////////////////////////////////////////////////////////////////
jack::value param::get() const
{
    auto value = jackctl_parameter_get_value(param_);
    auto type = jackctl_parameter_get_type(param_);

    switch (type)
    {
        case JackParamInt:    return value.i;
        case JackParamUInt:   return value.ui;
        case JackParamChar:   return value.c;
        case JackParamString: return value.str;
        case JackParamBool:   return value.b;
        default: throw jack::error{invalid_value_type, "jackctl_parameter_get_type()"};
    }
}

////////////////////////////////////////////////////////////////////////////////
params extract_from(const jack_list* node)
{
    jack::params params;

    for (; node; node = node->next)
    {
        auto param = static_cast<jackctl_parameter*>(node->data);
        std::string name = jackctl_parameter_get_name(param);

        params.emplace(std::move(name), param);
    }

    return params;
}

////////////////////////////////////////////////////////////////////////////////
param& find(jack::params& params, const std::string& name)
{
    auto it = params.find(name);
    return it != params.end() ? it->second : throw jack::error{invalid_param, name};
}

const param& find(const jack::params& params, const std::string& name)
{
    auto it = params.find(name);
    return it != params.end() ? it->second : throw jack::error{invalid_param, name};
}

////////////////////////////////////////////////////////////////////////////////
}
