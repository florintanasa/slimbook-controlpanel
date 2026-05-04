// This file is part of Slimbook ControlPanel - <https://github.com/Slimbook-Team/slimbook-controlpanel>
// SPDX-FileCopyrightText: 2025 Slimbook development team <dev@slimbook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "sensors.hpp"
#include "common.hpp"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <regex>

using namespace slimbook::controlpanel;
using namespace std;

FrequencySensor::FrequencySensor():Sensor("cpu.freq")
{
    const std::filesystem::path base{"/sys/devices/system/cpu"};

    for (auto const& dir : std::filesystem::directory_iterator{base}) {
        if (dir.is_directory()) {
            string filename = dir.path().filename();
            if (filename.find("cpu") == 0) {
                std::filesystem::path node = dir / std::filesystem::path("cpufreq/scaling_cur_freq");

                if (std::filesystem::exists(node)) {
                    Node child = Node(filename);
                    children.push_back(child);
                }
            }

        }
    }

    children.push_back(Node("average"));

}

void FrequencySensor::update()
{
    Node* average = nullptr;

    double avg = 0;

    for (Node& node:children) {
        if (node.name == "average") {
            average = &node;
        }
        else {
            read_device("/sys/devices/system/cpu/"+node.name+"/cpufreq/scaling_cur_freq",node.raw);

            node.value = node.raw / 1000.0;
            avg = avg + node.raw;
            node.timestamp = std::chrono::steady_clock::now();
        }
    }

    if (average and children.size() > 1) {
        average-> raw = avg / (children.size()-1);
        average->value = average->raw / 1000.0;
        average->timestamp = std::chrono::steady_clock::now();
    }
}

HwmonTempSensor::HwmonTempSensor(): Sensor("hwmon.temp")
{
    const std::filesystem::path base{"/sys/class/hwmon"};
    const std::regex hwmon_regex("hwmon[0-9]+");
    const std::regex temp_regex("temp[0-9]+_input");

    for (auto const& dir : std::filesystem::directory_iterator{base}) {
        if (dir.is_directory()) {
            string parent = dir.path().filename();
            if (std::regex_match(parent,hwmon_regex)) {

                std::string nodename;
                read_device(dir / std::filesystem::path("name"),nodename);

                for (auto const& file : std::filesystem::directory_iterator{dir}) {
                    string filename = file.path().filename();
                    if (std::regex_match(filename, temp_regex)) {

                        try {
                            double sink;
                            read_device("/sys/class/hwmon/" + parent + "/" + filename, sink);
                        }
                        catch (std::exception & e) {
                            clog<<"ignoring device "<<filename<<endl;
                            continue;
                        }

                        Node child = Node(parent + "/" + filename);
                        
                        std::filesystem::path label_node = std::regex_replace(filename,std::regex("_input"),"_label");
                        
                        if (std::filesystem::exists(dir/label_node)) {
                            string tmp;
                            read_device(dir/label_node,tmp);
                            child.label = nodename + "." + tmp;
                        }
                        else {
                            child.label = nodename + "." + filename;
                        }
                        children.push_back(child);
                    }

                }

            }

        }
    }
}

void HwmonTempSensor::update()
{
    for (Node& node:children) {

        read_device("/sys/class/hwmon/"+node.name,node.raw);

        node.value = node.raw / 1000.0;
        node.timestamp = std::chrono::steady_clock::now();

    }
}

HwmonVoltageSensor::HwmonVoltageSensor():Sensor("hwmon.voltage")
{
    const std::filesystem::path base{"/sys/class/hwmon"};
    const std::regex hwmon_regex("hwmon[0-9]+");
    const std::regex temp_regex("in[0-9]+_input");

    for (auto const& dir : std::filesystem::directory_iterator{base}) {
        if (dir.is_directory()) {
            string parent = dir.path().filename();
            if (std::regex_match(parent,hwmon_regex)) {

                std::string nodename;
                read_device(dir / std::filesystem::path("name"),nodename);

                for (auto const& file : std::filesystem::directory_iterator{dir}) {
                    string filename = file.path().filename();
                    if (std::regex_match(filename, temp_regex)) {

                        try {
                            double sink;
                            read_device("/sys/class/hwmon/" + parent + "/" + filename, sink);
                        }
                        catch (std::exception & e) {
                            clog<<"ignoring device "<<filename<<endl;
                            continue;
                        }

                        Node child = Node(parent + "/" + filename);
                        
                        std::filesystem::path label_node = std::regex_replace(filename,std::regex("_input"),"_label");
                        
                        if (std::filesystem::exists(dir/label_node)) {
                            string tmp;
                            read_device(dir/label_node,tmp);
                            child.label = nodename + "." + tmp;
                        }
                        else {
                            child.label = nodename + "." + filename;
                        }
                        children.push_back(child);
                    }

                }

            }

        }
    }
}

void HwmonVoltageSensor::update()
{
    for (Node& node:children) {

        read_device("/sys/class/hwmon/"+node.name,node.raw);

        node.value = node.raw / 1000.0;
        node.timestamp = std::chrono::steady_clock::now();

    }
}

HwmonPowerSensor::HwmonPowerSensor():Sensor("hwmon.power")
{
    const std::filesystem::path base{"/sys/class/hwmon"};
    const std::regex hwmon_regex("hwmon[0-9]+");
    const std::regex temp_regex("power[0-9]+_input");

    for (auto const& dir : std::filesystem::directory_iterator{base}) {
        if (dir.is_directory()) {
            string parent = dir.path().filename();
            if (std::regex_match(parent,hwmon_regex)) {

                std::string nodename;
                read_device(dir / std::filesystem::path("name"),nodename);

                for (auto const& file : std::filesystem::directory_iterator{dir}) {
                    string filename = file.path().filename();
                    if (std::regex_match(filename, temp_regex)) {

                        try {
                            double sink;
                            read_device("/sys/class/hwmon/" + parent + "/" + filename, sink);
                        }
                        catch (std::exception & e) {
                            clog<<"ignoring device "<<filename<<endl;
                            continue;
                        }

                        Node child = Node(parent + "/" + filename);
                        
                        std::filesystem::path label_node = std::regex_replace(filename,std::regex("_input"),"_label");
                        
                        if (std::filesystem::exists(dir/label_node)) {
                            string tmp;
                            read_device(dir/label_node,tmp);
                            child.label = nodename + "." + tmp;
                        }
                        else {
                            child.label = nodename + "." + filename;
                        }
                        
                        children.push_back(child);
                    }

                }

            }

        }
    }
}

void HwmonPowerSensor::update()
{
    for (Node& node:children) {

        read_device("/sys/class/hwmon/"+node.name,node.raw);

        node.value = node.raw / 1000000.0;
        node.timestamp = std::chrono::steady_clock::now();

    }
}

HwmonFanSensor::HwmonFanSensor():Sensor("hwmon.fan")
{
    const std::filesystem::path base{"/sys/class/hwmon"};
    const std::regex hwmon_regex("hwmon[0-9]+");
    const std::regex temp_regex("fan[0-9]+_input");

    for (auto const& dir : std::filesystem::directory_iterator{base}) {
        if (dir.is_directory()) {
            string parent = dir.path().filename();
            if (std::regex_match(parent,hwmon_regex)) {

                std::string nodename;
                read_device(dir / std::filesystem::path("name"),nodename);

                for (auto const& file : std::filesystem::directory_iterator{dir}) {
                    string filename = file.path().filename();
                    if (std::regex_match(filename, temp_regex)) {
                    
                        try {
                            double sink;
                            read_device("/sys/class/hwmon/" + parent + "/" + filename, sink);
                        }
                        catch (std::exception & e) {
                            clog<<"ignoring device "<<filename<<endl;
                            continue;
                        }
                        
                        Node child = Node(parent + "/" + filename);
                        
                        std::filesystem::path label_node = std::regex_replace(filename,std::regex("_input"),"_label");
                        
                        if (std::filesystem::exists(dir/label_node)) {
                            string tmp;
                            read_device(dir/label_node,tmp);
                            child.label = nodename + "." + tmp;
                        }
                        else {
                            child.label = nodename + "." + filename;
                        }
                        
                        children.push_back(child);
                    }

                }

            }

        }
    }
}

void HwmonFanSensor::update()
{
    for (Node& node:children) {

        read_device("/sys/class/hwmon/"+node.name,node.raw);

        node.value = node.raw;
        node.timestamp = std::chrono::steady_clock::now();

    }
}

RaplSensor::RaplSensor():Sensor("rapl")
{
    const std::filesystem::path base{"/sys/class/powercap/"};
    const std::regex rapl_regex("intel-rapl:[0-9]");

    for (auto const& dir : std::filesystem::directory_iterator{base}) {
        if (dir.is_directory()) {
            string filename = dir.path().filename();
            if (std::regex_match(filename, rapl_regex)) {
                std::filesystem::path node = dir / std::filesystem::path("energy_uj");

                if (std::filesystem::exists(node)) {
                    Node child = Node(filename);
                    read_device("/sys/class/powercap/"+filename+"/energy_uj",child.raw);
                    child.timestamp = std::chrono::steady_clock::now();
                    children.push_back(child);
                }
            }

        }
    }
}

void RaplSensor::update()
{
    for (Node& node:children) {
        double current;
        auto now = std::chrono::steady_clock::now();
        double delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - node.timestamp).count() / 1000.0;

        read_device("/sys/class/powercap/"+node.name+"/energy_uj", current);
        double watts = (current - node.raw) / 1000000.0 / delta;
        node.value = watts;
        node.timestamp = now;
        node.raw = current;
    }
}

CpuLoadSensor::CpuLoadSensor():Sensor("cpu.load")
{
    map<std::string,std::string> values = read_stat();

    for (auto cpu:values) {
        Node child = Node(cpu.first);
        child.raw = std::stol(cpu.second);
        child.timestamp = std::chrono::steady_clock::now();
        children.push_back(child);
    }

    children.push_back(Node("average"));
}

map<std::string,std::string> CpuLoadSensor::read_stat()
{
    ifstream file;
    string line;
    const std::regex cpu_regex("cpu[0-9]+");

    map<std::string,std::string> values;

    file.open("/proc/stat");
    while(!file.eof()) {
        std::getline(file, line);
        vector<std::string> tmp = split(line,' ');
        if(tmp.size() > 0 and std::regex_match(tmp[0], cpu_regex)) {
            //cout<<tmp[0]<<" idle "<<tmp[4]<<" %"<<endl;
            values[tmp[0]] = tmp[4];
        }
    }
    file.close();

    return values;
}

void CpuLoadSensor::update()
{
    map<std::string,std::string> values = read_stat();
    auto now = std::chrono::steady_clock::now();

    double avg = 0;
    Node* average = nullptr;

    for (Node& node:children) {

        if (node.name == "average") {
            average = &node;
        }
        else {
            double actual = std::stoll(values[node.name]);
            double delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - node.timestamp).count() / 1000.0;
            node.value = (actual - node.raw) / delta;
            node.value = 100 - std::min(node.value,100.0);

            node.raw = actual;
            node.timestamp = now;
            avg = avg + node.value;
        }
    }

    if (average and children.size() > 1) {
        avg = avg / (children.size() - 1);
        average->raw = avg;
        average->value = avg;
        average->timestamp = now;
    }
}

RamSensor::RamSensor():Sensor("ram")
{
    children.push_back(Node("total"));
    children.push_back(Node("free"));
    children.push_back(Node("usage"));

    update();
}

void RamSensor::read_meminfo(int& total, int& available)
{
    ifstream file("/proc/meminfo");
    string line;
    while (std::getline(file, line)) {
        if (line.find("MemTotal:") == 0) {
            vector<string> tmp = split(line, ' ');
            total = std::stoll(tmp[tmp.size() - 2]);
        }
        if (line.find("MemAvailable:") == 0) {
            vector<string> tmp = split(line, ' ');
            available = std::stoll(tmp[tmp.size() - 2]);
        }
    }
    file.close();
}

void RamSensor::update()
{
    int total;
    int free;
    read_meminfo(total,free);
    //clog<<"meminfo "<<total<<" "<<free<<endl;

    for (Node& node:children) {
        if (node.name == "total") {
            node.raw = total;
            node.value = total / (1024 * 1024);
            node.timestamp = std::chrono::steady_clock::now();
        }

        if (node.name == "free") {
            node.raw = free;
            node.value = free / (1024 * 1024);
            node.timestamp = std::chrono::steady_clock::now();
        }
        
        if (node.name == "usage") {
            node.raw = total - free;
            // Percents: (usage / total) * 100
            node.value = (double)(total - free) / total * 100.0; 
            node.timestamp = std::chrono::steady_clock::now();
        }
    }
}

BatterySensor::BatterySensor():Sensor("battery")
{
    const std::filesystem::path base{"/sys/class/power_supply/"};
    const std::regex batt_regex("BAT[0-9]");
    
    for (auto const& dir : std::filesystem::directory_iterator{base}) {
        if (dir.is_directory()) {
            string filename = dir.path().filename();
            
            if (std::regex_match(filename, batt_regex)) {
                std::filesystem::path node = dir / std::filesystem::path("capacity");

                if (std::filesystem::exists(node)) {
                    Node child = Node(filename + "/capacity");
                    //read_device("/sys/class/power_supply/"+filename+"/capacity",child.raw);
                    //child.timestamp = std::chrono::steady_clock::now();
                    children.push_back(child);
                }
            }

        }
    }
    
}

void BatterySensor::update()
{
    for (Node& node:children) {

        read_device("/sys/class/power_supply/"+node.name,node.raw);

        node.value = node.raw;
        node.timestamp = std::chrono::steady_clock::now();

    }
}
