from dataclasses import dataclass
from enum import IntEnum


class RiskLevel(IntEnum):
    SAFE = 0
    INFO = 1
    WARNING = 2
    DANGER = 3
    CRITICAL = 4


@dataclass
class RiskConfig:
    close_cm: float = 50.0
    warning_cm: float = 100.0
    safe_cm: float = 150.0


def classify_distance(distance_cm: float, cfg: RiskConfig) -> RiskLevel:
    if distance_cm <= 0:
        raise ValueError("distance must be positive")
    if distance_cm <= cfg.close_cm:
        return RiskLevel.CRITICAL
    if distance_cm <= cfg.warning_cm:
        return RiskLevel.WARNING
    if distance_cm <= cfg.safe_cm:
        return RiskLevel.INFO
    return RiskLevel.SAFE
