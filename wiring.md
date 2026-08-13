# Wiring Reference

The supplied prototype image shows:

- Arduino UNO
- 3 traffic-light groups
- 6 IR sensor modules
- external DC supply

## Suggested logical pin map

| Component | Arduino pin |
|---|---|
| Road A Red | D2 |
| Road A Yellow | D3 |
| Road A Green | D4 |
| Road B Red | D5 |
| Road B Yellow | D6 |
| Road B Green | D7 |
| Road C Red | D8 |
| Road C Yellow | D9 |
| Road C Green | D10 |
| Emergency input | D11 |
| IR sensor A1 | A0 |
| IR sensor A2 | A1 |
| IR sensor B1 | A2 |
| IR sensor B2 | A3 |
| IR sensor C1 | A4 |
| IR sensor C2 | A5 |

## Sensor logic

The sketch assumes:

```text
LOW  -> vehicle detected
HIGH -> no vehicle
```

If your IR module behaves in the opposite direction, invert the condition in `readTrafficDensity()`.

## Power warning

Do not blindly reproduce the 7.5 V wiring shown in a diagram without checking the exact component ratings and current requirements.

The Arduino, LEDs, sensor modules and any external module should have appropriate regulated power and a common ground where required. Use resistors for discrete LEDs if your traffic-light modules do not already contain current-limiting resistors.

**Verify the actual circuit with a multimeter and component datasheets before applying power.**
