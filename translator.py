import paho.mqtt.client as mqtt
import json

def json_factory(json, keys):
    return {key: json[key] for key in keys if key in json}

def on_connect(client, userdata, flags, return_code):
    if return_code == 0:
        print("connected")
        client.subscribe("domoticz/out")
    else:
        print("could not connect, return code:", return_code)


def on_message(client, userdata, message):
    msg_decoded = str(message.payload.decode("utf-8"))
    print("Received message: " ,msg_decoded, "topic: ", message.topic)
    msg_json = json.loads(msg_decoded)

    print("msg_json: ", msg_json)
    
    with open('translator.conf') as config_file:
        msg_json_shortened = json_factory(msg_json, [line.strip() for line in config_file])

    print("msg_json_shortened: ", msg_json_shortened)

    msg_json_reencoded = json.dumps(msg_json_shortened)
    

    topic = f'esp/out/{msg_json_shortened["idx"]}'
    result = client.publish(topic, msg_json_reencoded)
    status = result[0]
    if status == 0:
        print(f"Message published to topic {topic} msg size : {len(msg_json_reencoded)}")
    else:
        print(f"Failed to send message to topic {topic} msg size : {len(msg_json_reencoded)}")

broker_hostname = "1.tcp.eu.ngrok.io"
port = 21589


if __name__ == "__main__":
    client = mqtt.Client("Translator")
    client.username_pw_set(username="umpanumiw", password="umpanumiw") # uncomment if you use password auth
    client.on_connect=on_connect
    client.on_message=on_message

    client.connect(broker_hostname, port) 
    client.loop_forever()
    