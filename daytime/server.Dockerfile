# need uncomment some stuff to build locally
FROM alpine:3.12.0

ARG SOURCE
ARG PORT

WORKDIR /home/

#RUN apk add --no-cache gcc musl-dev zlib-dev
#COPY ${SOURCE} .
#RUN gcc ${SOURCE} -o main.o -static


#FROM alpine:3.12.0
#WORKDIR /home/
#COPY --from=build /home/main.o .
COPY ./${SOURCE} /home/main.o

# Also change this depends on the program
CMD ./main.o ${PORT}