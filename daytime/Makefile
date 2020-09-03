build-server:
	gcc server.c -o server.o -static

docker-server: build-server server.o
	docker build -f server.Dockerfile --build-arg SOURCE=server.o -t daytime-server .

run-server:
	docker run daytime-server --env PORT=7070
