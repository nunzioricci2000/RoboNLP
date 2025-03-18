from multiprocessing.pool import AsyncResult
from furhat_remote_api import FurhatRemoteAPI
from ServerConnection import ServerConnection


furhat = Furhat()
server = ServerConnection()

furhat.set_up()
furhat.login(server=server)

while True:
    thread: AsyncResult = furhat.furhat_listen_get(async_req=True, language="it-IT")
    thread.wait()
    print(thread.get())