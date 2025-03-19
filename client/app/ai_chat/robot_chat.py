from .ai_chat import AIChat
from .prompt_generator import PromptGenerator
from .user_analyzer import UserAnalyzer
from .fact_recorder import FactRecorder
from user_profile import UserProfile

class RobotChat(AIChat):
    "A class to generate a chat completion"
    "for a given user."
    def __init__(self, user_info: UserProfile):
        system_prompt = PromptGenerator().generate_prompt(user_info.to_string())
        super().__init__(system_prompt)
    
    def chat(self, message: str, fact_recorder: FactRecorder = FactRecorder()) -> str:
        self.write_message(message)
        fact = UserAnalyzer().analyze_user(message)
        fact_recorder.record(fact)
        response = self.generate_response()
        return response.choices[0].message.content
        