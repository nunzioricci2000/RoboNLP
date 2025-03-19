from robot_connection import RobotConnection


class TextualConnection(RobotConnection):
    def speak(self, text: str):
        print(f"Robot> {text}")

    def listen(self) -> str:
        return input("Utente> ")