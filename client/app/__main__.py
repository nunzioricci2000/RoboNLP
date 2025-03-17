from multiprocessing.pool import AsyncResult
from furhat_remote_api import FurhatRemoteAPI

def speak(text):
    furhat.say(text = text, lipsync=True, blocking=True)

def listen():
    thread: AsyncResult = furhat.furhat_listen_get(async_req=True, language="it-IT")
    thread.wait()
    return thread.get().message.lower()


# furhat = FurhatRemoteAPI("host.docker.internal")
furhat = FurhatRemoteAPI("localhost")
#voices = furhat.get_voices()
furhat.set_voice(name='Bianca')
speak(text="Salve! Sono RoboNLP. Qual è il suo username?")

furhat.attend(user="CLOSEST")

while True:
    username = listen()

    print(username)
    speak(text = username + ". Confermi questo username?")

    heard = listen()

    print(heard)
    if heard in ["si", "sì", "esatto", "confermo", "conferma", "corretto"]:
        break
    speak(text="Ripeti il tuo username, per favore.")
speak(text="Ciao, " + username + "!")




while True:
    thread: AsyncResult = furhat.furhat_listen_get(async_req=True, language="it-IT")
    thread.wait()
    print(thread.get())
