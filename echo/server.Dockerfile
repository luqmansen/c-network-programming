# need uncomment some stuff to build locally
FROM alpine:3.12.0

ARG SOURCE
ARG PORT

#WORKDIR /home/

#RUN apk add --no-cache gcc musl-dev zlib-dev
#COPY ${SOURCE} .
#RUN gcc ${SOURCE} -o main.o -static


#FROM alpine:3.12.0
#WORKDIR /home/
#COPY --from=build /home/main.o .
COPY server.o main.o

EXPOSE 6969
# Also change this depends on the program
CMD ./main.o ${PORT}