from typing import List, Optional

class UserProfile:
    def __init__(self,
                 name: Optional[str] = None,
                 extraversion: Optional[float] = None,
                 agreeableness: Optional[float] = None,
                 conscientiousness: Optional[float] = None,
                 emotional_stability: Optional[float] = None,
                 openness_to_experience: Optional[float] = None,
                 facts: Optional[List[str]] = None):
        self.name = name
        self.extraversion = extraversion
        self.agreeableness = agreeableness
        self.conscientiousness = conscientiousness
        self.emotional_stability = emotional_stability
        self.openness_to_experience = openness_to_experience
        self.facts = facts
    
    def to_string(self):
        attributes = vars(self)
        ret = ', '.join(f"{key}: {value}" for key, value in attributes.items() if value is not None)
        return ret