
# server stuff
server:
	gcc echoserver.c -o server.o -static

docker-server: server server.o
	docker build -f server.Dockerfile --build-arg SOURCE=server.o -t daytime-server .

run-server:
	docker run --env PORT=7070 daytime-server


# client stuff

client:
	gcc echoclient.c -o client.o -static

docker-client: client client.o
	docker build -f client.Dockerfile --build-arg SOURCE=client.o -t daytime-client .

run-client:
	docker run --env PORT=7070 --env HOST=localhost daytime-client
