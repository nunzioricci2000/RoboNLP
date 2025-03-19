# Documentazione
L'obiettivo del progetto è l'implementazione di un server scritto in C che, facendo uso delle system call, istruisca un client scritto in Python per la gestione del **robot Furhat**.

Il robot dovrà:
- somministrare il test della personalità [TIPI](https://gosling.psy.utexas.edu/scales-weve-developed/ten-item-personality-measure-tipi/) ad ogni nuovo utente;
- una volta ottenuti i risultati del test, conversare con l'utente emulandone la personalità individuata.

## Scelte implementative
Le responsabilità generali di client e server sono le seguenti:
- il client, scritto in Python, gestisce l'interazione con il robot Furhat e l'integrazione di OpenAI per l'interpretazione e l'elaborazione del linguaggio naturale;
- il server, scritto in C,  gestisce e conserva la memoria sulle informazioni degli utenti (personalità ed alcuni argomenti di conversazione notevoli).

Client e server interagiscono fra di loro tramite protocollo HTTP scambiandosi oggetti JSON.

#### User Profile
È utile introdurre il concetto generale di User Profile, astraendo i dettagli implementativi, che si differenziano fra client e server.

L'User Profile è l'oggetto che rappresenta i dati utente, che comprendono: 
- un'**username**, che rappresenta l'identificativo univoco per ciascun utente,
- i risultati del test della personalità TIPI, costituiti da 5 valori numerici compresi tra 1 e 7, a rappresentanza dei seguenti tratti della personalità,
  - **extraversion** (estroversione),
  - **agreeableness** (piacevolezza),
  - **conscientiousness** (coscienziosità),
  - **emotional stability** (stabilità emotiva),
  - **openness to experiences** (apertura alle esperienze);
- una collezione di **facts** (aneddoti) per fornire all'AI un contesto sulle conversazioni passate.

#### Richieste e risposte Client-Server
Come già menzionato, server e client comunicano esclusivamente tramite protocollo HTTP. In aggiunta, il body sia delle richieste (da parte del client) sia delle risposte (da parte del server) HTTP è sempre un oggetto JSON. L’oggetto JSON potrebbe contenere:
- nulla ( {} ),
- uno o più campi dell’User File,
- un messaggio di errore, rappresentato da una stringa denominata error (solo nelle risposte server),
- un singolo fact denominato fact da aggiungere allo user file (solo nelle richieste client).

Per quanto riguarda i path degli URL delle richieste HTTP, abbiamo le seguenti possibilità:
- root (`/`), richieste alla root sono interpetate come richieste di conferma di avvenuta connessione, 
- `/user`, è la cartella che contiene tutti i file utente, vi si indirizzano le POST per creare nuovi utenti,
- `/user/<username>` per richieste relative ad un utente esistente,
- `/user/<username>/<fieldname>` per richieste relative ad uno specifico campo dell'User File di un utente esistente.

### Server
Il server è in grado di accogliere e gestire più richieste alla volta. In seguito ad una richiesta il server può:
- fornire conferma di connessione,
- reperire ed inviare i dati di un User File salvato in memoria,
- salvare nuovi User File,
- modificare User File esistenti,
- eliminare User File esistenti.

#### Struttura
Nella cartella server sono contenuti, oltre al `Dockerfile`, un `Makefile`, che gestisce la compilazione del server, ed un il file `setup.sh` gestisce la risoluzione delle dipendenze, scaricandole in `external` e riportando in `libs` i file necessari. In aggiunta a queste due cartelle, è presente la cartella `src` contenente il codice del server. 

Le librerie esterne comprendono:
- [cJSON](https://github.com/DaveGamble/cJSON) per interpretare e costruire oggetti JSON,
- [picohttpparser](https://github.com/h2o/picohttpparser) per interpretare le richieste HTTP e costrure le rispote.

#### Gestione delle connessioni
Le connessioni sono implementate tramite chiamate di sistema per la creazione e gestione delle socket Linux in C. In particolare, l'inazializzazione della socket server è definita nel file `server.c`, la accettazione delle richieste è operata da `main.c`
 la ricezione di richieste e l'invio di risposte tramite socket avviene a vari livelli del codice riportato in `request_handler.c`.

La molteplicità delle connessioni accettate è resa possibile grazie all'utilizzo di thread. I thread vengono creati in `main.c` e gestiti in `request_handler.c`.



#### Gestione dei salvataggi
Tutte le operazioni su file sono contenunte in `file_operations.c`. 

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