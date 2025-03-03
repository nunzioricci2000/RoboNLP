from multiprocessing.pool import AsyncResult
from furhat_remote_api import FurhatRemoteAPI

furhat = FurhatRemoteAPI("host.docker.internal")
voices = furhat.get_voices()
furhat.set_voice(name='Bianca')
furhat.say(text="Ciao a tutti!")

while True:
    thread: AsyncResult = furhat.furhat_listen_get(async_req=True, language="it-IT")
    thread.wait()
    print(thread.get())
