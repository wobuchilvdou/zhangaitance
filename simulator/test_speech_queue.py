from speech_queue import SpeechItem, SpeechPriority, SpeechQueue


def test_empty_returns_none():
    q = SpeechQueue()
    assert q.pop() is None


def test_critical_beats_info():
    q = SpeechQueue()
    q.push(SpeechItem("obj", "前方有人", SpeechPriority.OBJECT_INFO))
    q.push(SpeechItem("danger", "危险靠近", SpeechPriority.CRITICAL))
    assert q.pop().key == "danger"
    assert q.pop().key == "obj"


def test_same_key_upgrades_priority():
    q = SpeechQueue()
    q.push(SpeechItem("wall", "前方障碍", SpeechPriority.OBSTACLE))
    q.push(SpeechItem("wall", "紧急避让", SpeechPriority.CRITICAL))
    assert q.pop().text == "紧急避让"
    assert len(q) == 0


def test_same_key_lower_priority_ignored():
    q = SpeechQueue()
    q.push(SpeechItem("wall", "紧急避让", SpeechPriority.CRITICAL))
    q.push(SpeechItem("wall", "前方障碍", SpeechPriority.OBSTACLE))
    assert q.pop().text == "紧急避让"
