#!/bin/bash

sudo docker build --force-rm --no-cache -t perch .
sudo docker run -d --name redis_instance -p 6379:6379 -it --rm redislabs/redistimeseries
