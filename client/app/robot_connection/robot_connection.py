from abc import ABC, abstractmethod

class RobotConnection(ABC):
    @abstractmethod
    def speak(self, text: str):
        pass

    @abstractmethod
    def listen(self) -> str:
        pass