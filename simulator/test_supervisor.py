from distance_sensor import DistanceReading
from haptics import HapticLevel
from risk_model import RiskLevel
from speech_queue import SpeechPriority, SpeechItem
from supervisor import Supervisor


def test_critical_maps_to_pulse():
    sup = Supervisor()
    state = sup.process(DistanceReading(30))
    assert state.risk == RiskLevel.CRITICAL
    assert state.haptic_level == HapticLevel.PULSE
    assert state.speech_text is not None


def test_safe_has_no_speech():
    sup = Supervisor()
    state = sup.process(DistanceReading(200))
    assert state.risk == RiskLevel.SAFE
    assert state.haptic_level == HapticLevel.OFF
    assert state.speech_text is None


def test_invalid_reading_triggers_sensor_alert():
    sup = Supervisor()
    state = sup.process(DistanceReading(0, valid=False))
    assert "异常" in state.speech_text


def test_critical_preempts_info():
    sup = Supervisor()
    sup.process(DistanceReading(120))  # queues INFO speech
    state = sup.process(DistanceReading(30))  # queues CRITICAL speech
    assert state.speech_key == "distance_critical"
