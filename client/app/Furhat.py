from multiprocessing.pool import AsyncResult
from furhat_remote_api import FurhatRemoteAPI
from ServerConnection import ServerConnection
from UserProfile import UserProfile
from Chat import RobotChat


CONFIRMATION_WORDS = ["si", "sì", "esatto", "confermo", "conferma", "corretto"]
NUMBERS = ["uno", "due", "tre", "quattro", "cinque", "sei", "sette", "1", "2", "3", "4", "5", "6", "7"]

class Furhat:
    api: FurhatRemoteAPI = None

    def set_up(self, host: str = "host.docker.internal"):
        self.api = FurhatRemoteAPI(host)
        self.api.set_voice(name='Bianca')
        self.api.attend(user="CLOSEST")

    def speak(self, text):
        self.api.say(text = text, lipsync=True, blocking=True)

    def listen(self):
        thread: AsyncResult = self.api.furhat_listen_get(async_req=True, language="it-IT")
        thread.wait()
        return thread.get().message.lower()
    
    def get_quiz_answer(self, question):
        self.speak(text = question)
        while True:
            answer = self.listen()
            print(answer)
            if answer in NUMBERS:
                break

            self.speak("Non ho capito. Per favore rispondi con un numero intero da uno a sette.")
        if answer in ("1", "uno"):
            return 1
        if answer in ("2", "due"):
            return 2
        if answer in ("3", "tre"):
            return 3
        if answer in ("4", "quattro"):
            return 4
        if answer in ("5", "cinque"):
            return 5
        if answer in ("6", "sei"):
            return 6
        if answer in ("7", "sette"):
            return 7
    
    def build_new_user(self, username):
        self.speak("Benvenuto!")
        self.speak("Ti sottoporrò un semplice quiz.")
        self.speak("Per ciascuna delle seguenti affermazioni, esprimi il tuo accordo o disaccordo in una scala da uno a sette.")
        self.speak("Uno significa: completamente in disaccordo. Quattro significa: indifferente. Sette significa: completamente d'accordo.")
        self.speak("Cominciamo.")

        extraversion = self.get_quiz_answer("Sei una persona estroversa ed esuberante.")
        agreeableness = 8 - self.get_quiz_answer("Sei una persona polemica e litigiosa.")
        conscientiousness = self.get_quiz_answer("Sei un persona affidabile ed auto disciplinata.")
        emotional_stability = 8 - self.get_quiz_answer("Sei una persona ansiosa, che si agita facilmente.")
        openness_to_experience =  self.get_quiz_answer("Sei una persona aperta alle nuove esperienze e con molti interessi.")
        extraversion = (extraversion + 8 - self.get_quiz_answer("Sei una persona riservata e silenziosa."))/2
        agreeableness = (agreeableness + self.get_quiz_answer("Sei una persona comprensiva ed affettuosa."))/2
        conscientiousness = (conscientiousness + 8 - self.get_quiz_answer("Sei una persona disorganizzata e distratta"))/2
        emotional_stability = (emotional_stability + self.get_quiz_answer("Sei una persona tranquilla ed emotivamente stabile"))/2
        openness_to_experience = (openness_to_experience + 8 - self.get_quiz_answer("Sei una persona tradizionalista ed abitudinaria"))/2

        self.speak("Grazie mille per l'attenzione! Ora possiamo andare avanti.")

        return UserProfile(extraversion=extraversion, agreeableness=agreeableness, conscientiousness=conscientiousness, 
                           openness_to_experience=openness_to_experience, emotional_stability=emotional_stability, name=username)



    def login(self):
        self.speak(text="Ciao! Sono Robo NLP. Qual è il tuo username?")
        
        wait_cycle = 0
        while True:
            username = self.listen()

            print(username)
            if username == "":
                wait_cycle = wait_cycle + 1
                if wait_cycle >= 3:
                    self.speak(text = "Non ho sentito. Qual è il tuo username?")
                    wait_cycle = 0

                continue
            self.speak(text = username + ". Confermi questo username?")

            heard = self.listen()

            print(heard)
            if heard in CONFIRMATION_WORDS:
                break
            self.speak(text="Ripeti il tuo username, per favore.")

        self.speak(text="Ciao, " + username + "!")

        server = ServerConnection()

        server_response = server.get_user_profile(username = username)
        if hasattr(server_response, "error_message"):
            new_user = self.build_new_user(username = username)
            server.post_user_profile(username=username, profile=new_user)

            self.speak("Piacere di conoscerti " + new_user.name + "!")
            self.user = new_user
        elif hasattr(server_response, "user_profile"):
            returning_user = server_response.user_profile
            self.speak("Bentornato " + returning_user.name + "!")
            self.user = returning_user
        
    def chat(self):
        robot_chat = RobotChat(user_info=self.user)
        while(True):
            user_speech = self.listen()
            robot_answer = robot_chat.chat(message=user_speech)
            self.speak(text=robot_answer)
                        
        



if __name__ == "__main__":
    furhat = Furhat()
    furhat.set_up(host="localhost")

    furhat.login()

    while True:
        spech = furhat.listen()
        print(print.speech)
