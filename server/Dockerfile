FROM gcc:14
WORKDIR /app
COPY . .
RUN chmod +x setup.sh && \
    ./setup.sh --avoid-download && \
    make clean && \
    make && \
    chmod +x bin/app
ENV PORT=1025
EXPOSE 1025
CMD ["./bin/app"]