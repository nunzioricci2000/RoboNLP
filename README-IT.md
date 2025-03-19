# RoboNLP

NOTE: This file is available in english [here](README.md)!

**RoboNLP** è un progetto sviluppato durante il corso di Laboratorio di Sistemi Operativi al Corso di Laurea Triennale in Informatica all'Università degli Studi di Napoli Federico II.

L'obiettivo del progetto è l'implementazione di un server scritto in C che, facendo uso delle system call, istruisca un client scritto in Python per la gestione del **robot Furhat**.

## Architettura

L'applicativo è diviso in due parti principali:

* un **server**, scritto in C, impiega le chiamate di sistema per la gestione dei thread, socket e gestione dei file e si avvale di due librerie esterne ([picohttpparser](https://github.com/h2o/picohttpparser) e [cJSON](https://github.com/DaveGamble/cJSON)) per il parsing di strutture dati complesse;
* un **client**, scritto in Python, impiega furhat-remote-api per l'interazione con il robot e [openai](https://github.com/openai/openai-python) per l'interpretazione e l'articolazione del linguaggio naturale.

## Descrizione

All'avvio il robot chiederà all'utente di identificarsi. In caso l'utente non risulti registrato gli verrà somministrato un questionario nella forma [TIPI](https://gosling.psy.utexas.edu/scales-weve-developed/ten-item-personality-measure-tipi/) ed i risultati verranno salvati sul server. Ogni dettaglio particolare dell'utente verrà salvato sul server in maniera tale che il robot ne possa avere memoria nella prossima sessione.

Una volta terminata la fase di registrazione il robot entrerà in modalità chatting, interagendo con l'utente cercando di emularne la personalità.

## Installazione

> **NOTA**: i seguenti passaggi sono stati testati su sistema MacOS ma dovrebbero funzionare anche su Linux e Windows tramite WLS.

In primis va eseguito il file di setup del server. Muovendosi nella cartella **server** da terminale eseguire il seguente comando:

```sh
./setup.sh
```

Questo installerà le dipendenze necessarie nella cartella **server/libs**.

In seguito va creta nella root del progetto il file **.env** e inserire all'interno il proprio Token API

### Installazione ambiente di sviluppo client (opzionale)

Aprendo la repository su VSCode, ed installando le opportune estensioni per lo sviluppo python, sarà possibile cercare il file **client/requirements.txt** e cliccare il pulsante in basso a destra "Inizzializza ambiente virtuale". Al termine di questa operazione VSCode riconoscerà le librerie terze e attiverà l'autocompletamento.

## Avvio

Prima di avviare l'applicativo è necessario eseguire la skill delle remote-api sul robot Furhat o sul suo simulatore. Nella versione 2.8.0 del simulatoretale skill è già presente nell'installazione dell'applicativo Furhat SDK disponibile a **~/.furhat/launcher/Plugins/furhat-remote-api.skill**

Per avviare l'applicativo sarà sufficiente eseguire nella root del progetto il seguente comando:

```sh
docker compose up
```
