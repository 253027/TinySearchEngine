#ifndef __CONFIGURATION_HEAD_H__
#define __CONFIGURATION_HEAD_H__

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

class Configuration
{
public:
    Configuration(const std::string &path);

    std::string &getFilePath(const std::string &name);

private:
    void record(const nlohmann::json &j, const std::string &value = "");
    // 配置文件路径
    std::string _path;
    // 配置文件包含的路径集合
    std::unordered_map<std::string, std::string> _filepath;
};

#endif //__CONFIGURATION_HEAD_H__