# Main template for dockerfile
FROM alpine:3.12.0 as build

ARG SOURCE
ARG PARAM

WORKDIR /home/

RUN apk add --no-cache gcc musl-dev zlib-dev

COPY ${SOURCE} .

RUN gcc ${SOURCE} -o main.o -static


FROM alpine:3.12.0

WORKDIR /home/

COPY --from=build /home/main.o .

# Also change this depens on the program
CMD ./main.o  ${PARAM}
