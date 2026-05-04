// This file is part of Slimbook ControlPanel - <https://github.com/Slimbook-Team/slimbook-controlpanel>
// SPDX-FileCopyrightText: 2025 Slimbook development team <dev@slimbook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SLIMBOOK_CONTROLPANEL_SENSORS
#define SLIMBOOK_CONTROLPANEL_SENSORS

#include "common.hpp"

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <thread>

namespace slimbook
{
    namespace controlpanel
    {
        class Node
        {
            public:

            std::string name;
            std::string label;
            double value;
            double raw;
            bool enabled;

            std::chrono::time_point<std::chrono::steady_clock> timestamp;

            Node(std::string name) : name(name),value(0),raw(0), enabled(true) {}
        };

        class Sensor
        {
            public:

            std::string name;

            Sensor(std::string name) : name(name) {}
            virtual ~Sensor() {}

            virtual std::vector<Node>& nodes() = 0;

            virtual void update() = 0;

        };

        class FrequencySensor: public Sensor
        {
            public:

            std::vector<Node> children;

            FrequencySensor();

            std::vector<Node> &nodes()
            {
                return children;
            }

            void update();
        };

        class HwmonTempSensor: public Sensor
        {
            public:

            std::vector<Node> children;

            HwmonTempSensor();

            std::vector<Node> &nodes()
            {
                return children;
            }

            void update();
        };

        class HwmonVoltageSensor: public Sensor
        {
            public:

            std::vector<Node> children;

            HwmonVoltageSensor();

            std::vector<Node> &nodes()
            {
                return children;
            }

            void update();
        };

        class HwmonPowerSensor: public Sensor
        {
            public:

            std::vector<Node> children;

            HwmonPowerSensor();

            std::vector<Node> &nodes()
            {
                return children;
            }

            void update();
        };

        class HwmonFanSensor: public Sensor
        {
            public:

            std::vector<Node> children;

            HwmonFanSensor();

            std::vector<Node> &nodes()
            {
                return children;
            }

            void update();
        };

        class RaplSensor: public Sensor
        {
            public:

            std::vector<Node> children;

            RaplSensor();

            std::vector<Node> &nodes()
            {
                return children;
            }

            void update();
        };

        class CpuLoadSensor: public Sensor
        {
            public:

            std::vector<Node> children;

            CpuLoadSensor();

            std::map<std::string,std::string> read_stat();

            std::vector<Node> &nodes()
            {
                return children;
            }

            void update();
        };

        class RamSensor: public Sensor
        {
            public:

            std::vector<Node> children;

            RamSensor();

            void read_meminfo(int& total,int& available);

            std::vector<Node> &nodes()
            {
                return children;
            }

            void update();
        };
        
        class BatterySensor: public Sensor
        {
            public:

            std::vector<Node> children;

            BatterySensor();

            std::vector<Node> &nodes()
            {
                return children;
            }

            void update();
        };
    }

}

#endif
