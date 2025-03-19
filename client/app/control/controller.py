from robot_connection import RobotConnection
from server_connection import ServerConnection
from ai_chat import RobotChat
from utils.string2number import str2int
from model.user_profile import UserProfile

class Controller:
    def __init__(
        self, 
        robot: RobotConnection,
        server: ServerConnection,
    ):
        self.robot = robot
        self.server = server
        self.chat: RobotChat = None
    
    def get_quiz_answer(self, question):
        self.robot.speak(text = question)
        while True:
            answer = self.robot.listen()
            if str2int(answer) is not None:
                break
            self.robot.speak("Non ho capito. Per favore rispondi con un numero intero da uno a sette.")
        return str2int(answer)
    
    def build_new_user(self, username):
        self.robot.speak("Benvenuto!")
        self.robot.speak("Ti sottoporrò un semplice quiz.")
        self.robot.speak("Per ciascuna delle seguenti affermazioni, esprimi il tuo accordo o disaccordo in una scala da uno a sette.")
        self.robot.speak("Uno significa: completamente in disaccordo. Quattro significa: indifferente. Sette significa: completamente d'accordo.")
        self.robot.speak("Cominciamo.")

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

        self.robot.speak("Grazie mille per l'attenzione! Ora possiamo andare avanti.")

        return UserProfile(extraversion=extraversion, agreeableness=agreeableness, conscientiousness=conscientiousness, 
                           openness_to_experience=openness_to_experience, emotional_stability=emotional_stability, name=username)

    def login(self):
        self.robot.speak(text="Ciao! Sono Robo NLP. Qual è il tuo username?")
        
        wait_cycle = 0
        while True:
            username = self.robot.listen()

            if username == "":
                wait_cycle = wait_cycle + 1
                if wait_cycle >= 3:
                    self.robot.speak(text = "Non ho sentito. Qual è il tuo username?")
                    wait_cycle = 0

                continue
            self.robot.speak(text = username + ". Confermi questo username?")

            heard = self.robot.listen()

            if heard in ["si", "sì", "esatto", "confermo", "conferma", "corretto"]:
                break
            self.robot.speak(text="Ripeti il tuo username, per favore.")

        self.robot.speak(text="Ciao, " + username + "!")

        server = ServerConnection()

        server_response = server.get_user_profile(username = username)
        if server_response.error_message is not None:
            new_user = self.build_new_user(username = username)
            server.post_user_profile(username=username, profile=new_user)
            self.robot.speak("Piacere di conoscerti " + new_user.name + "!")
            self.user = new_user
        elif server_response.user_profile is not None:
            returning_user = server_response.user_profile
            self.robot.speak("Bentornato " + returning_user.name + "!")
            self.user = returning_user

    def act_out(self, text: str):
        split_text = text.split("/")
        for i in range(len(split_text)):
            if i % 2 == 0:
                self.robot.speak(split_text[i])
            else:
                if split_text[i] in self.robot.gestures():
                    self.robot.perform_gesture(split_text[i])

    def start_chatting(self):
        robot_chat = RobotChat(user_info=self.user)
        while(True):
            user_speech = self.robot.listen()
            robot_answer = robot_chat.chat(message=user_speech)
            self.act_out(text=robot_answer)
