# Documentazione
L'obiettivo del progetto è l'implementazione di un server scritto in C che, facendo uso delle system call, istruisca un client scritto in Python per la gestione del **robot Furhat**.

Il robot dovrà:
- somministrare il test della personalità [TIPI](https://gosling.psy.utexas.edu/scales-weve-developed/ten-item-personality-measure-tipi/) ad ogni nuovo utente;
- una volta ottenuti i risultati del test, conversare con l'utente emulandone la personalità individuata.

## Scelte implementative
Le responsabilità generali di server è client sono le seguenti:
- il client, scritto in Python, gestisce l'interazione con il robot Furhat e l'integrazione di OpenAI per l'interpretazione e l'elaborazione del linguaggio naturale;
- il server gestisce e conserva la memoria sulle informazioni degli utenti (personalità ed alcuni argomenti di conversazione notevoli).

Client e server interagiscono fra di loro tramite protocollo HTTP scambiandosi oggetto JSON.

#### User Profile
È utile introdurre il concetto generale di User Profile, astraendo i dettagli implementativi che si differenziano fra client e server.

### Server
Il server è in grado di gestire più richieste, generando un thread per ogni client che richiede ed ottiene una connessione 

Come già menzionato, server e client comunicano esclusivamente tramite protocollo HTTP. In aggiunta, il body sia delle richieste (da parte del client) sia delle risposte (da parte del server) http è sempre un oggetto json. L’oggetto json potrebbe contenere:
- nulla ({})
- Uno o più campi dell’user file (LINKA la definizione dell’ user file)
- Un messaggio di errore, rappresentato da una stringa denominata error (solo nelle risposte server)
- Un messaggio di stato, rappresentato da una stringa denominata NON MI RICORDO (solo nelle risposte server)
- Un singolo fact denominato fact da aggiungere allo user file (solo nelle richieste client)

[Gestione degli end point?]
[Gestione dei file?]
[Costruzione delle risposte?]

- [Compilazione gestita con un makefile]
- [Risoluzione delle dipendenza gestita dal file setup.sh]
#### Struttura
- descrizione generale delle cartelle (src, external, libs)
- setup.sh scarica in external le dipendenze e riporta in libs i file necessari
#### Gestione delle connessioni
#### Gestione dei salvataggi

### Client
Il client è strutturato seguendo un paradigma ad oggetti. Possiamo individuare le seguenti macro responsabilità:

- Gestione del l’interfaccia, affidata alla classe furhat
- Gestione della comunicazione Server affidata alla classe ServerConnection
- Gestione dell’integrazione con OpenAI (NON LO SO ANCORA AHH)


Server Connection
Applica il design pattern _singleton_ 

Gestisce la conversione in json, ricevendo username in forma di stringa, e dati utente come oggetti di UserFile

Restituisce ServerResponse, oggetti che possono contenere un profile (User File), un error_message (str) o un success message (PENSO NON RICORDO) (str)

User file fa uso del design pattern factory per fornire costruttori che inizializzino solo gli attributi necessari a seconda della richiesta.

Furhat 
È la classe che gestisce l’interfaccia e la connessione con il robot. 
Trattandosi di un robot sociale, la parola interfaccia assume, in questo contesto, una forma particolare rispetto all’immaginario comune:
## Installazione
## Configurazione