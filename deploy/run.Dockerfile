FROM debian:bullseye

RUN apt update -y && apt upgrade -y && apt install -y build-essential

RUN useradd -m pwn

ADD https://yx7.cc/code/ynetd/ynetd-0.1.2.tar.xz /ynetd.tar.xz
RUN tar -xf /ynetd.tar.xz && make -C /ynetd-0.1.2/

WORKDIR /home/pwn
COPY names.txt .
COPY newspaper.txt .
COPY get_flag /bin/get_flag
COPY vuln .

RUN chown pwn /bin/get_flag && chmod 500 /bin/get_flag
RUN chmod 0755 /home/pwn/vuln

EXPOSE 1337
CMD ["/ynetd-0.1.2/ynetd", "-p", "1337", "-u", "pwn", "-d", "/home/pwn", "./vuln"]
