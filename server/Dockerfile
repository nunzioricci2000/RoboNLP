FROM gcc:14 AS build
WORKDIR /app
COPY . .
RUN chmod +x setup.sh && \
    ./setup.sh --avoid-download && \
    make clean && \
    make && \
    chmod +x bin/app
FROM debian:12
WORKDIR /app
COPY --from=build /app/bin/app /app/bin/app
RUN mkdir -p /user && \
    chmod +rw /user
ENV PORT=1025
EXPOSE 1025
CMD ["./bin/app"]