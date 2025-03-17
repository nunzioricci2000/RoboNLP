from typing import List, Optional
from UserProfile import UserProfile

class ServerResponse:
    def __init__(self,
                 name: Optional[str] = None,
                 extraversion: Optional[float] = None,
                 agreeableness: Optional[float] = None,
                 conscientiousness: Optional[float] = None,
                 emotional_stability: Optional[float] = None,
                 openness_to_experience: Optional[float] = None,
                 facts: Optional[List[str]] = None,
                 success_message: Optional[str] = None,
                 error_message: Optional[str] = None):
        if any([name, extraversion, agreeableness, conscientiousness, emotional_stability, 
                openness_to_experience, facts]):
            self.user_profile = UserProfile(
                name=name,
                extraversion=extraversion,
                agreeableness=agreeableness,
                conscientiousness=conscientiousness,
                emotional_stability=emotional_stability,
                openness_to_experience=openness_to_experience,
                facts=facts
            )
        if success_message:
            self.success_message = success_message
        if error_message:
            self.error_message = error_message

    @classmethod
    def from_profile(cls, profile: dict):
        return cls(
            name=profile.get('name'),
            extraversion=profile.get('extraversion'),
            agreeableness=profile.get('agreeableness'),
            conscientiousness=profile.get('conscientiousness'),
            emotional_stability=profile.get('emotional_stability'),
            openness_to_experience=profile.get('openness_to_experience'),
            facts=profile.get('facts')
        )

    @classmethod
    def success(cls, message: str):
        return cls(success_message=message)

    @classmethod
    def error(cls, message: str):
        return cls(error_message=message)
    
    def to_string(self):
        ret = ''
        if hasattr(self, 'user_profile'):
            ret = ret + self.user_profile.to_string() + "\n"
        if hasattr(self, 'success_message'):
            ret = ret + "Success message: " + self.success_message + "\n"
        if hasattr(self, 'error_message'):
            ret = ret + "Error message: " + self.error_message + "\n"
        if ret == '':
            return "Empty ServerResponse object"
        return ret