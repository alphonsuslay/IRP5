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

