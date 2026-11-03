from dataclasses import dataclass, field

from distance_sensor import DistanceReading
from haptics import HapticLevel, pattern_for_level
from risk_model import RiskConfig, RiskLevel, classify_distance
from speech_queue import SpeechItem, SpeechPriority, SpeechQueue


@dataclass
class SupervisorState:
    risk: RiskLevel = RiskLevel.SAFE
    haptic_level: HapticLevel = HapticLevel.OFF
    speech_key: str | None = None
    speech_text: str | None = None


class Supervisor:
    """Maps distance readings to unified risk, haptic, and speech outputs."""

    def __init__(self, cfg: RiskConfig | None = None):
        self.cfg = cfg or RiskConfig()
        self._speech_queue = SpeechQueue()

    def process(self, reading: DistanceReading) -> SupervisorState:
        if not reading.valid:
            self._speech_queue.push(SpeechItem(
                "sensor_fail", "测距传感器异常", SpeechPriority.GROUND_RISK
            ))
        else:
            risk = classify_distance(reading.distance_cm, self.cfg)
            level = {
                RiskLevel.SAFE: HapticLevel.OFF,
                RiskLevel.INFO: HapticLevel.LOW,
                RiskLevel.WARNING: HapticLevel.MEDIUM,
                RiskLevel.CRITICAL: HapticLevel.PULSE,
            }[risk]
            key = f"distance_{risk.name.lower()}"
            texts = {
                RiskLevel.CRITICAL: "危险 靠近",
                RiskLevel.WARNING: "前方有障碍",
                RiskLevel.INFO: "前方有物体",
                RiskLevel.SAFE: None,
            }
            if texts[risk]:
                self._speech_queue.push(SpeechItem(key, texts[risk], SpeechPriority.OBSTACLE))
            state = SupervisorState(risk=risk, haptic_level=level)
            nxt = self._speech_queue.pop()
            if nxt:
                state.speech_key = nxt.key
                state.speech_text = nxt.text
            return state
        state = SupervisorState(risk=RiskLevel.WARNING, haptic_level=HapticLevel.MEDIUM)
        nxt = self._speech_queue.pop()
        if nxt:
            state.speech_key = nxt.key
            state.speech_text = nxt.text
        return state
