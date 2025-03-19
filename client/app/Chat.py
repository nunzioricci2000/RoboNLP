from openai import OpenAI
from openai.types.chat import ChatCompletion, ChatCompletionMessageParam
import os
from typing import List
from UserProfile import UserProfile

class FactRecorder:
    def record(self, fact: str):
        # This is an abstract method that should be implemented by concrete classes
        # to define how facts about users are recorded
        pass

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

class PromptGenerator(AIChat):
    "A class to generate a prompt for a chatbot"
    "based on a user profile."
    def __init__(self):
        super().__init__(
            "Sei un generatore di prompt di sistema per chatbot personalizzati. Il tuo compito è ricevere in input un JSON contenente informazioni specifiche sull'utente, con la seguente struttura:\n"
            "```json\n"
            "{\n"
            "  \"name\": \"Biagio\",\n"
            "  \"extrovert\": \"7/7\",\n"
            "  \"argumantative\": \"7/7\",\n"
            "  \"facts\": [\"L'utente ama il colore rosso\", \"l'utente è tifoso del Napoli\"]\n"
            "}```\n"
            "\n"
            "Utilizzando queste informazioni, genera un prompt di sistema che istruise un assistente conversazionale a imitare la personalità e gli interessi dell'utente. Il prompt di sistema generato deve includere:\n"
            "\n"
            "Personalizzazione del Tono e dello Stile:\n"
            "Se il livello di 'extrovert' è alto, il chatbot deve utilizzare un tono energico, coinvolgente e amichevole.\n"
            "Se il livello di 'argumantative' è alto, il chatbot dovrà adottare un atteggiamento deciso e, se necessario, anche leggermente provocatorio o difensivo.\n"
            "Integrazione dei Fatti:\n"
            "Integra in modo naturale i fatti forniti (ad esempio: 'l'utente ama il colore rosso' e 'l'utente è tifoso del Napoli') all'interno delle risposte, creando riferimenti contestuali e personalizzati.\n"
            "Coerenza e Adattabilità:\n"
            "Il prompt deve specificare che ogni risposta del chatbot deve risultare coerente con i dati forniti e adattarsi dinamicamente alle informazioni contenute nel JSON.\n"
            "Il prompt di sistema generato deve essere dettagliato e strutturato, in modo che il chatbot che lo utilizzerà possa offrire un'interazione altamente personalizzata e autentica, rispecchiando la personalità e gli interessi specifici dell'utente.\n"
            "\n"
            "Restituisci il prompt di sistema in un formato pronto all'uso, senza ulteriori spiegazioni.\n"
        )
    
    def generate_prompt(self, message: str) -> str:
        self.write_message(message)
        response = self.generate_response()
        return response.choices[0].message.content

class UserAnalyzer(AIChat):
    "A class to generate facts about a user"
    "based on a given prompt."
    "The prompt should be a message that the user"
    "would say to the chatbot."
    def __init__(self):
        super().__init__(
            "Sei un analizzatore di messaggi progettato per estrarre informazioni utili sull'utente.\n"
            "Ricevi in input una serie di messaggi inviati dall'utente e restituisci una breve informazione significativa che possa descrivere i suoi interessi, personalità o fatti rilevanti.\n"
            "Se il messaggio non contiene informazioni interessanti o nuove, restituisci una stringa vuota.\n"
            "Non ripetere informazioni già note e non aggiungere interpretazioni personali.\n"
            "\n"
            "Esempi di input e output:\n"
            "\n"
            "Input: 'Oggi ho comprato una maglia del Napoli'\n"
            "Output: 'L'utente è tifoso del Napoli'\n"
            "\n"
            "Input: 'Ho studiato tutto il giorno, che noia'\n"
            "Output: '' (stringa vuota, nessuna informazione rilevante)\n"
            "\n"
            "Input: 'Adoro la musica jazz, specialmente Miles Davis'\n"
            "Output: 'L'utente ama la musica jazz, in particolare Miles Davis'\n"
            "\n"
            "Input: 'Boh'\n"
            "Output: '' (stringa vuota, nessuna informazione rilevante)\n"
            "\n"
            "Rispondi solo con l'informazione estratta o una stringa vuota, senza ulteriori spiegazioni."
        )

    def analyze_user(self, message: str) -> str:
        self.write_message(message)
        response = self.generate_response()
        return response.choices[0].message.content

class RobotChat(AIChat):
    "A class to generate a chat completion"
    "for a given user."
    def __init__(self, user_info: UserProfile):
        system_prompt = PromptGenerator().generate_prompt(str(user_info))
        super().__init__(system_prompt)
    
    def chat(self, message: str, fact_recorder: FactRecorder = FactRecorder()) -> str:
        self.write_message(message)
        fact = UserAnalyzer().analyze_user(message)
        fact_recorder.record(fact)
        response = self.generate_response()
        return response.choices[0].message.content
        