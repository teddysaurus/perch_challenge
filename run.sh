#!/bin/bash

sudo docker build -t perch .
sudo docker run --name redis_instace -d -p 6379:6379 -it --rm redislabs/redistimeseries
