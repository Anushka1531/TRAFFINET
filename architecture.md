# TRAFFINET Architecture

## 1. Sensing layer

IR sensors detect whether vehicles are present near each approach.

```text
Vehicle
   ↓
IR Sensor
   ↓
Arduino UNO
```

## 2. Edge-control layer

The Arduino aggregates sensor states into a simple density score.

For each road:

```text
density = detected_sensor_1 + detected_sensor_2
```

The controller selects the road with the highest demand, subject to minimum/maximum green constraints.

## 3. Communication layer

For a real connected deployment:

```text
Intersection Controller
        ↓
5G Gateway / Modem
        ↓
5G Network
        ↓
Edge Server / Traffic Management Platform
        ↓
Other Intersections / Emergency Vehicles
```

The prototype's Arduino circuit should therefore be described as the **local intersection controller**, while the 5G portion is the **networking architecture / simulation concept** unless a physical 5G modem is actually connected.

## 4. Emergency priority

A priority message can be represented as:

```json
{
  "type": "emergency",
  "vehicle": "ambulance",
  "intersection": "I01",
  "route": "ROAD_A",
  "priority": 1
}
```

The production system would authenticate and validate such messages before overriding signals.

## 5. Why edge + 5G?

A practical system can keep immediate safety-critical signal logic at the intersection edge while using 5G for coordination and time-sensitive data exchange.

This avoids making the traffic light dependent on a remote server for every individual LED transition.
