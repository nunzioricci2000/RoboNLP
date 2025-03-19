from control import Controller
from robot_connection import FurhatConnection
from server_connection import ServerConnection

controller = Controller(
    robot=FurhatConnection(),
    server=ServerConnection(),
    )

controller.login()
controller.chat()