# 🚗 TRAFFINET: A Smart 5G-Enabled Traffic Management System

TRAFFINET is an intelligent traffic-control prototype that combines **IoT vehicle-density sensing**, **adaptive traffic-signal control**, and a **5G communication concept** for low-latency coordination between intersections and edge devices.

> **Project status:** Prototype / controlled simulation  
> **Reported result:** ~25% congestion reduction in controlled scenarios.

## Problem

Conventional traffic lights operate on fixed timings. When traffic density changes, a road can remain green while another lane is heavily congested. This causes:

- unnecessary waiting time
- fuel wastage
- traffic congestion
- slower emergency response

## Solution

TRAFFINET detects traffic density using IR sensors and dynamically changes signal priority.

### Basic flow

```text
IR Sensors
    ↓
Arduino UNO
    ↓
Vehicle-density calculation
    ↓
Adaptive signal decision
    ↓
5G / low-latency communication layer
    ↓
Intersection controller
    ↓
Traffic lights
```

For an emergency vehicle, the intended flow is:

```text
Emergency vehicle detected
        ↓
Priority message
        ↓
Low-latency communication
        ↓
Controller receives request
        ↓
Current cycle is overridden safely
        ↓
Emergency route gets GREEN
```

## Hardware

- Arduino UNO
- 6 × IR obstacle/vehicle-detection sensors
- 3 × traffic-light groups (Red/Yellow/Green)
- External 5G communication gateway/module for a real deployment
- 7.5 V DC supply as represented in the prototype wiring
- Breadboard / jumper wires as required

### Important architecture note

The Arduino UNO shown in the prototype does **not itself provide 5G connectivity**. The 5G portion is represented as a communication layer/gateway. A real deployment would place a suitable 5G modem/router or 5G-capable edge gateway between the intersection controller and the network.

## Repository structure

```text
TRAFFINET/
├── README.md
├── LICENSE
├── .gitignore
├── arduino/
│   └── traffinet_controller.ino
├── simulation/
│   ├── traffic_simulator.py
│   └── requirements.txt
├── docs/
│   ├── architecture.md
│   ├── wiring.md
│   └── demo.md
└── data/
    └── sample_traffic.csv
```

## Arduino prototype

The Arduino sketch:

1. reads six IR sensors
2. converts sensor states into lane-density values
3. gives the busiest approach higher priority
4. prevents unsafe direct switching by using a yellow transition
5. supports an emergency-priority input
6. prints controller events to Serial for debugging

## Simulation

The Python simulator compares:

- fixed-time traffic control
- adaptive traffic control

It generates random vehicle arrivals and calculates average waiting time and a simple congestion index.

Run:

```bash
cd simulation
pip install -r requirements.txt
python traffic_simulator.py
```

## Example project metrics

The project presentation reports approximately **25% congestion reduction in controlled scenarios**. This number should be presented as a prototype/simulation result, not as a field-tested city-wide performance guarantee.

Useful metrics for future experiments:

- average vehicle waiting time
- queue length
- throughput
- congestion index
- emergency-vehicle clearance time
- number of signal changes
- communication latency

## Why 5G?

5G is relevant when the system is expanded from a single local Arduino prototype to **connected intersections and edge devices**.

The value proposition is:

- low-latency communication for time-sensitive messages
- high device density for large IoT deployments
- reliable connectivity for coordinated intersections
- support for edge/cloud traffic analytics
- scalable communication between vehicles, roadside devices and controllers

The key point is not that an Arduino needs 5G to switch an LED. A local controller can do that without 5G. The 5G layer becomes useful when **multiple intersections, emergency vehicles, sensors and edge services must coordinate in near real time**.

## Future scope

- real 5G modem/gateway integration
- MQTT/5G message broker
- emergency-vehicle V2X communication
- camera-based vehicle detection
- machine-learning traffic prediction
- multi-intersection coordination
- edge computing
- live dashboard
- GPS-based ambulance routing
- reinforcement-learning signal optimization

