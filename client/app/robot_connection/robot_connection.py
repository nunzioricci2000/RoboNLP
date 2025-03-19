from abc import ABC, abstractmethod
from typing import List

class RobotConnection(ABC):
    @abstractmethod
    def speak(self, text: str):
        pass

    @abstractmethod
    def listen(self) -> str:
        pass

    @abstractmethod
    def gestures(self) -> List[str]:
        pass

    @abstractmethod
    def perform_gesture(self, gesture: str):
        pass