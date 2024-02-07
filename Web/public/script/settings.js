import { initializeApp } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-app.js";
import { getFirestore, doc as firestoreDoc, setDoc, addDoc, collection, getDocs, where } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-firestore.js";

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
const db = getFirestore();

// Retrieve userEmail from localStorage
var userEmail = localStorage.getItem('userEmail');
console.log("User Email:", userEmail);

// Get user document reference
const userDocRef = firestoreDoc(db, 'users', userEmail);



// Your existing saveUserDetails function
function saveUserDetails() {
  var username = document.getElementById('usernameInput').value;
  var email = document.getElementById('emailInput').value;
  var dob = document.getElementById('dobInput').value;
  var gender = document.querySelector('input[name="gender"]:checked').value;

  var userDetails = {
    username: username,
    dob: dob,
    gender: gender
  };

  // Save or update user details in the Firestore collection
  if (userEmail) {
    setDoc(userDocRef, userDetails)
      .then(() => {
        console.log("Document updated with ID: ", userEmail);

        // Change the button style to indicate success
        var saveButton = document.querySelector('.save-button');
        saveButton.style.backgroundColor = '#41f1b6'; // Change to your desired color
        saveButton.innerHTML = 'Saved';
      })
      .catch((error) => {
        console.error("Error updating document: ", error);
      });
  } else {
    // userEmail does not exist, create a new document
    addDoc(collection(db, 'users'), { ...userDetails, email: email })
      .then((docRef) => {
        console.log("Document written with ID: ", docRef.id);
        // Update userEmail in localStorage after creating a new document
        userEmail = email;
        localStorage.setItem('userEmail', userEmail);
        // Re-initialize the screen to reflect the changes
        initializeScreen();
        
        // Change the button style to indicate success
        var saveButton = document.querySelector('.save-button');
        saveButton.style.backgroundColor = '#41f1b6'; // Change to your desired color
        saveButton.innerHTML = 'Saved';
      })
      .catch((error) => {
        console.error("Error adding document: ", error);
      });
  }
}

function saveHealthDetails() {
  var weight = document.getElementById('weightInput').value;
  var height = document.getElementById('heightInput').value;
  var bloodType = document.getElementById('bloodtypeInput').value;
  var allergies = document.getElementById('allergiesInput').value;

  var healthDetails = {
    weight: weight,
    height: height,
    bloodType: bloodType,
    allergies: allergies
  };

  // Save health details inside the user's document
  if (userEmail) {
    setDoc(userDocRef, { healthDetails: healthDetails }, { merge: true })
      .then(() => {
        console.log("Health details saved successfully");

        // Change the button style to indicate success
        var saveButton = document.querySelector('.save-button2');
        saveButton.style.backgroundColor = '#41f1b6'; // Change to your desired color
        saveButton.innerHTML = 'Saved';
      })
      .catch((error) => {
        console.error("Error saving health details: ", error);
      });
  }
}


function initializeScreen() {
  // If userEmail exists, set email input to readonly
  if (userEmail) {
    document.getElementById('emailInput').readOnly = true;

    // Set the email input value
    document.getElementById('emailInput').value = userEmail;

    // Fetch and display user details
    getDocs(collection(db, 'users'))
      .then((querySnapshot) => {
        querySnapshot.forEach((doc) => {
          const userData = doc.data();
          if (doc.id === userEmail) {
            // Match found, display user details
            displayUserDetails(userData);

            // Display health details directly from userData
            displayHealthDetails(userData);

          }
        });
      })
      .catch((error) => {
        console.error("Error getting documents: ", error);
      });
  }
}





function displayUserDetails(userData) {
  // Populate the input fields with user details
  document.getElementById('usernameInput').value = userData.username;
  document.getElementById('dobInput').value = userData.dob;
  const genderRadio = userData.gender === 'M' ? 'male' : 'female';
  document.getElementById(genderRadio).checked = true;
}

function displayHealthDetails(userData) {
  // Check if health details exist in the user's data
  if (userData.healthDetails) {
    const healthData = userData.healthDetails;

    // Populate the input fields with health details
    document.getElementById('weightInput').value = healthData.weight ? healthData.weight : '';
    document.getElementById('heightInput').value = healthData.height ? healthData.height : '';
    document.getElementById('bloodtypeInput').value = healthData.bloodType || '';
    document.getElementById('allergiesInput').value = healthData.allergies || '';
  } else {
    // If no health details exist, clear the input fields
    document.getElementById('weightInput').value = '';
    document.getElementById('heightInput').value = '';
    document.getElementById('bloodtypeInput').value = '';
    document.getElementById('allergiesInput').value = '';
  }
}





// Call initializeScreen when the page loads
initializeScreen();

var saveButton = document.querySelector('.save-button');
saveButton.addEventListener('click', saveUserDetails);

var saveButton2 = document.querySelector('.save-button2');
saveButton2.addEventListener('click', saveHealthDetails);
