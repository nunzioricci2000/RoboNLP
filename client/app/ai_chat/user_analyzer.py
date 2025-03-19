from .ai_chat import AIChat

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
