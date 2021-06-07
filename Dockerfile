FROM ros:melodic-robot

RUN apt-get update && apt-get install -y python-pip && pip install redistimeseries 

WORKDIR /perch_ws

RUN git clone https://github.com/teddysaurus/perch_challenge.git

RUN /bin/bash -c 'cd perch_challenge && source /opt/ros/melodic/setup.bash && catkin_make'