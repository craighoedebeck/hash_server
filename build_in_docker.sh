#!/bin/bash

#Build the Docker image
docker build -t hash_server .

#Run the Docker image
docker run -it --network="host" hash_server
