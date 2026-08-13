# Demo / Viva Flow

## 1. Start with the problem

"Traditional traffic signals often use fixed timings and do not react to real-time traffic density."

## 2. Demonstrate sensing

Place an object/vehicle in front of different IR sensors.

Show the Serial Monitor:

```text
Density | R1: 2 R2: 0 R3: 1
```

## 3. Demonstrate adaptive control

Increase traffic density on another road.

The controller selects the busier road after the minimum green period and changes:

```text
GREEN → YELLOW → ALL RED → GREEN
```

## 4. Demonstrate emergency priority

Activate the emergency input.

The controller transitions to the configured emergency route.

## 5. Explain the 5G component honestly

Say:

"At the prototype level, the Arduino handles local sensing and signal control. The 5G layer is the communication architecture for connecting intersections, edge devices and emergency vehicles. A production version would use a dedicated 5G gateway/modem."

## 6. Evaluation result

The project documentation reports approximately 25% congestion reduction in controlled scenarios.

Phrase it as:

"Under our controlled simulation conditions, the adaptive approach showed approximately 25% lower congestion."

Do not claim a 25% reduction in real city traffic unless you have field-test data.
