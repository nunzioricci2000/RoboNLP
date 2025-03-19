from .ai_chat import AIChat
from .prompt_generator import PromptGenerator
from .user_analyzer import UserAnalyzer
from .fact_recorder import FactRecorder
from model.user_profile import UserProfile

class RobotChat(AIChat):
    "A class to generate a chat completion"
    "for a given user."
    def __init__(self, user_info: UserProfile):
        system_prompt = PromptGenerator().generate_prompt(user_info.to_string())
        system_prompt_notes = """NOTE:
1. Mantenieni le risposte brevi al prompt generato, andranno lette a voce da un robot parlante
2. Non includere emoji
3. Usa le espressioni nel seguente array circondandole con il carattere '/' (es.: "Ciao! /Blink/ Come stai?"): ["Blink", "BrowFrown", "BrowRaise", "CloseEyes", "ExpressAnger", "ExpressDisgust","ExpressFear", "ExpressSad", "GazeAway", "Nod", "Oh", "OpenEyes", "Roll", "Shake", "Smile", "Surprise","Thoughtful", "Wink"]
"""
        super().__init__(f"{system_prompt}\n\n{system_prompt_notes}")
        self.system_prompt = f"{system_prompt}\n\n{system_prompt_notes}"
    
    def chat(self, message: str, fact_recorder: FactRecorder = FactRecorder()) -> str:
        self.messages.append({
            "role": "system",
            "content": self.system_prompt,
        })
        self.write_message(message)
        fact = UserAnalyzer().analyze_user(message)
        fact_recorder.record(fact)
        response = self.generate_response()
        print(self.messages)
        return response.choices[0].message.content
        