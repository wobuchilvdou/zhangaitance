from dataclasses import dataclass
from enum import IntEnum


class HapticLevel(IntEnum):
    OFF = 0
    LOW = 1
    MEDIUM = 2
    HIGH = 3
    PULSE = 4


@dataclass(frozen=True)
class VibrationPattern:
    duty_percent: int
    on_ms: int
    off_ms: int


PATTERNS = {
    HapticLevel.OFF: VibrationPattern(0, 0, 0),
    HapticLevel.LOW: VibrationPattern(40, 60, 540),
    HapticLevel.MEDIUM: VibrationPattern(60, 80, 320),
    HapticLevel.HIGH: VibrationPattern(80, 100, 150),
    HapticLevel.PULSE: VibrationPattern(100, 200, 100),
}


def pattern_for_level(level: HapticLevel) -> VibrationPattern:
    return PATTERNS[level]
