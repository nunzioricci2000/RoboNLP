from typing import List, Optional
from dataclasses import dataclass
from user_profile import UserProfile

@dataclass
class ServerResponse:
    name: Optional[str] = None
    extraversion: Optional[float] = None
    agreeableness: Optional[float] = None
    conscientiousness: Optional[float] = None
    emotional_stability: Optional[float] = None
    openness_to_experience: Optional[float] = None
    facts: Optional[List[str]] = None
    success_message: Optional[str] = None
    error_message: Optional[str] = None

    @property
    def user_profile(self) -> Optional[UserProfile]:
        if any([
            self.name, 
            self.extraversion,
            self.agreeableness, 
            self.conscientiousness, 
            self.emotional_stability, 
            self.openness_to_experience, 
            self.facts
        ]):
            return UserProfile(
                name=self.name,
                extraversion=self.extraversion,
                agreeableness=self.agreeableness,
                conscientiousness=self.conscientiousness,
                emotional_stability=self.emotional_stability,
                openness_to_experience=self.openness_to_experience,
                facts=self.facts
            )
        else:
            return None

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
    
    def to_string(self) -> str:
        ret: List[str] = []
        if self.user_profile is not None:
            ret.append(f"{ret}{self.user_profile.to_string()}")
        if self.success_message is not None:
            ret.append(f"{ret}Success message: {self.success_message}\n")
        if self.error_message is not None:
            ret.append(f"{ret}Error message: {self.error_message}\n")
        if ret == []:
            return "Empty ServerResponse object"
        return "\n".join(ret)
    

if __name__ == "__main__":
    print(ServerResponse.success("Success message").to_string())
    print(ServerResponse.success("Success message").__str__())