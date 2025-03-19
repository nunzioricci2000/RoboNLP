from multiprocessing.pool import AsyncResult
from furhat_remote_api import FurhatRemoteAPI
from ServerConnection import ServerConnection
from Furhat import Furhat


furhat = Furhat()
furhat.set_up()

furhat.login()

furhat.chat()