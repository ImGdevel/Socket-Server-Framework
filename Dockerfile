FROM ubuntu:rolling

RUN apt-get update && apt-get install -y \
    libprotobuf-dev libxml2 libmysqlclient-dev libhiredis-dev \
    && rm -rf /var/lib/apt/lists/*

COPY build/server /app/server
RUN chmod +x /app/server

CMD ["/app/server"]