#include "../include/Configuration.h"
#include "../include/utility.h"
#include <unistd.h>

Configuration::Configuration(const std::string &path) : _path(path)
{
    ERROR_CHECK(::access(_path.c_str(), F_OK) == -1, "path not exits");
    std::ifstream in(path);
    nlohmann::json parse;
    in >> parse;
    record(parse);
    in.close();
}

std::string &Configuration::getFilePath(const std::string &name)
{
    return _filepath[name];
}

void Configuration::record(const nlohmann::json &j, const std::string &value)
{
    if (j.is_object())
    {
        for (const auto &item : j.items())
            record(item.value(), item.key());
    }
    else if (j.is_array())
    {
        for (const auto &item : j)
            record(item, value);
    }
    else
        _filepath[value] = j;
}
