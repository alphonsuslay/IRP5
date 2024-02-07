import { initializeApp } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-app.js";
import { getDatabase, ref, onValue, set } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-database.js";

// Initialize Firebase
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

let myChart; // Variable to store the chart instance
let chartData = []; // Array to store chart data

const PushButtonTopic = localStorage.getItem('PushButtonTopic') || 'Button/PB';
const buttonRef = ref(database, PushButtonTopic);




// Function to get data from Firebase and update the chart, temperature, and humidity labels
function getData() {
  // Get the Humidity Topic from localStorage or set a default topic
  const humidityTopic = localStorage.getItem('HumidityTopic') || 'DHT/Humidity';
  const TemperatureTopic = localStorage.getItem('TemperatureTopic') || 'DHT/Temperature';
  const HeartRateTopic = localStorage.getItem('HeartRateTopic') || 'Sensor/Rotary';
  const GreenLEDTopic = localStorage.getItem('GreenLEDTopic') || 'LED/Green';
  const RedLEDTopic = localStorage.getItem('RedLEDTopic') || 'LED/Red';

  const MPU6050AccelerationRefX = ref(database, 'MPU6050/Acceleration/X');
  const MPU6050AccelerationRefY = ref(database, 'MPU6050/Acceleration/Y');
  const MPU6050AccelerationRefZ = ref(database, 'MPU6050/Acceleration/Z');

  const MPU6050RotationRefX = ref(database, 'MPU6050/Rotation/X');
  const MPU6050RotationRefY = ref(database, 'MPU6050/Rotation/Y');
  const MPU6050RotationRefZ = ref(database, 'MPU6050/Rotation/Z');

  
  // Set the humidityRef based on the Humidity Topic
  var redBulbRef = ref(database, RedLEDTopic);
  var greenBulbRef = ref(database, GreenLEDTopic);
  var humidityRef = ref(database, humidityTopic);
  var rotaryRef = ref(database, HeartRateTopic);
  var temperatureRef = ref(database, TemperatureTopic);
  

  // Listener for Rotary data
  onValue(rotaryRef, (rotarySnapshot) => {
    var rotaryData = rotarySnapshot.val();
    console.log('Rotary Sensor Reading:', rotaryData);

    // Update the chart data array
    chartData.push(rotaryData);

    // Destroy the existing chart if it exists
    if (myChart) {
      myChart.destroy();
    }

    // Call the function to create/update the line chart with the latest data
    createLineChart();

    const priorityHighElement = document.getElementById('heartratewarning');
    if (priorityHighElement) {
      if (rotaryData > 100 || rotaryData < 60) {
        showPriorityHigh(priorityHighElement);
      } else {
        hidePriorityHigh(priorityHighElement);
      }
    }

  });

  // Listener for Humidity data
  onValue(humidityRef, (humiditySnapshot) => {
    var humidityData = humiditySnapshot.val();
    console.log('Humidity Snapshot:', humiditySnapshot);
    console.log('Humidity Reading:', humidityData);

    // Update the humidity label
    updateHumidityLabel(humidityData);
  });

  // Listener for Temperature data
  onValue(temperatureRef, (temperatureSnapshot) => {
    var temperatureData = temperatureSnapshot.val();
    console.log('Temperature Snapshot:', temperatureSnapshot);
    console.log('Temperature Reading:', temperatureData);

    // Update the temperature label
    updateTemperatureLabel(temperatureData);

    // Show/hide temperature warning based on the condition
    const temperatureWarningElement = document.getElementById('temperaturewarning');
    if (temperatureWarningElement) {
      if (temperatureData > 35) {
        showTemperatureWarning(temperatureWarningElement);
      } else {
        hideTemperatureWarning(temperatureWarningElement);
      }
    }
  });

  // Listener for Red LED data
  onValue(redBulbRef, (redBulbSnapshot) => {
    const redBulbData = redBulbSnapshot.val();
    console.log('Red LED Snapshot:', redBulbSnapshot);
    console.log('Red LED Reading:', redBulbData);

    // Update the red bulb based on the value
    updateRedBulb('redBulb', redBulbData);
  });

  // Listener for Green LED data
  onValue(greenBulbRef, (greenBulbSnapshot) => {
    const greenBulbData = greenBulbSnapshot.val();
    console.log('Green LED Snapshot:', greenBulbSnapshot);
    console.log('Green LED Reading:', greenBulbData);

    // Update the green bulb based on the value
    updateGreenBulb('greenBulb', greenBulbData);
  });

  onValue(MPU6050AccelerationRefX, (accelerationSnapshotX) => {
    const accelerationDataX = accelerationSnapshotX.val();
    console.log('Acceleration X Snapshot:', accelerationSnapshotX);
    console.log('Acceleration X Reading:', accelerationDataX);
  
    // Update the X acceleration label
    updateAccelerationLabel('AccelerationX', accelerationDataX);
  });

  onValue(MPU6050AccelerationRefY, (accelerationSnapshotY) => {
    const accelerationDataY = accelerationSnapshotY.val();
    console.log('Acceleration Y Snapshot:', accelerationSnapshotY);
    console.log('Acceleration Y Reading:', accelerationDataY);
  
    // Update the Y acceleration label
    updateAccelerationLabel('AccelerationY', accelerationDataY);
  });

  onValue(MPU6050AccelerationRefZ, (accelerationSnapshotZ) => {
    const accelerationDataZ = accelerationSnapshotZ.val();
    console.log('Acceleration Z Snapshot:', accelerationSnapshotZ);
    console.log('Acceleration Z Reading:', accelerationDataZ);
  
    // Update the Z acceleration label
    updateAccelerationLabel('AccelerationZ', accelerationDataZ);
  });

  onValue(MPU6050RotationRefX, (rotationSnapshotX) => {
    const rotationDataX = rotationSnapshotX.val();
    console.log('Rotation X Snapshot:', rotationSnapshotX);
    console.log('Rotation X Reading:', rotationDataX);
  
    // Update the X rotation label
    updateRotationLabel('RotationX', rotationDataX);
  });
  
  onValue(MPU6050RotationRefY, (rotationSnapshotY) => {
    const rotationDataY = rotationSnapshotY.val();
    console.log('Rotation Y Snapshot:', rotationSnapshotY);
    console.log('Rotation Y Reading:', rotationDataY);
  
    // Update the Y rotation label
    updateRotationLabel('RotationY', rotationDataY);
  });
  
  onValue(MPU6050RotationRefZ, (rotationSnapshotZ) => {
    const rotationDataZ = rotationSnapshotZ.val();
    console.log('Rotation Z Snapshot:', rotationSnapshotZ);
    console.log('Rotation Z Reading:', rotationDataZ);
  
    // Update the Z rotation label
    updateRotationLabel('RotationZ', rotationDataZ);
  });
}

  function updateRotationLabel(rotationId, value) {
    console.log(`Received rotation data for ${rotationId}:`, value);

    const rotationElement = document.getElementById(rotationId);
    if (rotationElement) {
      // Update the rotation value
      rotationElement.textContent = value !== null ? `${rotationId.charAt(rotationId.length - 1)}: ${value}` : 'N/A';
    }
  }



// Function to update the acceleration label
function updateAccelerationLabel(accelerationId, value) {
  const accelerationElement = document.getElementById(accelerationId);
  if (accelerationElement) {
    // Update the acceleration value
    accelerationElement.textContent = value !== null ? `${accelerationId.charAt(accelerationId.length - 1)}: ${value}` : 'N/A';
  }
}



// Function to update the bulb based on the LED value
function updateGreenBulb(bulbId, value) {
  const bulbElement = document.getElementById(bulbId);
  if (bulbElement) {
    // If the LED value is 1, make the bulb glow, otherwise turn it off
    bulbElement.classList.toggle('Greenglow', value === 1);
  }
}

// Function to update the bulb based on the LED value
function updateRedBulb(bulbId, value) {
  const bulbElement = document.getElementById(bulbId);
  if (bulbElement) {
    // If the LED value is 1, make the bulb glow, otherwise turn it off
    bulbElement.classList.toggle('Redglow', value === 1);
  }
}


// Function to update the temperature label
function updateTemperatureLabel(temperature) {
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

// Function to show priority_high icon and set Heart Rate section color
function showPriorityHigh(element) {
  element.textContent = 'priority_high';
  element.style.color = 'red'; // Set color to red

  // Set color for the Heart Rate section
  setHeartRateSectionColor('red');

  const heartrateSection = document.querySelector('.hr');
  if (heartrateSection) {
    heartrateSection.style.border = '5px solid red';
  }
}

// Function to hide priority_high icon and reset Heart Rate section color
function hidePriorityHigh(element) {
  element.textContent = ''; // Remove text content

  // Reset color for the Heart Rate section
  setHeartRateSectionColor('');

  // Remove the border style for the heartrate section
  const heartrateSection = document.querySelector('.hr');
  if (heartrateSection) {
    heartrateSection.style.border = '';
  }
}

// Function to set color for the Heart Rate section
function setHeartRateSectionColor(color) {
  const heartRateSection = document.getElementById('heartRateSection');
  if (heartRateSection) {
    heartRateSection.style.color = color;
  }
}

// Function to show temperature warning
function showTemperatureWarning(element) {
  element.textContent = 'priority_high';
  element.style.color = 'red';

  // Set color for the Temperature section
  setTemperatureSectionColor('red');

  const temperatureSection = document.querySelector('.Temperature');
  if (temperatureSection) {
    temperatureSection.style.border = '5px solid red';
  }
}

// Function to hide temperature warning
function hideTemperatureWarning(element) {
  element.textContent = '';
  
  // Reset color for the Temperature section
  setTemperatureSectionColor('');

  // Remove the border style for the Temperature section
  const temperatureSection = document.querySelector('.Temperature');
  if (temperatureSection) {
    temperatureSection.style.border = '';
  }
}

// Function to set color for the Temperature section
function setTemperatureSectionColor(color) {
  const temperatureSection = document.querySelector('.Temperature');
  if (temperatureSection) {
    temperatureSection.style.color = color;
  }
}

// Call the function to get data when the script is loaded
document.addEventListener('DOMContentLoaded', getData);

// Function to create/update the line chart
function createLineChart() {
  console.log('Creating chart with data:', chartData);

  const ctx = document.getElementById('healthChart').getContext('2d');
  myChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: chartData.map((_, index) => index + 1), // Assuming array index as labels
      datasets: [{
        label: 'Readings',
        data: chartData,
        borderColor: 'blue',
        borderWidth: 1,
        fill: false,
      }],
    },
    options: {
      scales: {
        x: {
          type: 'category',
          labels: chartData.map((_, index) => index + 1), // Assuming array index as labels
          scaleLabel: {
            display: true,
            labelString: 'Time',
          },
        },
        y: {
          beginAtZero: true,
          scaleLabel: {
            display: true,
            labelString: 'Reading (bpm)',
          },
        },
      },
      maintainAspectRatio: false,
      responsive: true,
      plugins: {
        title: {
          display: true,
        },
      },
    },
  });
}


// Select the buttons by ID
const pushBtnON = document.getElementById('PushBtnON');
const pushBtnOFF = document.getElementById('PushBtnOFF');


// Add click event listeners
pushBtnON.addEventListener('click', () => {
  // Write a value of 1 to "Button/PB"
  set(buttonRef, 1);
  console.log("Sent to Button/PB = 1")
});

pushBtnOFF.addEventListener('click', () => {
  // Write a value of 0 to "Button/PB"
  set(buttonRef, 0);
  console.log("Sent to Button/PB = 0")

});

// Add a click event listener
HumidityMenu.addEventListener('click', () => {
  // Open a popup window with a specified URL
  window.open('../popup/humidity.html', 'Popup', 'width=600,height=400');
});


// Add a click event listener
TemperatureMenu.addEventListener('click', () => {
  // Open a popup window with a specified URL
  window.open('../popup/temperature.html', 'Popup', 'width=600,height=400');
});

// Add a click event listener
HeartRateMenu.addEventListener('click', () => {
  // Open a popup window with a specified URL
  window.open('../popup/chart.html', 'Popup', 'width=600,height=400');
});

// Add a click event listener
PushButtonMenu.addEventListener('click', () => {
  // Open a popup window with a specified URL
  window.open('../popup/pushbutton.html', 'Popup', 'width=600,height=400');
});

LEDMenu.addEventListener('click', () => {
  // Open a popup window with a specified URL
  window.open('../popup/LED.html', 'Popup', 'width=600,height=400');
});
