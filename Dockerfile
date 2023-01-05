# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest


RUN apt-get update && apt-get install build-essential cmake  c++
COPY . /usr/src
WORKDIR  /usr/src