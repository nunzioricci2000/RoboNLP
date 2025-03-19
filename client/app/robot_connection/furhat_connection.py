from multiprocessing.pool import AsyncResult
from furhat_remote_api import FurhatRemoteAPI
from config import furhat_host, furhat_voice
from .robot_connection import RobotConnection
from typing import List

class FurhatConnection(RobotConnection):
    api: FurhatRemoteAPI

    def __init__(self):
        self.api = FurhatRemoteAPI(furhat_host)
        self.api.set_voice(name=furhat_voice)
        self.api.attend(user="CLOSEST")

    def speak(self, text: str):
        self.api.say(text = text, lipsync=True, blocking=True)
        print("Robot: " + text)

    def listen(self) -> str:
        thread: AsyncResult = self.api.furhat_listen_get(async_req=True, language="it-IT")
        thread.wait()
        ret = thread.get().message.lower()
        print("User: " + ret)
        return ret
    
    def gestures(self) -> List[str]:
        return ["Blink", "BrowFrown", "BrowRaise", "CloseEyes", "ExpressAnger", "ExpressDisgust",
            "ExpressFear", "ExpressSad", "GazeAway", "Nod", "Oh", "OpenEyes", "Roll", "Shake", "Smile", "Surprise",
            "Thoughtful", "Wink"]
    
    def perform_gesture(self, gesture: str):
        self.api.gesture(name=gesture)
        print(f"Robot: /{gesture}/")