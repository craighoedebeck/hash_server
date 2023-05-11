#!/bin/bash

#Build the Docker image
docker build -t hash_server .

#Run the Docker image
docker run -it -p 5000:5000 hash_server
