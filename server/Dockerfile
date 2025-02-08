# Utilizziamo un'immagine base che include gli strumenti necessari (es. Ubuntu)
FROM gcc:latest

# Impostiamo la directory di lavoro
WORKDIR /app

# Copiamo l'intero contenuto della directory corrente nel container
COPY . .

# Avviamo il setup e la compilazione dell'applicazione
RUN chmod +x setup.sh && ./setup.sh --avoid-download & make clean && make && chmod +x bin/app

# Esponiamo la porta 1025
EXPOSE 1025

# Avviamo l'app compilata
CMD ["./bin/app"]