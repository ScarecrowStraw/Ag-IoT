import paho.mqtt.client as mqtt
from random import seed
from random import randint
import time
import json
seed(1)

def on_connect(mqttc, obj, flags, rc):
    print("rc: " + str(rc))


def on_message(mqttc, obj, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))


def on_publish(mqttc, obj, mid):
    print("mid: " + str(mid))
    pass


def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))


def on_log(mqttc, obj, level, string):
    print(string)


# If you want to use a specific client id, use
# mqttc = mqtt.Client("client-id")
# but note that the client id must be unique on the broker. Leaving the client
# id parameter empty will generate a random id for you.
mqttc = mqtt.Client()
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
# Uncomment to enable debug messages
# mqttc.on_log = on_log
mqttc.connect("citlab.myftp.org", 1883, 60)

mqttc.loop_start()
dict = {'t': 100, 'h': 100, 'MQ7': 100}

while(True):
    dict['t'] = 25 + randint(0,5)
    dict['h'] = randint(40,80)
    mess = json.dumps(dict)
    infot = mqttc.publish("demo/test", mess, qos=2)
    infot.wait_for_publish()
    infot = mqttc.publish("demo/test-2", mess, qos=2)
    infot.wait_for_publish()
    print("looping...", mess)
    time.sleep(10)

