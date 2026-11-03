import pytest
from risk_model import RiskConfig, RiskLevel, classify_distance


def test_critical_range():
    cfg = RiskConfig(close_cm=50)
    assert classify_distance(30, cfg) == RiskLevel.CRITICAL


def test_warning_range():
    cfg = RiskConfig(warning_cm=100)
    assert classify_distance(80, cfg) == RiskLevel.WARNING


def test_info_range():
    cfg = RiskConfig(safe_cm=150)
    assert classify_distance(120, cfg) == RiskLevel.INFO


def test_safe_range():
    cfg = RiskConfig(safe_cm=150)
    assert classify_distance(200, cfg) == RiskLevel.SAFE


def test_invalid_distance():
    cfg = RiskConfig()
    with pytest.raises(ValueError):
        classify_distance(0, cfg)
