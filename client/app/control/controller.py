from furhat_manager import FurhatManager
from server_connection import ServerConnection
from ai_chat import RobotChat

class Controller:
    def __init__(
        self, 
        furhat_manager: FurhatManager,
        server_connection: ServerConnection,
        robot_chat: RobotChat
    ):
        self.furhat_manager = furhat_manager
        self.server_connection = server_connection
        self.robot_chat = robot_chat
    
    def run(self):
        self.furhat_manager.set_up()
        self.furhat_manager.login()
        self.furhat_manager.chat()