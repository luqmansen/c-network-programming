FROM alpine:3.12.0 as build

WORKDIR /home/

RUN apk add --no-cache gcc musl-dev

COPY server.c .

RUN gcc server.c -o server.o -static


FROM alpine:3.12.0

WORKDIR /home/

COPY --from=build /home/server.o .

CMD ["./server.o", "6969"]