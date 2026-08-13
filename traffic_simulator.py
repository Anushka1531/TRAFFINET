"""
TRAFFINET - Controlled traffic-flow simulation

This is a lightweight demonstration model for comparing fixed-time
and adaptive traffic signal control.

It is NOT a microscopic traffic simulator and does not represent
real-world 5G latency, vehicle dynamics, or city traffic exactly.
"""

from dataclasses import dataclass
import random
import statistics

ROADS = 3
SIMULATION_STEPS = 180
ARRIVAL_PROBABILITY = 0.35
FIXED_GREEN_STEPS = 20


@dataclass
class Result:
    total_wait: int
    average_queue: float
    peak_queue: int


def generate_arrivals(seed: int = 42):
    random.seed(seed)
    return [
        [1 if random.random() < ARRIVAL_PROBABILITY else 0
         for _ in range(SIMULATION_STEPS)]
        for _ in range(ROADS)
    ]


def simulate_fixed(arrivals):
    queues = [0] * ROADS
    total_wait = 0
    queue_history = []
    active = 0

    for t in range(SIMULATION_STEPS):
        for road in range(ROADS):
            queues[road] += arrivals[road][t]

        if t % FIXED_GREEN_STEPS == 0:
            active = (active + 1) % ROADS

        # One vehicle clears per step from the green road.
        if queues[active] > 0:
            queues[active] -= 1

        total_wait += sum(queues)
        queue_history.append(sum(queues))

    return Result(total_wait, statistics.mean(queue_history), max(queue_history))


def simulate_adaptive(arrivals):
    queues = [0] * ROADS
    total_wait = 0
    queue_history = []
    active = 0
    green_age = 0

    for t in range(SIMULATION_STEPS):
        for road in range(ROADS):
            queues[road] += arrivals[road][t]

        busiest = max(range(ROADS), key=lambda r: queues[r])

        # Stay on the current road briefly to avoid rapid oscillation.
        if green_age >= 8 and queues[busiest] > queues[active]:
            active = busiest
            green_age = 0

        if queues[active] > 0:
            queues[active] -= 1

        green_age += 1
        total_wait += sum(queues)
        queue_history.append(sum(queues))

    return Result(total_wait, statistics.mean(queue_history), max(queue_history))


def main():
    arrivals = generate_arrivals()

    fixed = simulate_fixed(arrivals)
    adaptive = simulate_adaptive(arrivals)

    improvement = (
        (fixed.total_wait - adaptive.total_wait)
        / fixed.total_wait * 100
        if fixed.total_wait else 0
    )

    print("TRAFFINET CONTROLLED SIMULATION")
    print("--------------------------------")
    print(f"Fixed-time total waiting units : {fixed.total_wait}")
    print(f"Adaptive total waiting units   : {adaptive.total_wait}")
    print(f"Fixed average queue             : {fixed.average_queue:.2f}")
    print(f"Adaptive average queue          : {adaptive.average_queue:.2f}")
    print(f"Fixed peak queue                : {fixed.peak_queue}")
    print(f"Adaptive peak queue             : {adaptive.peak_queue}")
    print(f"Waiting-time improvement        : {improvement:.2f}%")
    print()
    print("Note: The result changes with the traffic seed and simulation assumptions.")
    print("Do not present this output as a real-world field-test result.")


if __name__ == "__main__":
    main()
