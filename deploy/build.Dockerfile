FROM debian:bullseye

RUN apt update && apt install -y --no-install-recommends \
    clang build-essential \
    gdb

WORKDIR /build

COPY vuln.c .

RUN clang -O0 -fstack-protector-strong -fsanitize=safe-stack -no-pie -Wl,-z,relro,-z,now vuln.c -o vuln
