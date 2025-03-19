from robot_connection import RobotConnection
from typing import List

class TextualConnection(RobotConnection):
    def speak(self, text: str):
        print(f"Robot> {text}")

    def listen(self) -> str:
        return input("Utente> ")
    
    def gestures(self) -> List[str]:
        return ["Blink", "BrowFrown", "BrowRaise", "CloseEyes", "ExpressAnger", "ExpressDisgust",
            "ExpressFear", "ExpressSad", "GazeAway", "Nod", "Oh", "OpenEyes", "Roll", "Shake", "Smile", "Surprise",
            "Thoughtful", "Wink"]
    
    def perform_gesture(self, gesture: str):
        return print(f"/{gesture}/")