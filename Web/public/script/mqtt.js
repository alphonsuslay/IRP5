
function StartConnect(){

  clientID = "ClientID - "+parseInt(Math.random() * 100);

  host = document.getElementById("host").value;   
  port = document.getElementById("port").value;  


  document.getElementById("consoleframe").innerHTML += "<span> [CONSOLE] Connecting to " + host + " on port " + port+"</span><br>";
  document.getElementById("consoleframe").innerHTML += "<span> [CONSOLE] " + clientID +" </span><br>";

  client = new Paho.MQTT.Client(host,Number(port),clientID);

  client.onConnectionLost = OnConnectionLost;
  client.onMessageArrived = OnMessageArrived;

  client.connect({
      onSuccess: OnConnect
  });
}

function OnConnect(){
  topic = document.getElementById("topic_s").value;
  client.subscribe(topic);
  document.getElementById("consoleframe").innerHTML += "<span> [CONSOLE] Subscribing to Topic: " + topic + "</span> <br>";
  document.getElementById("consoleframe").innerHTML += "---------------------------------------------------</span> <br>";
  document.getElementById("status").value = "SUBSCRIBED";
  document.getElementById("status").classList.remove("status-disconnected");
  document.getElementById("status").classList.add("status-subscribed");
}

function OnConnectionLost(responseObject) {
  if (!intentionalDisconnect) {
    document.getElementById("consoleframe").innerHTML += "<span> [ERROR] Connection is Lost. </span> <br>";

    if (responseObject.errorCode !== 0) {
      document.getElementById("consoleframe").innerHTML += "<span> [ERROR] " + responseObject.errorMessage + "</span> <br>";
    }
  }
  document.getElementById("status").value = "DISCONNECTED";
  document.getElementById("status").classList.remove("status-subscribed");
  document.getElementById("status").classList.add("status-disconnected");
  intentionalDisconnect = false;
}


function OnMessageArrived(message) {
  console.log("OnMessageArrived" + message.payloadString);
  document.getElementById("consoleframe").innerHTML += "<span> [SUBSCRIBE] Topic: " + message.destinationName + "</span> <br>";
  document.getElementById("consoleframe").innerHTML += "<span> [SUBSCRIBE] Message: " + message.payloadString + "</span> <br>";
  document.getElementById("consoleframe").innerHTML += "---------------------------------------------------</span> <br>";
}


let intentionalDisconnect = false;

function StartDisconnect() {
  intentionalDisconnect = true;
  client.disconnect();
  document.getElementById("consoleframe").innerHTML += "<span> [CONSOLE] Disconnected. </span><br>";
  document.getElementById("consoleframe").innerHTML += "---------------------------------------------------</span> <br>";
  document.getElementById("status").value = "DISCONNECTED";
  document.getElementById("status").classList.remove("status-subscribed");
  document.getElementById("status").classList.add("status-disconnected");
}

function publishMessage() {
  msg = document.getElementById("Message").value;
  topic = document.getElementById("topic_p").value;
  Message = new Paho.MQTT.Message(msg);
  Message.destinationName = topic;

  client.send(Message);
  document.getElementById("consoleframe").innerHTML += "<span> [PUBLISH] Message to topic "+topic+" is sent </span><br>";
  document.getElementById("consoleframe").innerHTML += "---------------------------------------------------</span> <br>";
}
