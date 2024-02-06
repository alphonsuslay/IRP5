# IRP5 - IoT Sensor Data to Cloud and Applications

## Setting up a Firebase Project

1. **Go to Firebase Console**: <br>

   • Open your web browser and go to the Firebase Console.

2.  **Create a New Project**: <br>

    • Click on the "Add Project" button.<br>
    • Fill in the necessary information (project name, project ID, etc.).<br>
    • Click "Continue" and follow the prompts to complete the project creation.
  
3. **Enable Firebase Services**: <br>
   
    • Once the project is created, click on the project name in the Firebase Console.<br>
    • In the left-hand menu, navigate to "Firestore" to set up a Firestore database.<br>
    • In addition, also set up Web Hosting in "Hosting", Authentication in "Authentication", and Realtime Database in "Realtime Database".

4. **Get Firebase Configuration**: <br>

    • In the Firebase Console, go to "Project settings" (gear icon next to "Project Overview").<br>
    • Scroll down to the "Your apps" section.<br>
    • Click on the "</>" icon to add a web app to your project.<br>
    • Copy the configuration object provided and keep it on a notepad for later use.


## Integrating Firebase in Visual Studio Code

1. **Install Firebase Tools**: <br>

    • Open your terminal in Visual Studio Code.<br>
    • Run the following command to install Firebase tools globally:

        npm install -g firebase-tools

2. **Login to Firebase**: <br>

    • In the terminal, run the following command to log in to Firebase
  
        firebase login
        
    •   Follow the prompts to log in with your Google account.

3. **Initialize Firebase Project**: <br>

    • Navigate to the root directory of your project in the terminal.<br>
    • Run the following command to initialize Firebase in your project:
    
        firebase init
  
    • Follow the prompts to select the Firebase services you want to use (Firestore, Hosting, etc.).

4. **Configure Firebase Project**: <br>

    • When prompted, select an existing Firebase project (choose the one you created).<br>
    • Choose the Firebase features you want to set up.

## Deploy Web to Firebase
  
1. **Paste Firebase Configuration**: <br>

    • Before deploying your web, you will have to fill up the Firebase configuration object with the configuration you copied earlier into auth.js, dashboard.js, dashboardtable.js.<br>

    ```javascript
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
    ```

6. **Deploy Firebase Project**: <br>

    • After configuring, deploy your project to Firebase by running the following in the terminal:
       firebase deploy

    • The terminal will return with your website URL.


## Setting up Arduino IDE

1. **Downloading Libraries**: <br>

    • Download the libraries from the library folder.<br>
    • In addition, also download firebase library inside the Arduino IDE itself.<br>
    • In the Arduino IDE, Open your library Manager and search for Firebase. Download the 2 libraries by Mobzit<br>    - *Firebase Arduino Client Library for ESP8266 and ESP32* <br>    - *Firebase ESP32 Client*

2. **Setting File Preference**: <br>

    • Click on File > Preference, and set the sketchbook location to be where your libraries are. <br>

3. **Setting up & Running the Firebase Code**: <br>
    • Change the ssid and password to your Wifi SSID and Password.<br>
    • NOTE: Wifi/Hotspot has to be 2.4G bandwidth for the Wifi Module to work.


      ```c++
      const char *ssid = "YOUR_SSID";
      const char *password = "WIFI_PASSWORD";
      ```
    • Head over to your firebase console. On the left navigation bar, open the settings icon beside "Project Overview", and click open the "Project Settings" feature.<br>
    • Copy the Web API Key provided and replace the *API KEY* in the arduino code.

      ```c++
      #define API_KEY ""
      ```
    • Head over to the realtime database, and copy the url as provided. Replace the *DATABASE_URL* in the arduino code

      ```c++
      #define DATABASE_URL "https://{project name}-default-rtdb.{availability zone}.firebasedatabase.app/"
      ```

    • Search for the ``` connectToFirebase()``` function, and modify the email and password.
      ```c++
      // Firebase Setup
      void connectToFirebase()
      {
        config.api_key = API_KEY;
        config.database_url = DATABASE_URL;
        if (Firebase.signUp(&config, &auth, "33sSFssss3@outlook.com", "MkDpI9m22")) //Replace with any random word each time before u run the code
        {
          Serial.println("Sign Up OK");
          SignUpOK = true;
        }
        else
        {
          Serial.printf("Sign Up failed: %s\n", config.signer.signupError.message.c_str());
        }
      
        config.token_status_callback = tokenStatusCallback;
        Firebase.begin(&config, &auth);
        Firebase.reconnectWiFi(true);
      }
      ```
    • Lastly, run the code and wait for between 1 - 5mins for it to upload successfully.
   
4. **Running BLE Code**: <br>

    • After successfully running the code, code will ```Serial.printfln("Waiting a client connection to notify...")<br>
    • You can then use your MIT App and connect to the ESP32. After that, the data will immediately send the data to the Mobile Application.


## MIT App Inventor

1. **Setting up & Running the Mobile Application**: <br>
   
    • Ensure that you have MIT App Companion installed.<br>
    • Download the *aia source file* above and import it into your MIT App Inventor<br>
    • Connect your MIT App Companion with the application by linking it together with the code.<br>
    • NOTE: The application only works on Android.


## Telegram Bot

1. **Setting up & Running the Telegram Bot**: >br>

    • Head over to Firebase Console. <br>
    • Click on the Settings Icon beside "Project Overview" > Open Project Settings > Click on Service Account tabs > Click on Generate New Private Key <br>
    • Copy the content inside and paste it into credentials.json

2. **Creating Telegram Bot***: <br>

    • Start a new conversation with @botfather.
    • Type /newbot > Give the bot a name > Give the Bot a Username
    • Copy the HTTP API Token.
    • Replace the Token and the username with ur API Token and your desired username.

      ```py
      token: Final = 'API_KEY'
      username: Final = 'USERNAME'
      ```
    • Run the main.py file to get the bot Up and Running.

