#!/usr/bin/env python

import rospy
from redistimeseries.client import Client
from std_msgs.msg import Int32
from std_msgs.msg import Int64
from std_msgs.msg import String

product_event = ['up', 'down', 'review', 'video', 'accessories', 'close' ]
vendor = "KWIKEMART:"


def event_callback(msg):
    message = msg.data.split('/')
    print "[" + str(fake_clock) + "] " + vendor + message[0] + " " + str(product_event.index(message[1]))
    rts.add(vendor + message[0], fake_clock, product_event.index(message[1]))

def time_callback(msg):
    global fake_clock;
    fake_clock = msg.data
    

rospy.init_node('ros_redis')
fake_clock = 0;

rts = Client()

for i in range(1,5):
    try:
        rts.create(vendor + 'PRODUCT' + str(i))
        print vendor + 'PRODUCT' + str(i)
        break
    except:
        pass


prod_sub = rospy.Subscriber('perch_product', String, event_callback)
screen_sub = rospy.Subscriber('perch_screen', String, event_callback)
clock_sub = rospy.Subscriber('perch_clock', Int64, time_callback)
rospy.spin()

