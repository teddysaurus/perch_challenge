FROM ros:melodic-robot

RUN apt-get update && pip install redistimeseries
COPY 