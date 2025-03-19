# RoboNLP - Documentazione Tecnica

## 1. Panoramica del Progetto

RoboNLP è un sistema sviluppato durante il corso di Laboratorio di Sistemi Operativi presso l'Università degli Studi di Napoli Federico II. Il progetto implementa un'architettura client-server per gestire un robot Furhat, consentendo interazioni conversazionali personalizzate basate sulla personalità dell'utente.

Il robot esegue due funzioni principali:

- Somministrare il test della personalità TIPI (Ten-Item Personality Inventory) a nuovi utenti
- Conversare con l'utente adattando il proprio comportamento in base al profilo di personalità rilevato

## 2. Architettura del Sistema

### 2.1 Componenti Principali

L'applicativo è costituito da due componenti fondamentali:

1. **Server (C)**
   - Gestisce e conserva i dati degli utenti (profili di personalità e argomenti di conversazione)
   - Utilizza system calls per thread, socket e operazioni su file
   - Si avvale di librerie esterne per il parsing HTTP e JSON

2. **Client (Python)**
   - Gestisce l'interazione con il robot Furhat
   - Integra OpenAI per l'interpretazione e l'elaborazione del linguaggio naturale
   - Comunica con il server tramite richieste HTTP

### 2.2 Comunicazione

La comunicazione tra client e server avviene tramite protocollo HTTP con scambio di oggetti JSON. Il formato standardizzato consente un'interazione semplice e portabile tra i componenti.

### 2.3 Diagramma di Architettura

```text
┌─────────────┐        HTTP/JSON       ┌─────────────┐          ┌─────────────┐
│             ├───────────────────────▶│             │          │             │
│   Client    │◀───────────────────────┤    Server   │          │ Robot Furhat│
│  (Python)   │                        │     (C)     │          │             │
└─────┬───────┘                        └─────────────┘          └─────▲───────┘
      │                                                               │
      │               Furhat Remote API                               │
      └───────────────────────────────────────────────────────────────┘
```

## 3. Implementazione del Server

### 3.1 Struttura del Server

Il server è organizzato nelle seguenti directory:

- `src/`: Contiene il codice sorgente C
- `libs/`: Librerie esterne necessarie
- `external/`: Repository esterni completi
- `Makefile`: Gestisce la compilazione del server
- setup.sh: Script per l'installazione delle dipendenze
- `Dockerfile`: Configurazione per la containerizzazione

### 3.2 Dipendenze Esterne

Il server utilizza due librerie esterne principali:

- **cJSON**: Per interpretare e costruire oggetti JSON
- **picohttpparser**: Per analizzare richieste HTTP e costruire risposte

### 3.3 Gestione delle Connessioni

Le connessioni sono implementate tramite socket Linux in C:

- L'inizializzazione della socket server è definita in `server.c`
- L'accettazione delle richieste è gestita in `main.c`
- La ricezione delle richieste e l'invio delle risposte avviene in `request_handler.c`

Il server è multi-thread, consentendo la gestione simultanea di più connessioni:

- I thread vengono creati in `main.c`
- La loro gestione avviene in `request_handler.c`

### 3.4 Gestione dei Salvataggi

Le operazioni su file sono centralizzate in `file_operations.c`, che si occupa di:

- Salvare i profili utente in formato JSON
- Recuperare i profili esistenti
- Modificare profili esistenti
- Eliminare profili

### 3.5 API Endpoints

Il server espone i seguenti endpoint HTTP:

- `/user` (POST): Crea un nuovo utente
- `/user/<username>` (GET): Ottiene il profilo di un utente esistente
- `/user/<username>` (DELETE): Elimina un utente esistente
- `/user/<username>` (PUT): Aggiorna il profilo di un utente esistente
- `/user/<username>/<fieldname>` (GET): Ottiene un campo specifico del profilo utente
- `/user/<username>/facts` (POST): Aggiunge dati al campo facts

## 4. Implementazione del Client

### 4.1 Struttura del Client

Il client è organizzato secondo un paradigma ad oggetti con le seguenti responsabilità:

- **Gestione dell'interfaccia**: Affidata alla classe `FurhatConnection`
- **Comunicazione con il server**: Gestita dalla classe `ServerConnection`
- **Integrazione con OpenAI**: Per l'elaborazione del linguaggio naturale

### 4.2 ServerConnection

La classe `ServerConnection` implementa il pattern Singleton e si occupa di:

- Gestire le richieste HTTP verso il server
- Convertire dati in formato JSON
- Gestire username e dati utente

Restituisce oggetti `ServerResponse` che possono contenere:

- Un profilo utente
- Un messaggio di errore
- Un messaggio di conferma

### 4.3 UserProfile

La classe `UserProfile` implementa il pattern Factory per fornire costruttori specializzati che inizializzano solo gli attributi necessari in base al tipo di richiesta.

Contiene i seguenti campi:

- `name`: Nome utente
- `extraversion`: Punteggio di estroversione
- `agreeableness`: Punteggio di gradevolezza
- `conscientiousness`: Punteggio di coscienziosità
- `emotional_stability`: Punteggio di stabilità emotiva
- `openness_to_experience`: Punteggio di apertura all'esperienza
- `facts`: Lista di argomenti di conversazione rilevanti

### 4.4 FurhatConnection

La classe `FurhatConnection` gestisce l'interazione con il robot Furhat, occupandosi di:

- Inizializzare la connessione con il robot
- Gestire espressioni facciali e movimenti
- Controllare il dialogo e la sintesi vocale

## 5. Installazione e Configurazione

### 5.1 Prerequisiti

- Docker e Docker Compose
- Git
- Connessione a internet per scaricare le dipendenze
- Robot Furhat o simulatore Furhat SDK (versione 2.8.0 o successiva)

### 5.2 Installazione del Server

1. Clonare il repository:

   ```sh
   git clone <repository-url> ### TODO: SETTARE URL!!
   cd repo
   ```

2. Eseguire lo script di setup:

   ```sh
   cd server
   chmod +x setup.sh
   ./setup.sh
   ```

   Questo script:
   - Aggiorna i submodule Git
   - Copia i file necessari dalle dipendenze esterne nella cartella `libs`

### 5.3 Installazione del Client (Ambiente di sviluppo)

Per configurare l'ambiente di sviluppo del client:

1. Aprire la repository in VSCode
2. Installare le estensioni per Python
3. Cercare il file requirements.txt e cliccare "Inizializza ambiente virtuale"
4. Al termine, VSCode riconoscerà le librerie e attiverà l'autocompletamento

### 5.4 Configurazione

Il client richiede le seguenti variabili d'ambiente nel file .env:

- `OPENAI_API_KEY`: Chiave API per OpenAI
- `OPENAI_BASE_URL`: URL base per le API OpenAI (opzionale)
- `OPENAI_MODEL`: Modello OpenAI da utilizzare (opzionale)
- `SERVER_IP`: IP del server (default: 'localhost')
- `SERVER_PORT`: Porta del server (default: 1025)
- `FURHAT_HOST`: Indirizzo del robot Furhat (default: 'localhost')
- `FURHAT_VOICE`: Voce da utilizzare per il robot (opzionale)

## 6. Avvio dell'Applicazione

### 6.1 Preparazione del Robot Furhat

Prima di avviare l'applicazione, è necessario eseguire la skill remote-api sul robot Furhat o sul suo simulatore:

1. Assicurarsi che la skill sia disponibile a `~/.furhat/launcher/Plugins/furhat-remote-api.skill`
2. Avviare il robot o il simulatore
3. Attivare la skill remote-api

### 6.2 Avvio con Docker Compose

Per avviare l'intero sistema utilizzando Docker:

```sh
docker compose up
```

Questo comando:

1. Costruisce l'immagine Docker del server
2. Costruisce l'immagine Docker del client
3. Avvia entrambi i container
4. Collega i container alla stessa rete

### 6.3 Avvio Manuale (Sviluppo)

Per avviare i componenti separatamente durante lo sviluppo:

1. Avvio del server:

   ```sh
   cd server
   make clean && make
   ./bin/app
   ```

2. Avvio del client:

   ```sh
   cd client
   python -m app
   ```

## 7. Casi d'Uso

### 7.1 Nuovo Utente

1. Il robot accoglie l'utente e chiede il nome
2. Il client invia una richiesta GET al server per verificare se l'utente esiste
3. Se l'utente non esiste, il robot somministra il test TIPI
4. Il client elabora le risposte e crea un profilo di personalità
5. Il profilo viene inviato al server tramite una richiesta POST a `/user`
6. Il server salva il profilo e risponde con una conferma
7. Il robot inizia la conversazione adattandosi alla personalità dell'utente

### 7.2 Utente Esistente

1. Il robot accoglie l'utente e chiede il nome
2. Il client invia una richiesta GET al server per verificare se l'utente esiste
3. Il server risponde con il profilo dell'utente
4. Il client configura il comportamento del robot in base al profilo
5. Il robot inizia la conversazione personalizzata
6. Durante la conversazione, nuovi fatti rilevanti vengono inviati al server tramite POST a `/user/<username>/facts`

## 8. Sicurezza e Limitazioni

### 8.1 Sicurezza

- Il sistema non implementa autenticazione
- I dati degli utenti sono memorizzati in chiaro
- Il sistema è progettato per uso didattico/dimostrativo e non per ambienti di produzione

### 8.2 Limitazioni Note

- Il server gestisce un numero limitato di connessioni simultanee
- Non c'è persistenza dei dati tra riavvii del container Docker
- L'integrazione con OpenAI richiede una connessione internet

## 9. Conclusioni

RoboNLP dimostra l'integrazione di diverse tecnologie per creare un'esperienza interattiva personalizzata con un robot umanoide. L'architettura client-server consente una separazione di responsabilità, dove il server C si occupa dell'archiviazione e gestione dei dati, mentre il client Python gestisce l'interazione con il robot e l'elaborazione del linguaggio naturale.

Il progetto mette in pratica diversi concetti di programmazione di sistema operativo, tra cui:

- Gestione dei thread
- Comunicazione tramite socket
- Gestione dei file
- Serializzazione e deserializzazione di dati complessi

Inoltre, il sistema è facilmente estendibile per supportare nuove funzionalità e comportamenti del robot.

## 10. Riferimenti

- [cJSON](https://github.com/DaveGamble/cJSON)
- [picohttpparser](https://github.com/h2o/picohttpparser)
- [OpenAI Python](https://github.com/openai/openai-python)
- [Ten-Item Personality Measure (TIPI)](https://gosling.psy.utexas.edu/scales-weve-developed/ten-item-personality-measure-tipi/)
- [Furhat Robot](https://furhatrobotics.com/)
