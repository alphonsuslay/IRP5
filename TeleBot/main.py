from typing import Final
from telegram import Update, InlineKeyboardButton, InlineKeyboardMarkup
from telegram.ext import Application, CommandHandler, ContextTypes, CallbackContext, CallbackQueryHandler
import firebase_admin
from firebase_admin import credentials, db

token: Final = ''
username: Final = ''


cred = credentials.Certificate("credentials.json")
firebase_admin.initialize_app(cred, {'databaseURL': "https://irp-fyp-project-default-rtdb.asia-southeast1.firebasedatabase.app/"})

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text("Welcome! I am here to help! If you need assistance, use the help command.")

async def help(update: Update, context: ContextTypes.DEFAULT_TYPE):
    # Define the list of commands and their descriptions
    commands = [
        ("/start", "Start the bot"),
        ("/help", "Display available commands"),
        ("/status", "Get the status of the ESP32 sensors"),
        ("/pushbutton", "Control the push button (ON/OFF)"),
    ]

    # Create a formatted response with the list of commands
    response_text = "Available commands:\n\n"
    for command, description in commands:
        response_text += f"{command}: {description}\n\n"

    # Send the response to the user
    await update.message.reply_text(response_text)


async def status(update: Update, context: ContextTypes.DEFAULT_TYPE):
    # Dictionary to store sensor names and corresponding Firebase paths
    sensors = {
        "Rotary": "Sensor/Rotary",
        "DHT Humidity": "DHT/Humidity",
        "DHT Temperature": "DHT/Temperature",
        "LED Red": "LED/Red",
        "LED Green": "LED/Green",
        "MPU6050 Acceleration X": "MPU6050/Acceleration/X",
        "MPU6050 Acceleration Y": "MPU6050/Acceleration/Y",
        "MPU6050 Acceleration Z": "MPU6050/Acceleration/Z",
        "MPU6050 Rotation X": "MPU6050/Rotation/X",
        "MPU6050 Rotation Y": "MPU6050/Rotation/Y",
        "MPU6050 Rotation Z": "MPU6050/Rotation/Z",
    }

    # Dictionary to store sensor values
    sensor_values = {}

    # Retrieve data for each sensor
    for sensor_name, sensor_path in sensors.items():
        ref = db.reference(sensor_path)
        sensor_data = ref.get()
        sensor_values[sensor_name] = sensor_data

    # Create a formatted response
    response_text = "------------ ESP32  Status ------------\n\n"
    for sensor_name, sensor_value in sensor_values.items():
        response_text += f"{sensor_name}: {sensor_value}\n\n"

    # Send the response to the user
    await update.message.reply_text(response_text)

async def pushbutton(update: Update, context: ContextTypes.DEFAULT_TYPE):
    # Create an inline keyboard with two buttons: ON and OFF
    keyboard = [
        [InlineKeyboardButton("ON", callback_data='1')],
        [InlineKeyboardButton("OFF", callback_data='0')],
    ]
    reply_markup = InlineKeyboardMarkup(keyboard)

    # Send the keyboard to the user
    message = await update.message.reply_text('Choose the button state:', reply_markup=reply_markup)

    # Save the message ID to be used for later editing
    context.user_data['pushbutton_message_id'] = message.message_id

# Callback function for button presses
async def button_callback(update: Update, context: CallbackContext):
    query = update.callback_query
    button_state = query.data

    # Write the button state to the Firebase database
    ref = db.reference("Button/PB")
    ref.set(int(button_state))

    # Determine the message text based on the button state
    message_text = f'You have turned {"ON" if button_state == "1" else "OFF"} the system'

    # Edit the message text
    await query.edit_message_text(message_text)

    # Send a confirmation message
    await query.answer(f"You have pressed {'ON' if button_state == '1' else 'OFF'} Button")

async def error(update: Update, context: ContextTypes.DEFAULT_TYPE):
    print(f"Update {update} caused error {context.error}")

if __name__ == '__main__':
    print("Bot is ready.")
    app = Application.builder().token(token).build()

    # Commands
    app.add_handler(CommandHandler('start', start))
    app.add_handler(CommandHandler('help', help))
    app.add_handler(CommandHandler('status', status))
    app.add_handler(CommandHandler('pushbutton', pushbutton))

    # Callback for button presses
    app.add_handler(CallbackQueryHandler(button_callback))

    # Errors
    app.add_error_handler(error)

    # Polls the Bot
    print("Polling...")
    app.run_polling(poll_interval=3)
