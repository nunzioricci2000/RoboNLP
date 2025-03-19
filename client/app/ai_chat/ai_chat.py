import os
from typing import List
from openai import OpenAI
from openai.types.chat import ChatCompletion, ChatCompletionMessageParam


class AIChat:
    "A class to interact with the OpenAI API"
    "for chat completions. It can be used to generate"
    "chat completions for a given prompt."
    def __init__(self, system_prompt: str):
        self.messages: List[ChatCompletionMessageParam] = [
                {
                    "role": "system",
                    "content": system_prompt,
                },
            ]
        self.client: OpenAI = OpenAI(
                api_key=os.getenv("OPENAI_API_KEY"),
                base_url=os.getenv("OPENAI_BASE_URL"),
            )
    
    def write_message(self, message: str):
        self.messages.append({
            "role": "user",
            "content": message,
        })

    def generate_response(self) -> ChatCompletion:
        response = self.client.chat.completions.create(
            model=os.getenv("OPENAI_MODEL"),
            messages=self.messages,
        )
        self.messages.append({
            "role": "system",
            "content": response.choices[0].message.content
        })
        return response
