import { initializeApp } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-app.js";
import { getDatabase, ref, onValue, set } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-database.js";


// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "",
  authDomain: "",
  databaseURL: "",
  projectId: "",
  storageBucket: "",
  messagingSenderId: "",
  appId: "",
  measurementId: ""
};


const app = initializeApp(firebaseConfig);
const database = getDatabase();


// Topic Configuration //
const dhtTemperatureTopic = "DHT/Temperature";
const dhtHumidityTopic = "DHT/Humidity";
const phTopic = "LED/Green";
const MoistureTopic = 'LED/Red';
const ldrTopic = 'Sensor/Rotary';
const co2Topic = 'MPU6050/Acceleration/X';

// Variable to store previous readings
let prevReadings = {
  temperature: null,
  humidity: null,
  pH: null,
  moisture: null,
  ldr: null,
  co2: null
};
let TempData = null;
let HumidityData = null;
let pHData = null;
let MoistureData = null;
let ldrData = null;
let co2Data = null;



function getData() {
  var dhtTemperatureRef = ref(database, dhtTemperatureTopic);
  var dhtHumidityRef = ref(database, dhtHumidityTopic);
  var phRef = ref(database, phTopic);
  var MoistureRef = ref(database, MoistureTopic);
  var ldrRef = ref(database, ldrTopic);
  var co2Ref = ref(database, co2Topic);

  
  // Listener for Temperature data
  onValue(dhtTemperatureRef, (TempSnapshot) => {
    TempData = TempSnapshot.val();
    console.log('Temperature Reading:', TempData);

    // Update the humidity label
    updateTempLabel(TempData);

    // Check if TempData is in the warning zone
    checkWarningZone(TempData);

    // Check if TempData is in the danger zone
    checkDangerZone(TempData);
  });

  // Listener for Humioity data
  onValue(dhtHumidityRef, (HumiditySnapshot) => {
    HumidityData = HumiditySnapshot.val();
    console.log('Humidity Reading:', HumidityData);

    // Update the humidity label
    updateHumidityLabel(HumidityData);

    checkWarningZone(TempData, HumidityData);

    // Check if TempData is in the danger zone
    checkDangerZone(TempData, HumidityData);

  });

  // Listener for pH data
  onValue(phRef, (pHSnapshot) => {
    pHData = pHSnapshot.val();
    console.log('pH Reading:', pHData);

    // Update the humidity label
    updatepHLabel(pHData);

    // Check if pHData is in the warning zone
    checkWarningZone(TempData, HumidityData, ldrData, pHData);
    
    // Check if pHData is in the danger zone
    checkDangerZone(TempData, HumidityData, ldrData, pHData);
  });

  // Listener for Moisture data
  onValue(MoistureRef, (MoistureSnapshot) => {
    MoistureData = MoistureSnapshot.val();
    console.log('Moisture Reading:', MoistureData);

    // Update the humidity label
    updateMoistureLabel(MoistureData);

    // Check if MoistureData is in the warning zone
    checkWarningZone(TempData, HumidityData, ldrData, pHData, MoistureData);

    // Check if MoistureData is in the warning zone
    checkDangerZone(TempData, HumidityData, ldrData, pHData, MoistureData);
  });

  // Listener for LDR data
  onValue(ldrRef, (ldrSnapshot) => {
    ldrData = ldrSnapshot.val();
    console.log('LDR Reading:', ldrData);

    // Update the LDR label
    updateLDRLabel(ldrData);

    // Check if ldrData is in the warning zone
    checkWarningZone(TempData, HumidityData, ldrData);

    // Check if pHData is in the danger zone
    checkDangerZone(TempData, HumidityData, ldrData, pHData);
  });

  // Listener for C02 data
  onValue(co2Ref, (co2Snapshot) => {
    co2Data = co2Snapshot.val();
    console.log('CO2 Reading:', co2Data);

    // Update the CO2 label
    updateco2Label(co2Data);
  });

  // Function to compare current readings with previous readings
function compareReadings(currentReading, statusId) {
  const prevReading = prevReadings[statusId];
  const statusElement = document.getElementById(statusId);

  // Find the h4 element inside the status div
  const statusTextElement = statusElement.querySelector('h4');

  // Find the corresponding notifs div
  const notifsElement = document.getElementById(`${statusId}notifs`);

  // If readings are the same, set background color to red and display 'OFFLINE' text
  if (currentReading !== null && prevReading !== null && currentReading === prevReading) {
    statusElement.style.backgroundColor = '#ff7782';
    statusTextElement.textContent = 'OFFLINE';
    // Update notifs text
    notifsElement.innerHTML = `<h4>Component is Offline</h4>`;
  } else {
    // If readings are different, remove background color and display 'ONLINE' text
    statusElement.style.backgroundColor = '';  // Set it to an empty string to remove the background color
    statusTextElement.textContent = 'ONLINE';
    // Update notifs text
    notifsElement.innerHTML = `<h4>Component is Online</h4>`;
  }

  // Update previous readings
  prevReadings[statusId] = currentReading;
}



  // Timer to compare readings every 10 seconds
  setInterval(() => {
    compareReadings(TempData, 'temp');
    compareReadings(HumidityData, 'Humidity');
    compareReadings(ldrData, 'LDR');
    compareReadings(co2Data, 'co2');
    compareReadings(pHData, 'pH');
    compareReadings(MoistureData, 'Moisture');
  }, 20000);  // Change this to 10000 to compare every 10 seconds

  function checkWarningZone(temperature, humidity, ldrData, pHData, MoistureData) {
    // Find the "Important" card's content div
    const importantCard = document.querySelector('.warning');
  
    // Check if the important card exists
    if (importantCard) {
      // Find the .card-content element or create one if it doesn't exist
      let importantCardContent = importantCard.querySelector('.card-content');
      if (!importantCardContent) {
        importantCardContent = document.createElement('div');
        importantCardContent.classList.add('card-content');
        importantCard.appendChild(importantCardContent);
      }
  
      // Check if temperature is in the danger zone
      if ((temperature >= 5 && temperature <= 10) || (temperature >= 35 && temperature <= 37)) {
        // Create or get the existing h4 element for temperature
        let tempH4Element = importantCardContent.querySelector('.temperature-h4');
        if (!tempH4Element) {
          tempH4Element = document.createElement('h4');
          tempH4Element.classList.add('temperature-h4');
          importantCardContent.appendChild(tempH4Element);
        }
  
        // Set the text content of the temperature h4 element
        tempH4Element.textContent = 'Temperature is in the WARNING ZONE';
      } else {
        // If not in the danger zone for temperature, remove the temperature h4 element
        importantCardContent.querySelector('.temperature-h4')?.remove();
      }
  
      // Check if ldrData is in the danger zone
      if ((ldrData >= 101 && ldrData <= 119)) {
        // Create or get the existing h4 element for ldrData
        let ldrH4Element = importantCardContent.querySelector('.ldr-h4');
        if (!ldrH4Element) {
          ldrH4Element = document.createElement('h4');
          ldrH4Element.classList.add('ldr-h4');
          importantCardContent.appendChild(ldrH4Element);
        }
  
        // Set the text content of the ldrData h4 element
        ldrH4Element.textContent = 'Heart Rate is in the WARNING ZONE';
      } else {
        // If not in the danger zone for ldrData, remove the ldrData h4 element
        importantCardContent.querySelector('.ldr-h4')?.remove();
      }
    }
  }
  
  function checkDangerZone(temperature, humidity, ldrData, pHData, MoistureData) {
    // Find the "Warning" card's content div
    const warningCard = document.querySelector('.danger');
  
    // Check if the warning card exists
    if (warningCard) {
      // Find the .card-content element or create one if it doesn't exist
      let warningCardContent = warningCard.querySelector('.card-content');
      if (!warningCardContent) {
        warningCardContent = document.createElement('div');
        warningCardContent.classList.add('card-content');
        warningCard.appendChild(warningCardContent);
      }
  
      // Check if temperature is in the warning zone
      if (temperature < 5 || temperature > 37) {
        // Create or get the existing h4 element for temperature warning
        let tempWarningH4Element = warningCardContent.querySelector('.temperature-warning-h4');
        if (!tempWarningH4Element) {
          tempWarningH4Element = document.createElement('h4');
          tempWarningH4Element.classList.add('temperature-warning-h4');
          warningCardContent.appendChild(tempWarningH4Element);
        }
  
        // Set the text content of the temperature warning h4 element
        tempWarningH4Element.textContent = 'Temperature Level is in the WARNING ZONE';
      } else {
        // If not in the warning zone for temperature, remove the temperature warning h4 element
        warningCardContent.querySelector('.temperature-warning-h4')?.remove();
      }
      // Check if ldrData is in the danger zone
      if (ldrData > 119) {
        // Create or get the existing h4 element for ldrData warning
        let ldrWarningH4Element = warningCardContent.querySelector('.ldr-warning-h4');
        if (!ldrWarningH4Element) {
          ldrWarningH4Element = document.createElement('h4');
          ldrWarningH4Element.classList.add('ldr-warning-h4');
          warningCardContent.appendChild(ldrWarningH4Element);
        }
  
        // Set the text content of the ldrData warning h4 element
        ldrWarningH4Element.textContent = 'Heart Rate is in the DANGER ZONE';
      } else {
        // If not in the danger zone for ldrData, remove the ldrData warning h4 element
        warningCardContent.querySelector('.ldr-warning-h4')?.remove();
      }
    }
  }
  

}

// Call the function to get data when the script is loaded
document.addEventListener('DOMContentLoaded', getData);

// Function to update the temperature label
function updateTempLabel(temperature) {
  const temperatureLabel = document.getElementById('TemperatureLb');
  if (temperatureLabel) {
    temperatureLabel.innerHTML = `<h6>${temperature !== null ? temperature + '°C' : 'N/A'}</h6>`;
  }
}

// Function to update the humidity label
function updateHumidityLabel(humidity) {
  const humidityLabel = document.getElementById('humidityLb');
  if (humidityLabel) {
    humidityLabel.innerHTML = `<h6>${humidity !== null ? humidity + '%' : 'N/A'}</h6>`;
  }
}

// Function to update the pH label
function updatepHLabel(pH) {
  const pHLabel = document.getElementById('pHLabel');
  if (pHLabel) {
    pHLabel.innerHTML = `<h6>${pH !== null ? pH : 'N/A'}</h6>`;
  }
}

// Function to update the Moisture label
function updateMoistureLabel(Moisture) {
  const MoistureLabel = document.getElementById('MoistureLabel');
  if (MoistureLabel) {
    MoistureLabel.innerHTML = `<h6>${Moisture !== null ? Moisture: 'N/A'}</h6>`;
  }
}

// Function to update the LDR label
function updateLDRLabel(ldr) {
  const LDRLabel = document.getElementById('LDRLabel');
  if (LDRLabel) {
    LDRLabel.innerHTML = `<h6>${ldr !== null ? ldr + " bpm" : 'N/A'}</h6>`;
  }
}

// Function to update the CO2 label
function updateco2Label(CO2) {
  const co2Label = document.getElementById('co2Label');
  if (co2Label) {
    co2Label.innerHTML = `<h6>${CO2 !== null ? CO2 : 'N/A'}</h6>`;
  }
}