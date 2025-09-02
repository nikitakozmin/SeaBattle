FROM gcc:15.2.0

WORKDIR /usr/games/seabattle/bin/src

COPY src .

RUN make -j

WORKDIR /usr/games/seabattle/bin

RUN mv src/sbtl .

CMD ["./sbtl"]
