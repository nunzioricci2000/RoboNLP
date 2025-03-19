from typing import List, Optional
from dataclasses import dataclass, field

@dataclass
class UserProfile:
    name: Optional[str] = None
    extraversion: Optional[float] = None
    agreeableness: Optional[float] = None
    conscientiousness: Optional[float] = None
    emotional_stability: Optional[float] = None
    openness_to_experience: Optional[float] = None
    facts: Optional[List[str]] = field(default_factory=list)
    
    def to_string(self):
        attributes = vars(self)
        ret = ', '.join(f"{key}: {value}" for key, value in attributes.items() if value)
        return ret
    
    def to_json(self):
        return {key: value for key, value in vars(self).items() if value}
    
if __name__ == "__main__":
    # Test
    profile = UserProfile(name="test", extraversion=3)
    print(profile.to_string())
    print(profile.to_json())
    assert profile.to_string() == "name: test, extraversion: 3"
    assert profile.to_json() == {'name': 'test', 'extraversion': 3}
    print("All tests passed!")