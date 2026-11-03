import pytest
from haptics import PATTERNS, HapticLevel, pattern_for_level


def test_off_has_zero_duty():
    p = pattern_for_level(HapticLevel.OFF)
    assert p.duty_percent == 0 and p.on_ms == 0


def test_duty_increases_with_urgency():
    duties = [PATTERNS[lv].duty_percent for lv in HapticLevel]
    assert duties == sorted(duties)


def test_pulse_is_strongest_and_fastest():
    p = pattern_for_level(HapticLevel.PULSE)
    assert p.duty_percent == 100
    assert p.off_ms == min(x.off_ms for x in PATTERNS.values() if x.off_ms > 0)


def test_unknown_level_rejected():
    with pytest.raises(KeyError):
        pattern_for_level(99)
