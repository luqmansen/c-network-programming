
build:
	gcc server.c -o server.o -static

docker: build server.o
	docker build -f Dockerfile.server -t daytime-server .

run:
	docker run daytime-server