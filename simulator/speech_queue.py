from dataclasses import dataclass
from enum import IntEnum
from typing import Optional


class SpeechPriority(IntEnum):
    OBJECT_INFO = 1
    GROUND_RISK = 2
    OBSTACLE = 3
    CRITICAL = 4


@dataclass
class SpeechItem:
    key: str
    text: str
    priority: SpeechPriority


class SpeechQueue:
    """Priority queue with dedup: same key keeps only the highest priority pending item."""

    def __init__(self):
        self._items: dict[str, SpeechItem] = {}

    def push(self, item: SpeechItem) -> None:
        existing = self._items.get(item.key)
        if existing is None or item.priority > existing.priority:
            self._items[item.key] = item

    def pop(self) -> Optional[SpeechItem]:
        if not self._items:
            return None
        key = max(self._items, key=lambda k: self._items[k].priority)
        return self._items.pop(key)

    def __len__(self) -> int:
        return len(self._items)
