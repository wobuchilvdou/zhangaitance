from dataclasses import dataclass
from typing import Optional


@dataclass
class DistanceReading:
    distance_cm: float
    valid: bool = True
    timestamp_ms: int = 0


class DistanceSensor:
    """Interface for distance sensors. Implementations may use GPIO, UART, or I2C."""

    def read(self) -> Optional[DistanceReading]:
        raise NotImplementedError


class SimulatedDistanceSensor(DistanceSensor):
    def __init__(self, distances_cm=None, interval_ms: int = 100):
        self._distances = distances_cm if distances_cm is not None else [200, 150, 100, 80, 40, 20]
        self._index = 0
        self._interval_ms = interval_ms

    def read(self) -> DistanceReading:
        distance = self._distances[self._index % len(self._distances)]
        self._index += 1
        return DistanceReading(distance_cm=distance, timestamp_ms=self._index * self._interval_ms)
