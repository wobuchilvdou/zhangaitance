from distance_sensor import DistanceSensor, SimulatedDistanceSensor


def test_interface_exists():
    sensor = SimulatedDistanceSensor()
    assert isinstance(sensor, DistanceSensor)


def test_read_returns_distance():
    sensor = SimulatedDistanceSensor(distances_cm=[100])
    reading = sensor.read()
    assert reading.valid
    assert reading.distance_cm == 100


def test_read_cycles():
    sensor = SimulatedDistanceSensor(distances_cm=[10, 20])
    assert sensor.read().distance_cm == 10
    assert sensor.read().distance_cm == 20
    assert sensor.read().distance_cm == 10
